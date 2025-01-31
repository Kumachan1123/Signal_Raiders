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
#include "Game/Enemy/EnemyManager/EnemyManager.h"

#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
Boss::Boss(Player* pPlayer, CommonResources* resources, int hp)
	: IEnemy(pPlayer, resources, hp)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_bossBS{}
	, m_commonResources{ resources }
	, m_currentHP{ hp }
	, m_maxHP{}
	, m_attackCooldown{ EnemyParameters::ATTACK_COOLDOWN }
	, m_bulletCooldown{ EnemyParameters::ATTACK_INTERVAL }
	, m_SEVolume{ 0.0f }
	, m_SEVolumeCorrection{ 0.0f }
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
	, m_isDead{ false }
	, m_isHitToPlayer{ false }
	, m_isHitToOtherEnemy{ false }
	, m_isEnemyHitByPlayerBullet{ false }
	, m_isPlayerHitByEnemyBullet{ false }
	, m_canAttack{ true }
	, m_bossBulletType{ BossBulletType::NORMAL }
	, m_bulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_audioManager{ }

{
}
// �f�X�g���N�^
Boss::~Boss() {}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Boss::Initialize()
{
	// �����蔻��`��p�N���X�̏�����
	DrawCollision::Initialize(m_commonResources);
	// �{�X���f������
	m_pBossModel = std::make_unique<BossModel>();
	m_pBossModel->Initialize(m_commonResources);
	m_maxHP = m_currentHP;
	// HPBar����
	m_pHPBar = std::make_unique<EnemyHPBar>();
	m_pHPBar->SetEnemyHP(m_currentHP);
	m_pHPBar->Initialize(m_commonResources);
	// AI����
	m_pBossAI = std::make_unique<BossAI>(this);
	m_pBossAI->Initialize();
	// �e�S�̐���
	m_pEnemyBullets = std::make_unique<EnemyBullets>(this);
	m_pEnemyBullets->Initialize(m_commonResources);
	// �V�[���h����
	m_pBossSheild = std::make_unique<BossSheild>(m_maxHP, this);
	m_pBossSheild->Initialize(m_commonResources);
	// �����ʒu��ݒ�
	m_position = EnemyParameters::INITIAL_BOSS_POSITION;
	// �G�̍��W��ݒ�
	m_pBossAI->SetPosition(m_position);
	// ���E���̏�����
	m_bossBS.Center = m_position;
	m_bossBS.Radius = EnemyParameters::NORMAL_BOSS_RADIUS;
	// ���ʂ̐ݒ�
	m_SEVolume = m_pPlayer->GetVolume();
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();
}
// �`��
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �G�̃��[���h�s���ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale())
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
	Vector3 hpBarPos = m_position - EnemyParameters::BOSS_HPBAR_OFFSET;
	m_pHPBar->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));
	// HPBar�`��
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
}
void Boss::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG
	// �`��J�n
	DrawCollision::DrawStart(view, proj);
	// �F�ݒ�
	DirectX::XMVECTOR color = Colors::Black;
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// ��������
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// �������Ă��Ȃ�
	// ���E���`��
	DrawCollision::DrawBoundingSphere(m_bossBS, color);
	// �`��I��
	DrawCollision::DrawEnd();
#endif
}
// �X�V
void Boss::Update(float elapsedTime)
{
	// �J�����̏����擾
	m_cameraEye = m_pCamera->GetEyePosition();
	m_cameraTarget = m_pCamera->GetTargetPosition();
	m_cameraUp = m_pCamera->GetUpVector();
	// �{�X�̃��f���̏�Ԃ��X�V
	m_pBossModel->SetState(m_pBossAI->GetState());// ���f���̃A�j���[�V�����X�V
	m_pBossAI->Update(elapsedTime);// AI�̍X�V
	m_position = m_pBossAI->GetPosition();// �G�̍��W���X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// �U���̃N�[���_�E���^�C�����擾
	this->ShootBullet();// �e����
	m_pEnemyBullets->Update(elapsedTime);// �G�̒e�̍X�V
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// ���E���̒��S���W���X�V
	// �e�̈ʒu�ݒ�
	this->BulletPositioning();
	// HPBar�X�V
	m_pHPBar->SetCurrentHP(m_currentHP);
	m_pHPBar->Update(elapsedTime);
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
	m_audioManager->PlaySound("Barrier", this->GetSheildSEVolume());// �T�E���h�Đ� 
}
//---------------------------------------------------------
// �e����
//---------------------------------------------------------
void Boss::ShootBullet()
{
	// �U���̃N�[���_�E���^�C�����Ǘ�
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		// �N�H�[�^�j�I����������x�N�g�����v�Z
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());
		// �e�𔭎�
		this->CreateBullet();
		// �N�[���_�E���^�C�������Z�b�g
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);
	}

}

// �e�̈ʒu�ݒ�
void Boss::BulletPositioning()
{

	// �e�̔��ˈʒu��ݒ�
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// �����̍��W�ɉ�]��K�p
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);
	// ���̍��W�ɉ�]��K�p
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);
	// �E�̍��W�ɉ�]��K�p
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);
}


// �e�𐶐�
void Boss::CreateBullet()
{
	// �p�x�����炵�č��E�̒e�𔭎�
	float angleOffset = XMConvertToRadians(EnemyParameters::BOSS_BULLET_ANGLE); // 30�x�̊p�x�I�t�Z�b�g
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
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosCenter);
	m_pEnemyBullets->SetDirection(m_bulletDirection);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, type);
}

// ���̒e�𔭎�
void Boss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	// ������
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	// ���ˈʒu��ݒ�
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosLeft);
	m_pEnemyBullets->SetDirection(leftDirection);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, type);
}

// �E�̒e�𔭎�
void Boss::CreateRightBullet(float angleOffset, EnemyBullet::BulletType type)
{
	// �E����
	Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);
	Vector3 rightDirection = Vector3::Transform(m_bulletDirection, rightRotation);
	// ���ˈʒu��ݒ�
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosRight);
	m_pEnemyBullets->SetDirection(rightDirection);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, type);
}

// �^���ɗ�����e�𔭎�
void Boss::CreateVerticalBullet()
{
	// �^���ɗ�����e�𔭎�
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosCenter);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, EnemyBullet::BulletType::VERTICAL);
}

// �G��HP�Ɋւ��鏈��
void Boss::SetEnemyHP(int hp)
{
	// �V�[���h������ꍇ�̓V�[���h��HP�����炷
	if (m_pBossSheild->GetSheildHP() > 0 && m_pBossSheild->GetSheild() == true)
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);
		if (m_pBossSheild->GetSheildHP() <= 0)
		{
			m_audioManager->PlaySound("BarrierBreak", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		}
	}
	else
	{
		// �V�[���h���Ȃ��ꍇ�͓G��HP�����炷
		m_currentHP -= hp;
	}

}
