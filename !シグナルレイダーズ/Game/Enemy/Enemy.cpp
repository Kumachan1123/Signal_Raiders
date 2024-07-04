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
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
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
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
	// HPBar����
	m_HPBar = std::make_unique<EnemyHPBar>();
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
	m_currentHP = hp;
	m_HPBar->SetEnemyHP(m_currentHP);
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
	// �G�̃T�C�Y��ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_enemyAI->GetScale());
	// �G�̍��W��ݒ�
	enemyWorld *= Matrix::CreateFromQuaternion(m_enemyAI->GetRotation());
	enemyWorld *= Matrix::CreateTranslation(m_position);
	enemyWorld *= Matrix::CreateTranslation(Vector3{ 0,-2,0 });


	// ���f���̃G�t�F�N�g�����X�V����
	m_model->UpdateEffects([](DirectX::IEffect* effect)
						   {
							   // �x�[�V�b�N�G�t�F�N�g��ݒ肷��
							   BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);

							   if (basicEffect)
							   {
								   // �ʂ̃��C�g�����ׂĖ���������
								   basicEffect->SetLightEnabled(0, false);
								   basicEffect->SetLightEnabled(1, false);
								   basicEffect->SetLightEnabled(2, false);

								   // ���f����������������
								   basicEffect->SetAmbientLightColor(Colors::White);
								   basicEffect->SetFogStart(1);
							   }


						   }
	);
	// HPBar�`��
	m_HPBar->Render(view, proj, m_position, m_rotate);


	// �G�`��
	m_model->Draw(context, *states, enemyWorld, view, proj);
	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	//Vector3 lightDir = Vector3{ 0.5f,1.0f,-0.5f };
	lightDir.Normalize();
	Matrix shadowMatrix = Matrix::CreateShadow(lightDir, Plane(0, 1, 0, -0.01f));
	Matrix mat = enemyWorld * shadowMatrix;
	m_model->Draw(context, *states, mat, view, proj, false, [&]()
				  {
					  context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
					  context->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
					  context->RSSetState(states->CullCounterClockwise());
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

}
// �X�V
void Enemy::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{

	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit);
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
	{
		// �U���̃N�[���_�E���^�C�����Ǘ�
		m_attackCooldown -= elapsedTime;
		if (m_attackCooldown <= 0.0f)
		{
			// �e�𔭎�
			auto bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize(m_commonResources);
			m_rotation = m_enemyAI->GetRotation();
			// �N�H�[�^�j�I����������x�N�g�����v�Z
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_rotation);
			bullet->MakeBall(GetPosition(), direction, playerPos);
			m_bullets.push_back(std::move(bullet));
			m_attackCooldown = 3.0f; // ���̍U���܂ł̃N�[���_�E���^�C����3�b�ɐݒ�
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
		SetBulletBoundingSphere(bullet->GetBoundingSphere());
		m_isBullethit = GetBulletBoundingSphere().Intersects(GetPlayerBoundingSphere());
		if (m_isBullethit)
		{
			SetBulletHitToPlayer(m_isBullethit);
			newBullets.push_back(std::move(bullet));

		}
		else if (!bullet->IsExpired())//�������}�����e�̓��[�J���ϐ��ɑ���
		{
			newBullets.push_back(std::move(bullet));

		}
	}

	// m_bullets ��V�������X�g�Œu��������
	m_bullets = std::move(newBullets);
}
