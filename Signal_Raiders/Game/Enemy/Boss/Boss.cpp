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
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"

// �R���X�g���N�^
Boss::Boss(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_bossBS{}
	, m_commonResources{}
	, m_currentHP{}
	, m_maxHP{}
	, m_attackCooldown{ 3.0f }
	, m_bulletCooldown{ 1.0f }
	, m_pBossModel{}
	, m_pBossAI{}
	, m_pHPBar{}
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
{
}
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
	// �����蔻��`��p�N���X�̏�����
	DrawCollision::Initialize(m_commonResources);
	// �{�X���f������
	m_pBossModel = std::make_unique<BossModel>();
	m_pBossModel->Initialize(m_commonResources);
	// �G�̗̑͂�ݒ�
	m_currentHP = hp;
	m_maxHP = hp;
	// HPBar����
	m_pHPBar = std::make_unique<EnemyHPBar>();
	m_pHPBar->SetEnemyHP(m_currentHP);
	m_pHPBar->Initialize(m_commonResources);
	// AI����
	m_pBossAI = std::make_unique<BossAI>();
	m_pBossAI->Initialize();
	// �e�S�̐���
	m_pEnemyBullets = std::make_unique<EnemyBullets>(this);
	m_pEnemyBullets->Initialize(m_commonResources);
	// �V�[���h����
	m_pBossSheild = std::make_unique<BossSheild>(m_maxHP, this);
	m_pBossSheild->Initialize(m_commonResources);
	// �����ʒu��ݒ�
	m_position = INITIAL_POSITION;
	// �G�̍��W��ݒ�
	m_pBossAI->SetPosition(m_position);
	// ���E���̏�����
	m_bossBS.Center = m_position;
	m_bossBS.Radius = SPHERE_RADIUS;
	// �I�[�f�B�I�}�l�[�W���[
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");// �e���ˉ�
	m_audioManager->LoadSound("Resources/Sounds/Barrier.mp3", "Barrier");// �V�[���h��
	m_audioManager->LoadSound("Resources/Sounds/BarrierBreak.mp3", "BarrierBreak");// �V�[���h�j��
}
// �`��
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �G�̃��[���h�s���ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale() * 2)
		* Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// �V�[���h�̍��W��ݒ�
	m_pBossSheild->SetPosition(m_bossBS.Center);
	m_pBossSheild->SetRotation(m_pBossAI->GetRotation());
	// �G�`��	
	m_pBossModel->Render(context, states, enemyWorld, view, proj);
	// �V�[���h�`��
	m_pBossSheild->Render(context, states, enemyWorld, view, proj);
	// �G�̒e�`��
	m_pEnemyBullets->Render(view, proj);
	// HPBar�̍��W��ݒ�
	Vector3 hpBarPos = m_position - HPBAR_OFFSET;
	m_pHPBar->SetScale(Vector3(HPBAR_SCALE));
	// HPBar�`��
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
}
void Boss::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
#ifdef _DEBUG
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �`��J�n
	DrawCollision::DrawStart(view, proj);
	// �F�ݒ�
	DirectX::XMVECTOR color = Colors::Black;
	if (m_isHit)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// ��������
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// �������Ă��Ȃ�
	// ���E���`��
	DrawCollision::DrawBoundingSphere(m_bossBS, color);
	// �`��I��
	DrawCollision::DrawEnd();
