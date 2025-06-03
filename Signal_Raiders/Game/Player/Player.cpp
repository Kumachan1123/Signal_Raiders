/*
*	@file	Player.cpp
*	@brief	�v���C���[�N���X
*/
#include <pch.h>
#include "Player.h"
/*
*	@brief	�R���X�g���N�^
*	@details �v���C���[�N���X�̃R���X�g���N�^
*	@param	commonResources	���ʃ��\�[�X
*	@return �Ȃ�
*/
Player::Player(CommonResources* commonResources)
	: m_pCommonResources{ commonResources }// ���ʃ��\�[�X
	, m_playerHP{  }// �v���C���[��HP
	, m_maxPlayerHP{  }// �v���C���[�̍ő�HP
	, m_pPlayerController{}// �v���C���[�R���g���[���[
	, m_inPlayerArea{}// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	, m_playerSphere{}// �v���C���[�̋��E��
	, m_pCamera{ nullptr }// �J����
	, m_pEnemyManager{ nullptr }// �G�}�l�[�W���[
	, m_pBulletManager{ nullptr }// �e�}�l�[�W���[
	, m_mouseSensitive{  }// �}�E�X���x
	, m_isDamage{ false }// �_���[�W���󂯂���
	, m_isPlayEffect{ false }// �G�t�F�N�g�Đ��t���O
	, m_damageTime{ 0.0f }// �_���[�W���󂯂�����
	, m_timer{ 0.0f }// �o�ߎ���
	, m_SEVolume{ 0.0f }// SE�̉���
	, m_VolumeCorrection{ 0.0f }// ���ʕ␳
	, m_playerDir{}// �v���C���[�̌���
	, m_dashTime{ 0.0f }// �v���C���[������鎞��
	, m_isKillAll{ false }// �`�[�g�R�}���h�F�G����|����t���O
	, m_isCheat{ false }// �`�[�g�R�}���h���L����
{
	// �v���C���[�ƓG�Ƃ̈��͈͂̋��E���̔��a
	m_inPlayerArea.Radius = IN_PLAYER_AREA_RADIUS;
	// �v���C���[�̋��E���̔��a
	m_playerSphere.Radius = PLAYER_SPHERE_RADIUS;
}
/*
*	@brief	�f�X�g���N�^
*	@details �v���C���[�N���X�̃f�X�g���N�^
*	@param	�Ȃ�
*	@return �Ȃ�
*/
Player::~Player()
{
	// �v���C���[�R���g���[���[�̉��
	m_pPlayerController.reset();
	// �J�����̉��
	m_pCamera.reset();
	// �_���[�W�G�t�F�N�g�̉��
	m_pWarningEffects.reset();
	// �e�}�l�[�W���[�̉��
	m_pBulletManager = nullptr;
	// �G�}�l�[�W���[�̉��
	m_pEnemyManager = nullptr;
}
/*
*	@brief	������
*	@details �v���C���[�N���X�̏�����
*	@param	enemyManager	�G�}�l�[�W���[
*	@return �Ȃ�
*/
void Player::Initialize(EnemyManager* pEnemiyManager)
{
	// �v���C���[��HP��ݒ�
	m_playerHP = PLAYER_HP;
	// �v���C���[�̍ő�HP��ݒ�
	m_maxPlayerHP = m_playerHP;
	// �G�}�l�[�W���[�̃|�C���^�[��n��
	m_pEnemyManager = pEnemiyManager;
	// FPS�J�������쐬����
	m_pCamera = std::make_unique<FPS_Camera>();
	// �v���C���[�̃R���g���[���[���쐬����
	m_pPlayerController = std::make_unique<PlayerController>(this);
	// �v���C���[�R���g���[���[�̏�����
	m_pPlayerController->Initialize(m_pCommonResources);
	// �v���C���[�̈ʒu��ݒ�
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
	// �_���[�W�G�t�F�N�g���Ǘ�����N���X���쐬����
	m_pWarningEffects = std::make_unique<WarningEffects>(m_pCommonResources);
	// �_���[�W�G�t�F�N�g�̏�����
	m_pWarningEffects->Initialize(this, m_pEnemyManager);
}
/*
*	@brief	�v���C���[�̍X�V
*	@details �v���C���[�N���X�̍X�V
*	@param	elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Player::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_timer += elapsedTime;
	// �J�����������Ă���������擾����
	m_playerDir = m_pCamera->GetDirection();
	// �J�����X�V
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYaw());
	// �v���C���[�R���g���[���[�X�V
	m_pPlayerController->Update(elapsedTime);
	// �G���U�����Ă������̏���
	InComingEnemy();
	// �_���[�W��H��������̏���
	PlayerDamage(elapsedTime);
	// �v���C���[�̈ʒu���擾
	m_playerPos = m_pPlayerController->GetPlayerPosition();
	// ����鎞�Ԃ��X�V
	m_dashTime = m_pPlayerController->GetDashTime();
#ifdef _DEBUG// �f�o�b�O �`�[�g�R�}���h
	// �f�o�b�O�R�}���h�����s
	m_pPlayerController->DebugCommand();
#endif
	// Wi-Fi���瓾���ǉ��_���[�W���v���C���[�̒e�ɔ��f
	m_pBulletManager->SetAdditionalDamage(m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());
	// �e�𔭎˂���
	m_pPlayerController->Shoot();
	// �e�̃����[�h����
	m_pPlayerController->Reload();
	// �_���[�W�G�t�F�N�g���X�V����
	m_pWarningEffects->Update(elapsedTime);
	// �v���C���[�̋��E�����X�V
	// �v���C���[�̈ʒu���擾���A���E���̒��S��ݒ�
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();
	m_playerSphere.Center = m_inPlayerArea.Center;
}
/*
*	@brief	�v���C���[�̕`��
*	@details �v���C���[�N���X�̕`��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Player::Render()
{
	// �_���[�W�G�t�F�N�g���X�V����
	m_pWarningEffects->Render();
}
/*
*	@brief	�e�𐶐�����
*	@details �v���C���[�N���X�̒e�𐶐�����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Player::CreateBullet()
{
	// �e�𐶐�����
	m_pBulletManager->CreatePlayerBullet(GetPlayerController()->GetPlayerPosition(), m_playerDir);
}
/*
*	@brief	�v���C���[���_���[�W���󂯂����̏���
*	@details �_���[�W���󂯂����ɃJ������h�炷
*	@param	elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Player::PlayerDamage(float elapsedTime)
{
	// �v���C���[���_���[�W���󂯂���
	if (m_isDamage)
	{
		// �U����H�������_���[�W�G�t�F�N�g�𐶐�
		if (m_isPlayEffect == true)
		{
			// �_���[�W�G�t�F�N�g����
			m_pWarningEffects->CreateDamageEffects();
			// �_���[�W�G�t�F�N�g�𐶐�������false
			m_isPlayEffect = false;
		}
		// �_���[�W���󂯂����̃J�����̏���
		// sin�g���g���ď㉺�ɗh�炷
		float shakeOffset = sin(m_damageTime * DAMAGE_SHAKE_FREQUENCY) * DAMAGE_SHAKE_AMPLITUDE;
		// �J�����̒����_��ݒ�
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch() + shakeOffset);
		// �_���[�W���Ԃ����Z
		m_damageTime += elapsedTime;
		// �_���[�W���󂯂����Ԃ�0.25�b�𒴂�����
		if (m_damageTime >= DAMAGE_DURATION)
		{
			// �_���[�W�t���O��false�ɂ���
			m_isDamage = false;
			// �J�����̒����_��ݒ�
			m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
			// �_���[�W���Ԃ�0�ɂ���
			m_damageTime = 0.0f;
		}
	}
	// �J�����̒����_��ݒ�
	else m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
}
/*
*	@brief	�U�����悤�Ƃ��Ă���G�Ɋւ��鏈��
*	@details �U�����悤�Ƃ��Ă���G��������G�t�F�N�g�𐶐�����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Player::InComingEnemy()
{
	// ���ꂪture�Ȃ���s
	bool isPlayEffect = false;
	// �U�����悤�Ƃ��Ă���G�̐�
	int count = 0;
	// �U�����悤�Ƃ��Ă���G����������s
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		// �U�����Ă��Ȃ��G�̓X���[
		if (!enemy->GetIsAttack())continue;
		// �U�����Ă���G��������true
		isPlayEffect = true;
		// �U�����Ă���G�̐����J�E���g
		count++;
	}
	// �_���[�W�G�t�F�N�g����
	if (isPlayEffect)
	{
		// �U�����Ă���G�̐���ݒ�
		m_pWarningEffects->SetWarningEffectCount(count);
		// �_���[�W�G�t�F�N�g����
		m_pWarningEffects->CreateInComingEnemy();
	}
}