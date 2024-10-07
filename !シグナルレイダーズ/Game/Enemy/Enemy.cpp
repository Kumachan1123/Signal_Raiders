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
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>

// �R���X�g���N�^
Enemy::Enemy(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS{}
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_enemyModel{}
	, m_enemyAI{}
	, m_HPBar{}
	, m_bullets{}
	, m_depthStencilState_Shadow{}
	, m_pixelShader{}
	, m_depthStencilState{}
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_enemyBSToPlayerArea{}
	, m_enemyBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{}
	, m_isHit{}
	, m_isHitToOtherEnemy{}
	, m_isHitToPlayerBullet{}
	, m_isBullethit{}
	, m_audioManager{ AudioManager::GetInstance() }


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
	// ���ʃ��\�[�X��ݒ�
	m_commonResources = resources;
	// �f�o�C�X�ƃR���e�L�X�g���擾
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

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
	// �e�p�̃��f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
	m_enemyModel = std::make_unique<EnemyModel>();
	m_enemyModel->Initialize(m_commonResources);
	// �G�̗̑͂�ݒ�
	m_currentHP = hp;
	// HPBar����
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// AI����
	m_enemyAI = std::make_unique<EnemyAI>();
	m_enemyAI->Initialize();
	// �e�S�̐���
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);
	// ��������
	std::random_device rd;  // �V�[�h������
	std::mt19937 gen(rd()); // �����Z���k�E�c�C�X�^�̗���������
	std::uniform_real_distribution<float> dist(-50.0f, 50.0f); // ��l���z
	m_position.x = dist(gen);
	m_position.y = 6.0f;
	m_position.z = dist(gen);
	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// �G�̍��W��ݒ�
	m_enemyAI->SetPosition(m_position);
	// ���E���̏�����
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;
	// �I�[�f�B�I�}�l�[�W���[
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");

}
// �`��
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ��ƂȂ���W����]���
	Matrix world = Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())
		* Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
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
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, -0.01f));
	enemyWorld *= shadowMatrix;
	// �e�`��
	m_model->Draw(context, *states, enemyWorld * Matrix::Identity, view, proj, true, [&]()
				  {
					  context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
					  context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 0);
					  context->RSSetState(states->CullClockwise());
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
	// �G�̒e�`��
	m_enemyBullets->Render(view, proj);
#ifdef _DEBUG
	m_primitiveBatch->Begin();
	if (!m_isHit)
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Black);
		else					DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::White);
	}
	else
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Blue);
		else					 DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Tomato);

	}
	m_primitiveBatch->End();
#endif

}
// �X�V
void Enemy::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_enemyModel->Update(elapsedTime, m_enemyAI->GetState());// ���f���̃A�j���[�V�����X�V
	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AI�̍X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
	{
		m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
		// �U���̃N�[���_�E���^�C�����Ǘ�
		if (m_attackCooldown <= 0.1f)
		{
			m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
			// �N�H�[�^�j�I����������x�N�g�����v�Z
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
			// �e�𔭎�
			m_enemyBullets->CreateBullet(GetPosition(), direction, playerPos);
			// �N�[���_�E���^�C�������Z�b�g
			m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
		}
	}
	m_enemyBullets->Update(elapsedTime, GetPosition());// �G�̒e�̍X�V
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// �I�u�W�F�N�g���m���Փ˂����牟���߂�(���E�����m�̏ꍇ�j
void Enemy::CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
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
	A.Center.y -= 2.0f;
}
// �I�u�W�F�N�g���m���Փ˂����牟���߂�(�I�u�W�F�N�g�ƕǂ̏ꍇ�j
void Enemy::CheckHitWall(DirectX::BoundingSphere& O, DirectX::BoundingBox& W)
{
	using namespace DirectX::SimpleMath;
	Vector3 closestPoint;
	closestPoint.x = std::max(W.Center.x - W.Extents.x, std::min(O.Center.x, W.Center.x + W.Extents.x));
	closestPoint.y = std::max(W.Center.y - W.Extents.y, std::min(O.Center.y, W.Center.y + W.Extents.y));
	closestPoint.z = std::max(W.Center.z - W.Extents.z, std::min(O.Center.z, W.Center.z + W.Extents.z));
	// ���̒��S�ƍŋߐړ_�̃x�N�g�����v�Z
	Vector3 pushDirection = O.Center - closestPoint;
	float distance = pushDirection.Length();
	// �Փ˂��������Ă���ꍇ�ɉ����߂��������s��
	if (distance < O.Radius)
	{
		pushDirection.Normalize();
		float pushDistance = O.Radius - distance;

		// �����߂���K�p
		O.Center = O.Center + (pushDirection * pushDistance);
	}
}




