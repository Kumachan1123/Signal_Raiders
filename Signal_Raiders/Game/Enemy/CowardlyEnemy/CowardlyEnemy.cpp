/*
	@file	CowardlyEnemy.cpp
	@brief	���΂��������Ĕڋ��ȓG�N���X
*/
#include <pch.h>
#include "CowardlyEnemy.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

/*
*	@brief	�R���X�g���N�^
*	@details�@���������s��
*	@param	pPlayer	�v���C���[�̃|�C���^�[
*	@param	pCommonResources	���ʃ��\�[�X
*	@param	hp	�G��HP
*	@return	�Ȃ�
*/
CowardlyEnemy::CowardlyEnemy(Player* pPlayer, CommonResources* pCommonResources, int hp)
	: m_pPlayer{ pPlayer }// �v���C���[�̃|�C���^�[
	, m_pCamera{ pPlayer->GetCamera() }// �J�����̃|�C���^�[
	, m_enemyBS{}// �G�̋��E��
	, m_pCommonResources{ pCommonResources }// ���ʃ��\�[�X
	, m_currentHP{ hp }// �G��HP
	, m_pBulletManager{ nullptr }// �e�Ǘ��N���X�̃|�C���^�[
	, m_attackCooldown{ EnemyParameters::ATTACK_COOLDOWN }// �U���̃N�[���_�E���^�C��
	, m_pEnemyModel{}// �G�̃��f��
	, m_pEnemyAI{}// �G��AI
	, m_pHPBar{}// �G��HP�o�[
	, m_position{}// �G�̍��W
	, m_velocity{}// �G�̑��x
	, m_rotate{}// �G�̉�]
	, m_enemyBSToPlayerArea{}// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	, m_enemyBulletBS{}// �G�̒e�̋��E��
	, m_playerBS{}// �v���C���[�̋��E��
	, m_matrix{}// �}�g���N�X
	, m_isDead{ false }// �G��HP��0�ɂȂ�����True
	, m_isHitToPlayer{ false }// �v���C���[�Ƃ̔���
	, m_isHitToOtherEnemy{ false }// ���̑��̓G�Ƃ̔���
	, m_isEnemyHitByPlayerBullet{ false }// �G���v���C���[�̒e�ɓ���������
	, m_isPlayerHitByEnemyBullet{ false }// �G�̒e���v���C���[�ɓ���������
	, m_canAttack{ true }// �U���\��
	, m_isAttack{ false }// �U������
{}
/*
*	@brief	�f�X�g���N�^
*	@details	���S�����G�̒e���폜����
*	@param	�Ȃ�
*/
CowardlyEnemy::~CowardlyEnemy()
{
	// �e���폜����
	m_pBulletManager->RemoveBulletsByShooter(this);
}
/*
*	@brief	����������
*	@details	�G�̏��������s��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void CowardlyEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;
	// �����蔻��`��N���X�̏�����
	DrawCollision::Initialize(m_pCommonResources);
	// �G�̃��f����ǂݍ���
	m_pEnemyModel = std::make_unique<CowardlyEnemyModel>();
	// ���f���̏�����
	m_pEnemyModel->Initialize(m_pCommonResources);
	// HP�o�[����
	m_pHPBar = std::make_unique<EnemyHPBar>();
	// �ő�HP��ݒ�
	m_pHPBar->SetEnemyMaxHP(m_currentHP);
	// HP�o�[������
	m_pHPBar->Initialize(m_pCommonResources);
	// AI����
	m_pEnemyAI = std::make_unique<EnemyAI>(this);
	// ��Ԃ�ǐՏ�Ԃɂ���
	m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyChase());
	// AI������
	m_pEnemyAI->Initialize();
	// ��������
	Vector3 position = Vector3(GenerateRandomMultiplier(-EnemyParameters::ENEMY_SPAWN_RADIUS, EnemyParameters::ENEMY_SPAWN_RADIUS));
	// �G�̏����ʒu��ݒ�
	m_position = Vector3{ position.x, 0.0f,position.z };
	// �G�̍��W��ݒ�
	m_pEnemyAI->SetPosition(m_position);
	// ���E���̍��W�ݒ�
	m_enemyBS.Center = m_position;
	// ���E���̔��a�ݒ�
	m_enemyBS.Radius = EnemyParameters::NORMAL_ENEMY_RADIUS;
}
/*
*	@brief	�X�V
*	@param	elapsedTime	�o�ߎ���
*	@return	�Ȃ�
*/
void CowardlyEnemy::Update(float elapsedTime)
{
	// ���f���̍X�V
	m_pEnemyModel->SetState(m_pEnemyAI->GetState());
	// HP�̍X�V
	m_pHPBar->SetCurrentHP(m_currentHP);
	// HP�o�[�̍X�V
	m_pHPBar->Update(elapsedTime);
	// AI�̍X�V
	m_pEnemyAI->Update(elapsedTime);
	// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_pCommonResources->GetAudioManager()->Update();
	// �U���Ԑ��Ȃ�e�𔭎�
	/*if (m_pEnemyAI->GetNowState() == m_pEnemyAI->GetEnemyAttack())*/ShootBullet(elapsedTime);
	// �G�̓����蔻��̍��W���X�V
	m_enemyBS.Center = m_position;
	// �G��HP��0�ɂȂ����玀�S
	m_isDead = m_pHPBar->GetIsDead();
}
/*
*	@brief	�`��
*	@details	�G�̕`����s��
*	@param	view	�r���[�s��
*	@param	proj	�ˉe�s��
*	@return	�Ȃ�
*/
void CowardlyEnemy::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g�擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g�擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���[���h�s���ݒ�i�X�P�[�� �� ��] �� �ʒu�j
	Matrix world = Matrix::CreateScale(m_pEnemyAI->GetScale())
		* Matrix::CreateFromQuaternion(m_pEnemyAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// HPBar�̍��W��ݒ�
	Vector3 hpBarPos = m_position + EnemyParameters::ENEMY_HPBAR_OFFSET;
	// HPBar�`��
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
	// �G�`��	
	m_pEnemyModel->Render(context, states, world, view, proj);
}
/*
*	@brief	�G�̓����蔻���`��
*	@details	�G�̓����蔻���`�悷��
*	@param	view	�r���[�s��
*	@param	proj	�ˉe�s��
*	@return	�Ȃ�
*/
void CowardlyEnemy::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	// ���g�p�ϐ��x���𖳎�
	UNREFERENCED_PARAMETER(view);
	// ���g�p�ϐ��x���𖳎�
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG// �f�o�b�O�r���h���̂ݎ��s
	// �`��J�n
	DrawCollision::DrawStart(view, proj);
	// �F�ݒ�
	DirectX::XMVECTOR color = Colors::Black;
	// �v���C���[�ɓ������Ă���ꍇ�F���̓G�ɂ��������Ă����Tomato�A�������Ă��Ȃ����Blue
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;
	// �v���C���[�ɓ������Ă��Ȃ��ꍇ�F���̓G�ɓ������Ă����White�A�������Ă��Ȃ����Black
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;
	// ���E���`��
	DrawCollision::DrawBoundingSphere(m_enemyBS, color);
	// �`��I��
	DrawCollision::DrawEnd();
#endif
}
/*
*	@brief	�G�̒e�𔭎�
*	@details	�G�̒e�𔭎˂���
*	@param	elapsedTime �o�ߎ���
*	@return	�Ȃ�
*/
void CowardlyEnemy::ShootBullet(float elapsedTime)
{
	// �N�[���_�E���^�C�����擾
	m_attackCooldown = m_pEnemyAI->GetAttackCooldown();
	// �U���̃N�[���_�E���^�C�����Ǘ�
	if (m_attackCooldown <= elapsedTime)
	{
		// �e�̔��ˉ��Đ� 
		m_pCommonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
		// �N�H�[�^�j�I����������x�N�g�����v�Z
		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_pEnemyAI->GetRotation());
		// �e�̎�ނ�ݒ�
		m_pBulletManager->SetEnemyBulletType(BulletType::NORMAL);
		// �e�̃T�C�Y��ݒ�
		m_pBulletManager->SetEnemyBulletSize(EnemyParameters::ENEMY_BULLET_SIZE);
		// �e�̔��ˎ҂�ݒ�
		m_pBulletManager->SetShooter(this);
		// �e�𐶐�
		m_pBulletManager->CreateEnemyBullet(m_position, direction);
		// �N�[���_�E���^�C�������Z�b�g
		m_pEnemyAI->SetAttackCooldown(EnemyParameters::ATTACK_COOLDOWN);
	}
}