#endif
}
// �X�V
void Boss::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pBossModel->Update(elapsedTime, m_pBossAI->GetState());// ���f���̃A�j���[�V�����X�V
	m_pBossAI->Update(elapsedTime, playerPos, m_isHit, m_isHitToPlayerBullet);// AI�̍X�V
	m_position = m_pBossAI->GetPosition();// �G�̍��W���X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();
	ShootBullet();// �e����
	m_pEnemyBullets->Update(elapsedTime, m_position);// �G�̒e�̍X�V
	m_bossBS.Center = m_position + SPHERE_OFFSET;// ���E���̒��S���W���X�V
	// �e�̈ʒu�ݒ�
	BulletPositioning();
	// HPBar�X�V
	m_pHPBar->Update(elapsedTime, m_currentHP);
	// �ő�HP�̔����ɂȂ�����V�[���h��W�J
	if (m_currentHP <= m_maxHP / 2)m_pBossSheild->SetSheild(true);
	// �V�[���h�X�V
	m_pBossSheild->Update(elapsedTime);
	// ���񂾂��ǂ������󂯎��
	m_isDead = m_pHPBar->GetIsDead();
}
//---------------------------------------------------------
// �o���ASE�Đ�
//---------------------------------------------------------
void Boss::PlayBarrierSE()
{
	m_audioManager->PlaySound("Barrier", m_pPlayer->GetVolume() / 2);// �T�E���h�Đ� 
}
//---------------------------------------------------------
// �e����
//---------------------------------------------------------
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
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);
	}

}

// �e�̈ʒu�ݒ�
void Boss::BulletPositioning()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	// �e�̔��ˈʒu��ݒ�
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// �����̍��W�ɉ�]��K�p
	m_bulletPosCenter = Vector3::Transform(TOP_OFFSET, transform);
	// ���̍��W�ɉ�]��K�p
	m_bulletPosLeft = Vector3::Transform(LEFT_OFFSET, transform);
	// �E�̍��W�ɉ�]��K�p
	m_bulletPosRight = Vector3::Transform(RIGHT_OFFSET, transform);
}


// �e�𐶐�
void Boss::CreateBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �p�x�����炵�č��E�̒e�𔭎�
	float angleOffset = XMConvertToRadians(ANGLE_OFFSET); // 30�x�̊p�x�I�t�Z�b�g
	// Enemies�N���X�Őݒ肵���e�̃^�C�v�ɂ���ď����𕪊�
	switch (GetBulletType())
	{
	case BossBulletType::NORMAL:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// �����̒e�𔭎�
		break;
	case BossBulletType::TWIN:

		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::STRAIGHT);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// �E�̒e�𔭎�
		break;
	case BossBulletType::THREE:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// �����̒e�𔭎�
		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::STRAIGHT);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// �E�̒e�𔭎�
		break;
	case BossBulletType::SPIRAL:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// �����̒e�𔭎�
		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::SPIRAL);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::SPIRAL);// �E�̒e�𔭎�
		CreateVerticalBullet();// �^���ɗ����Ē��e�������������e�𔭎�
		break;
	}

}

// �����̒e�𔭎�
void Boss::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_pEnemyBullets->CreateBullet(m_bulletPosCenter, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// ���̒e�𔭎�
void Boss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ������
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	m_pEnemyBullets->CreateBullet(m_bulletPosLeft, m_bulletDirection, m_pPlayer->GetPlayerPos(),
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
	m_pEnemyBullets->CreateBullet(m_bulletPosRight, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// �^���ɗ�����e�𔭎�
void Boss::CreateVerticalBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �^���ɗ�����e�𔭎�
	m_pEnemyBullets->CreateBullet(m_bulletPosCenter, Vector3::Zero, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, EnemyBullet::BulletType::VERTICAL);
}
void Boss::SetEnemyHP(int hp)
{
	// �V�[���h������ꍇ�̓V�[���h��HP�����炷
	if (m_pBossSheild->GetSheildHP() > 0 && m_pBossSheild->GetSheild() == true)
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);
		if (m_pBossSheild->GetSheildHP() <= 0)
		{
			m_audioManager->PlaySound("BarrierBreak", m_pPlayer->GetVolume() * 3);// �T�E���h�Đ� 
		}
	}
	else //if (m_pBossSheild->GetSheildHP() <= 0)
	{
		// �V�[���h���Ȃ��ꍇ�͓G��HP�����炷
		m_currentHP -= hp;
	}

}
