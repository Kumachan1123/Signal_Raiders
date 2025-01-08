/*
	@file	VerticalAttacker.cpp
	@brief	�����U���G�N���X
*/
#include "pch.h"
#include "VerticalAttacker.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/VerticalAttackerModel/VerticalAttackerModel.h"
#include "Game/Enemy/Enemies/Enemies.h"
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

// �R���X�g���N�^
VerticalAttacker::VerticalAttacker(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS{}
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_pVerticalAttackerModel{}
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
	, m_audioManager{ AudioManager::GetInstance() } {}

// �f�X�g���N�^
VerticalAttacker::~VerticalAttacker() {}

// ����������
void VerticalAttacker::Initialize(CommonResources* resources, int hp)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ�
	m_commonResources = resources;

	DrawCollision::Initialize(resources);
	// ���f����ǂݍ���
	m_pVerticalAttackerModel = std::make_unique<VerticalAttackerModel>();
	m_pVerticalAttackerModel->Initialize(m_commonResources);
	// �G�̗̑͂�ݒ�
	m_currentHP = hp;
	// HPBar����
	m_pHPBar = std::make_unique<EnemyHPBar>();
	m_pHPBar->SetEnemyHP(m_currentHP);
	m_pHPBar->Initialize(resources);
	// AI����
	m_enemyAI = std::make_unique<EnemyAI>(this);
	m_enemyAI->Initialize();
	// �e�S�̐���
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);
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

// �X�V����
void VerticalAttacker::Update(float elapsedTime)
{
	m_pVerticalAttackerModel->SetState(m_enemyAI->GetState());// ���f���̍X�V
	m_enemyAI->Update(elapsedTime);// AI�̍X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
	{
		ShootBullet();// �e�𔭎�
	}
	m_enemyBullets->Update(elapsedTime);// �G�̒e�̍X�V
	// �G�̓����蔻��̍��W���X�V
	m_enemyBS.Center = m_position;
	m_pHPBar->SetCurrentHP(m_currentHP);
	m_pHPBar->Update(elapsedTime);
	m_isDead = m_pHPBar->GetIsDead();// �G��HP��0�ɂȂ����玀�S
}

// �`��
void VerticalAttacker::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
	m_pVerticalAttackerModel->Render(context, states, world, view, proj);
	// �G�̒e�`��
	m_enemyBullets->Render(view, proj);
}

void VerticalAttacker::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG
	// �`�悷��
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
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

void VerticalAttacker::ShootBullet()
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
		m_enemyBullets->CreateBullet(0.15f, EnemyBullet::BulletType::VERTICAL);
		// �N�[���_�E���^�C�������Z�b�g
		m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
	}

}
