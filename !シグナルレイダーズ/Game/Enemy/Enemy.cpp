/*
	@file	Enemy.cpp
	@brief	�G�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>

// �R���X�g���N�^
Enemy::Enemy()
	: m_enemyBoundingSphere()
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{}
	, m_enemyModel{}
	, m_enemyAI{}
	, m_HPBar{}
	, m_bullets{}
	, m_depthStencilState_Shadow{}
	, m_pixelShader{}
	, m_depthStencilState{}
	, m_blendState{}
	, m_outlinePS{}
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_accele{}
	, m_nowScale{}
	, m_startScale{}
	, m_endScale{}
	, m_rotation{}
	, m_enemyBoundingSphereToPlayer{}
	, m_enemyWBoundingSphere{}
	, m_enemyBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{}
	, m_isHit{}
	, m_isHitToOtherEnemy{}
	, m_isHitToPlayerBullet{}
	, m_isBullethit{}
{}
// �f�X�g���N�^
Enemy::~Enemy() {}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Enemy::Initialize(CommonResources* resources, int hp)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	m_attackCooldown = 3.0f;
	/*
	�f�o�b�O�h���[�̕\���p�I�u�W�F�N�g�𐶐�����
	*/
	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/PS_EnemyShadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));


	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Enemy");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);

	m_enemyModel = std::make_unique<EnemyModel>();
	m_enemyModel->Initialize(m_commonResources);
	// ���Z����
	CD3D11_DEFAULT defaultSettings{};
	CD3D11_BLEND_DESC blendDesc(defaultSettings);
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
	// �u�����h�X�e�[�g���쐬����
	DX::ThrowIfFailed(device->CreateBlendState(&blendDesc, m_blendState.ReleaseAndGetAddressOf()));
	// �[�x�X�e���V���o�b�t�@������������
	this->InitializeDepthStencilState(device);

	// �A�E�g���C���V�F�[�_�[���쐬����
	std::vector<uint8_t> psBlob = DX::ReadData(L"Resources/Shaders/PS_Outline.cso"); // ���O�ɃR���p�C�����ꂽ�V�F�[�_�[�o�C�i��
	DX::ThrowIfFailed(
		device->CreatePixelShader(
			psBlob.data(),
			psBlob.size(),
			nullptr,
			m_outlinePS.ReleaseAndGetAddressOf()
		)
	);
	m_currentHP = hp;
	// HPBar����
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// AI����
	m_enemyAI = std::make_unique<EnemyAI>();
	m_enemyAI->Initialize();
	std::random_device rd;  // �V�[�h������
	std::mt19937 gen(rd()); // �����Z���k�E�c�C�X�^�̗���������
	std::uniform_real_distribution<float> dist(-25.0f, 25.0f); // ��l���z
	m_position.x = dist(gen);
	m_position.y = 6.0f;
	m_position.z = dist(gen);
	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);

	m_enemyAI->SetPosition(m_position);


	// ���E���̏�����
	m_enemyBoundingSphere.Center = m_position;
	m_enemyBoundingSphere.Radius = 1.5f;

}
// �`��
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ��ƂȂ���W����]���
	Matrix world = Matrix::CreateFromQuaternion(m_enemyAI->GetRotation()) * Matrix::CreateTranslation(m_position) * Matrix::CreateTranslation(Vector3{ 0,-2,0 });

	// �G�̃T�C�Y��ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_enemyAI->GetScale());
	// �G�̍��W��ݒ�
	enemyWorld *= world;



	// HPBar�`��
	m_HPBar->Render(view, proj, m_position, m_rotate);
	// �G�`��	
	m_enemyModel->Render(context, states, enemyWorld, view, proj);




	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	//Vector3 lightDir = Vector3{ 0.5f,1.0f,-0.5f };
	lightDir.Normalize();
	// �e�s��̌������
		//** Plane(�@���A����)�FTK�̐�����A�@���̌������t�Ȃ̂ŁA������l������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, -0.01f));
	Matrix mat = enemyWorld * shadowMatrix;
	// �e�`��
	m_model->Draw(context, *states, mat * Matrix::Identity, view, proj, true, [&]()
				  {
					  context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
					  context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 1);
					  context->RSSetState(states->CullNone());
					  context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
				  });
	// �`�悷��
	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);

	for (const auto& bullet : m_bullets)bullet->Render(view, proj);
#ifdef _DEBUG
	m_primitiveBatch->Begin();
	if (!m_isHit)
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::Black);
		else					DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::White);
	}
	else
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::Blue);
		else					 DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::Tomato);

	}
	m_primitiveBatch->End();
