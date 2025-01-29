/*
	@file	Enemy.cpp
	@brief	�G�N���X
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
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/KumachiLib.h"
#include <Game/KumachiLib/DrawCollision/DrawCollision.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;


// �R���X�g���N�^
Enemy::Enemy(Player* pPlayer, CommonResources* resources, int hp)
	: IEnemy(pPlayer, resources, hp)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS{}
	, m_commonResources{ resources }
	, m_currentHP{ hp }
	, m_attackCooldown{ 3.0f }
	, m_enemyModel{}
	, m_enemyAI{}
	, m_pHPBar{}
	, m_bullets{}
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
	, m_audioManager{ AudioManager::GetInstance() }


{}
// �f�X�g���N�^
Enemy::~Enemy() {}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Enemy::Initialize()
{

	// �f�o�b�O�p�̏�����
	DrawCollision::Initialize(m_commonResources);

	// �G�̃��f����ǂݍ���
	m_enemyModel = std::make_unique<EnemyModel>();
	m_enemyModel->Initialize(m_commonResources);
	// HPBar����
	m_pHPBar = std::make_unique<EnemyHPBar>();
	m_pHPBar->SetEnemyHP(m_currentHP);
	m_pHPBar->Initialize(m_commonResources);
	// AI����
	m_enemyAI = std::make_unique<EnemyAI>(this);
	m_enemyAI->Initialize();
	// �e�S�̐���
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(m_commonResources);
	// ��������
	Vector3 position = Vector3(GenerateRandomMultiplier(-50.0f, 50.0f)); // ��l���z
	// �G�̏����ʒu��ݒ�
	m_position = Vector3{ position.x, 0.0f,position.z };// �G�̏����ʒu��ݒ�
	// �G�̍��W��ݒ�
	m_enemyAI->SetPosition(m_position);
	// ���E���̏�����
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;

}
// �X�V
void Enemy::Update(float elapsedTime)
{

	m_enemyModel->SetState(m_enemyAI->GetState());// ���f���̍X�V
	m_pHPBar->SetCurrentHP(m_currentHP);
	m_pHPBar->Update(elapsedTime);
	m_enemyAI->Update(elapsedTime);// AI�̍X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
	{
		ShootBullet();// �e�𔭎�
	}
	m_enemyBullets->Update(elapsedTime);// �G�̒e�̍X�V
	// �G�̓����蔻��̍��W���X�V
	m_enemyBS.Center = m_position;
	m_isDead = m_pHPBar->GetIsDead();// �G��HP��0�ɂȂ����玀�S
}
// �`��
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ��ƂȂ���W����]���
	Matrix world = Matrix::CreateScale(m_enemyAI->GetScale())
		* Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// HPBar�̍��W��ݒ�
	Vector3 hpBarPos = Vector3(m_position.x, m_position.y - 1, m_position.z);
	// HPBar�`��
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
	// �G�`��	
	m_enemyModel->Render(context, states, world, view, proj);

	// �G�̒e�`��
	m_enemyBullets->Render(view, proj);

}
void Enemy::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG
	// �`��J�n
	DrawCollision::DrawStart(view, proj);
	// �F�ݒ�
	DirectX::XMVECTOR color = Colors::Black;
	// ��������
	if (m_isHitToPlayer) color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;
	// �������Ă��Ȃ�
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;
	// ���E���`��
	DrawCollision::DrawBoundingSphere(m_enemyBS, color);
	// �`��I��
	DrawCollision::DrawEnd();
#endif
}

// �G�̒e�𔭎�
void Enemy::ShootBullet()
{
	m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
	// �U���̃N�[���_�E���^�C�����Ǘ�
	if (m_attackCooldown <= ATTACK_INTERVAL)
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		// �N�H�[�^�j�I����������x�N�g�����v�Z
		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
		// �e����ԕ�����ݒ�
		m_enemyBullets->SetDirection(direction);
		// ���ˈʒu��ݒ�
		m_enemyBullets->SetEnemyPosition(m_position);
		// �e�𔭎�
		m_enemyBullets->CreateBullet(0.15f, EnemyBullet::BulletType::STRAIGHT);
		// �N�[���_�E���^�C�������Z�b�g
		m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
	}
}
