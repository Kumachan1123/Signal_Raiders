/*
	@file	Boss.cpp
	@brief	�{�X�N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Boss/Boss.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/Boss/BossModel/BossModel.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>

// �R���X�g���N�^
Boss::Boss(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS()
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_bossModel{}
	, m_pBossAI{}
	, m_HPBar{}
	, m_time{ 0.0f }
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
	, m_bossBulletType{ BossBulletType::NORMAL }
	, m_bulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_audioManager{ AudioManager::GetInstance() }

{}
// �f�X�g���N�^
Boss::~Boss() {}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Boss::Initialize(CommonResources* resources, int hp)
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

	m_bossModel = std::make_unique<BossModel>();
	m_bossModel->Initialize(m_commonResources);
	// �G�̗̑͂�ݒ�
	m_currentHP = hp;
	// HPBar����
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// AI����
	m_pBossAI = std::make_unique<BossAI>();
	m_pBossAI->Initialize();
	//// �e�S�̐���
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);


	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// �����ʒu��ݒ�
	m_position = Vector3(0.0f, 10.0f, 0.0f);
	// �G�̍��W��ݒ�
	m_pBossAI->SetPosition(m_position);


	// ���E���̏�����
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 2.5f;
	// �I�[�f�B�I�}�l�[�W���[
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");

}
// �`��
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ��ƂȂ���W����]���
	Matrix world = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
	// �G�̃T�C�Y��ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale() * 2);
	// �G�̍��W��ݒ�
	enemyWorld *= world;

	// �G�`��	
	m_bossModel->Render(context, states, enemyWorld, view, proj);
	// HPBar�̍��W��ݒ�
	Vector3 hpBarPos = Vector3(m_position.x, m_position.y - 5, m_position.z);
	m_HPBar->SetScale(Vector3(3.0f));
	// HPBar�`��
	m_HPBar->Render(view, proj, hpBarPos, m_rotate);

	// �G�̒e�`��
	m_enemyBullets->Render(view, proj);


#ifdef _DEBUG
	// ���E����`�悷��
	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
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
void Boss::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bossModel->Update(elapsedTime, m_pBossAI->GetState());// ���f���̃A�j���[�V�����X�V
	m_pBossAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AI�̍X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V

	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();
	ShootBullet();// �e����

	m_enemyBullets->Update(elapsedTime, GetPosition());// �G�̒e�̍X�V
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 1.0f;
	// �e�̈ʒu�ݒ�
	BulletPotsitioning();
	// HPBar�X�V
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// �I�u�W�F�N�g���m���Փ˂����牟���߂�
void Boss::CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
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

// �ǂƂ̏Փ˔���
void Boss::CheckHitWall(DirectX::BoundingSphere& A, DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	// �����߂��x�N�g�����v�Z
	Vector3 pushBackVec = Vector3::Zero;
	// ���̂̒��S�ƃ{�b�N�X�̃N�����v���ꂽ�ʒu�̍��������߂�
	Vector3 closestPoint; // �{�b�N�X�̍ł��߂��_

	// �e���ŃN�����v���āA�ł��߂��ʒu���擾
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));

	// ���̂̒��S�ƍł��߂��_�̃x�N�g����
	Vector3 diffVector = A.Center - closestPoint;

	// �������v�Z
	float distance = diffVector.Length();

	// ���������̂̔��a��菬�����ꍇ�͉����߂�����
	if (distance < A.Radius)
	{
		// �����߂��ʂ��v�Z (���K�����ĉ����߂��x�N�g�����쐬)
		float penetrationDistance = A.Radius - distance;
		diffVector.Normalize();
		pushBackVec = diffVector * penetrationDistance;

		m_position += pushBackVec;
		A.Center = m_position;
		A.Center.y -= 2.0f;
	}
}

// �e����
void Boss::ShootBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �U���̃N�[���_�E���^�C�����Ǘ�
	if (m_attackCooldown <= 0.1f)
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		// �N�H�[�^�j�I����������x�N�g�����v�Z
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());

		// �e�𔭎�
		CreateBullet();

		// �N�[���_�E���^�C�������Z�b�g
		m_pBossAI->GetBossAttack()->SetCoolTime(1.5f);
	}
}

// �e�̈ʒu�ݒ�
void Boss::BulletPotsitioning()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	// �e�̔��ˈʒu��ݒ�
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// �����̍��W�ɉ�]��K�p
	m_bulletPosCenter = Vector3::Transform(Vector3(0, 2.5f, 3), transform);
	// ���̍��W�ɉ�]��K�p
	m_bulletPosLeft = Vector3::Transform(Vector3(-2.5f, 0, 3), transform);
	// �E�̍��W�ɉ�]��K�p
	m_bulletPosRight = Vector3::Transform(Vector3(2.5f, 0, 3), transform);
}


// �e�𐶐�
void Boss::CreateBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �p�x�����炵�č��E�̒e�𔭎�
	constexpr float angleOffset = XMConvertToRadians(30.0f); // 30�x�̊p�x�I�t�Z�b�g
	// Enemies�N���X�Őݒ肵���e�̃^�C�v�ɂ���ď����𕪊�
	switch (GetBulletType())
	{
	case BossBulletType::NORMAL:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// �����̒e�𔭎�
		break;
	case BossBulletType::TWIN:

		CreateLeftBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// �E�̒e�𔭎�
		break;
	case BossBulletType::THREE:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// �����̒e�𔭎�
		CreateLeftBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// �E�̒e�𔭎�
		break;
	case BossBulletType::SPIRAL:
		CreateCenterBullet(EnemyBullet::BulletType::SPIRAL);// �����̒e�𔭎�
		CreateLeftBullet(angleOffset, EnemyBullet::BulletType::SPIRAL);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::SPIRAL);// �E�̒e�𔭎�
		CreateVerticalBullet();// �^���ɗ����Ē��e�������������e�𔭎�
		break;
	}

}

// �����̒e�𔭎�
void Boss::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_enemyBullets->CreateBullet(m_bulletPosCenter, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
	m_enemyBullets->SetRotateDirection(-1);// �����e�̉�]������ݒ�i�����
}

// ���̒e�𔭎�
void Boss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ������
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	m_enemyBullets->CreateBullet(m_bulletPosLeft, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// �E�̒e�𔭎�
void Boss::CreateRightBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �E����
	Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);
	Vector3 rightDirection = Vector3::Transform(m_bulletDirection, rightRotation);
	m_enemyBullets->CreateBullet(m_bulletPosRight, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// �^���ɗ�����e�𔭎�
void Boss::CreateVerticalBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �^���ɗ�����e�𔭎�
	m_enemyBullets->CreateBullet(m_bulletPosCenter, Vector3(0, 0, 0), m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, EnemyBullet::BulletType::VERTICAL);
}