#endif

}
// �X�V
void Enemy::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_enemyModel->Update(elapsedTime, m_enemyAI->GetState());
	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
	{
		m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
		// �U���̃N�[���_�E���^�C�����Ǘ�

		if (m_attackCooldown <= 0.1f)
		{
			// �e�𔭎�
			auto bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize(m_commonResources);
			m_rotation = m_enemyAI->GetRotation();
			// �N�H�[�^�j�I����������x�N�g�����v�Z
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_rotation);
			bullet->MakeBall(GetPosition(), direction, playerPos);
			m_bullets.push_back(std::move(bullet));
			m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
		}
	}
	UpdateBullets(elapsedTime);
	m_enemyBoundingSphere.Center = m_position;
	m_enemyBoundingSphere.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// �G���m���Փ˂����牟���߂�
void Enemy::CheckHitOtherEnemy(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
{
	using namespace DirectX::SimpleMath;
	// �����߂�����
	// �`�̒��S�Ƃa�̒��S�Ƃ̍����x�N�g���i�a����`�Ɍ������x�N�g���j�c�@
	Vector3 diffVector = A.Center - B.Center;
	// �`�̒��S�Ƃa�̒��S�Ƃ̋����i�@�̒����j�c�A
	float distance = diffVector.Length();
	// �`�̔��a�Ƃa�̔��a�̍��v�c�B
	float sumRadius = A.Radius + B.Radius;
	// �i�`���a�Ɂj�߂荞�񂾋����i�B�|�A�j�c�C
	float penetrationDistance = sumRadius - distance;
	// �@�𐳋K������c�D
	diffVector.Normalize();
	// �����߂��x�N�g�����v�Z����i�D�ƇC�ŕ\������j�c�E
	Vector3 pushBackVec = diffVector * penetrationDistance;
	// �E���g�p���āA�`�̍��W�Ƃ`�̃R���C�_�[���W���X�V����i���ۂɉ����߂��j
	m_position += pushBackVec;
	A.Center = m_position;
}



void Enemy::UpdateBullets(float elapsedTime)
{
	std::vector<std::unique_ptr<EnemyBullet>> newBullets;

	// �e�̍X�V�ƗL���Ȓe��V�������X�g�Ɉړ�����
	for (auto& bullet : m_bullets)
	{
		bullet->Update(m_position, elapsedTime); // �e�̍X�V

		// �������}���Ă��Ȃ��e������V�������X�g�ɒǉ�����
		if (!bullet->IsExpired() && !m_isBullethit)
		{
			SetBulletBoundingSphere(bullet->GetBoundingSphere());
			m_isBullethit = GetBulletBoundingSphere().Intersects(GetPlayerBoundingSphere());
			if (m_isBullethit)
			{
				SetBulletHitToPlayer(m_isBullethit);
				continue;
			}
			newBullets.push_back(std::move(bullet));
		}
	}

	// m_bullets ��V�������X�g�Œu��������
	m_bullets = std::move(newBullets);
}

//---------------------------------------------------------
// �[�x�X�e���V���X�e�[�g������������
//---------------------------------------------------------
void Enemy::InitializeDepthStencilState(ID3D11Device* device)
{
	assert(device);

	// �[�x�X�e���V���o�b�t�@��ݒ肷��
	D3D11_DEPTH_STENCIL_DESC desc{};

	/*
		���̐ݒ�
	*/
	// ���i�`�掞�O���P�ɂ���j
	desc.DepthEnable = TRUE;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �e�X�g�̏����F�[�x�l�ȉ��Ȃ灨��O�Ȃ�

	desc.StencilEnable = TRUE;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// �\��
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// �Q�ƒl���X�e���V���l�Ɠ����Ȃ�F�[���Ȃ�
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK�@�X�e���V���l���C���N�������g����
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG�@�������Ȃ�

	// ���ʂ������ݒ�
	desc.BackFace = desc.FrontFace;


	/*
		�e�̐ݒ�
	*/
	// �e�i�X�e���V���l���P�̂Ƃ��`�悷��j
	//desc.DepthEnable = TRUE;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;			// �[�x�o�b�t�@�͍X�V���Ȃ�
	//desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �e�X�g�̏����F�[�x�l�ȉ��Ȃ灨��O�Ȃ�

	//desc.StencilEnable = TRUE;									// �X�e���V���e�X�g���s��
	//desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	//desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// �\��
	//desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// �Q�ƒl�ƃX�e���V���l�����l�Ȃ�F�P�Ȃ�
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK�@�X�e���V���l���C���N�������g����
	//desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�
	//desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG�@�������Ȃ�

	// ���ʂ������ݒ�
	desc.BackFace = desc.FrontFace;

	// �[�x�X�e���V���X�e�[�g���쐬����
	device->CreateDepthStencilState(&desc, m_depthStencilState_Shadow.ReleaseAndGetAddressOf());
}
