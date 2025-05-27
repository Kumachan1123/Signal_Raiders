/*
*	@file	Player.cpp
*	@brief	�v���C���[�N���X
*/
#include "pch.h"
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
	m_inPlayerArea.Radius = IN_PLAYER_AREA_RADIUS;// �v���C���[�ƓG�Ƃ̈��͈͂̋��E���̔��a
	m_playerSphere.Radius = PLAYER_SPHERE_RADIUS;// �v���C���[�̋��E���̔��a
}
/*
*	@brief	�f�X�g���N�^
*	@details �v���C���[�N���X�̃f�X�g���N�^
*	@param	�Ȃ�
*	@return �Ȃ�
*/
Player::~Player()
{
	m_pPlayerController.reset();// �v���C���[�R���g���[���[�̉��
	m_pCamera.reset();// �J�����̉��
	m_pWarningEffects.reset();// �_���[�W�G�t�F�N�g�̉��
	m_pBulletManager = nullptr;// �e�}�l�[�W���[�̉��
	m_pEnemyManager = nullptr;// �G�}�l�[�W���[�̉��
}
/*
*	@brief	������
*	@details �v���C���[�N���X�̏�����
*	@param	enemyManager	�G�}�l�[�W���[
*	@return �Ȃ�
*/
void Player::Initialize(EnemyManager* pEnemiyManager)
{
	m_playerHP = PLAYER_HP;// �v���C���[��HP��ݒ�
	m_maxPlayerHP = m_playerHP;// �v���C���[�̍ő�HP��ݒ�
	m_pEnemyManager = pEnemiyManager;// �G�}�l�[�W���[�̃|�C���^�[��n��
	m_pCamera = std::make_unique<FPS_Camera>();// FPS�J�������쐬����
	m_pPlayerController = std::make_unique<PlayerController>(this);// �v���C���[�̃R���g���[���[���쐬����
	m_pPlayerController->Initialize(m_pCommonResources);// �v���C���[�R���g���[���[�̏�����
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());// �v���C���[�̈ʒu��ݒ�
	m_pWarningEffects = std::make_unique<WarningEffects>(m_pCommonResources);// �_���[�W�G�t�F�N�g���Ǘ�����N���X���쐬����
	m_pWarningEffects->Initialize(this, m_pEnemyManager);// �_���[�W�G�t�F�N�g�̏�����
}
/*
*	@brief	�v���C���[�̍X�V
*	@details �v���C���[�N���X�̍X�V
*	@param	elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Player::Update(float elapsedTime)
{
	m_timer += elapsedTime;// �o�ߎ��Ԃ����Z
	m_playerDir = m_pCamera->GetDirection();// �J�����������Ă���������擾����
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYaw());// �J�����X�V
	m_pPlayerController->Update(elapsedTime);// �v���C���[�R���g���[���[�X�V
	InComingEnemy();// �G���U�����Ă������̏���
	PlayerDamage(elapsedTime);// �_���[�W��H��������̏���
	m_playerPos = m_pPlayerController->GetPlayerPosition();// �v���C���[�̈ʒu���擾
	m_dashTime = m_pPlayerController->GetDashTime();// ����鎞�Ԃ��X�V
#ifdef _DEBUG// �f�o�b�O �`�[�g�R�}���h
	m_pPlayerController->DebugCommand();// �f�o�b�O�R�}���h�����s
#endif
	m_pBulletManager->SetAdditionalDamage(m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());// Wi-Fi���瓾���ǉ��_���[�W���v���C���[�̒e�ɔ��f
	m_pPlayerController->Shoot();// �e����
	m_pPlayerController->Reload();// �e�̃����[�h
	m_pWarningEffects->Update(elapsedTime);// �_���[�W�G�t�F�N�g���X�V����
	// �v���C���[�̋��E�����X�V
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// �v���C���[�̈ʒu���擾
	m_playerSphere.Center = m_inPlayerArea.Center;// �v���C���[�̈ʒu���擾
}
/*
*	@brief	�v���C���[�̕`��
*	@details �v���C���[�N���X�̕`��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Player::Render()
{
	m_pWarningEffects->Render();// �_���[�W�G�t�F�N�g���X�V����
}
/*
*	@brief	�e�𐶐�����
*	@details �v���C���[�N���X�̒e�𐶐�����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Player::CreateBullet()
{
	m_pBulletManager->CreatePlayerBullet(GetPlayerController()->GetPlayerPosition(), m_playerDir);// �e�𐶐�����
}
/*
*	@brief	�v���C���[���_���[�W���󂯂����̏���
*	@details �_���[�W���󂯂����ɃJ������h�炷
*	@param	elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Player::PlayerDamage(float elapsedTime)
{
	if (m_isDamage)// �v���C���[���_���[�W���󂯂���
	{
		if (m_isPlayEffect == true)// �U����H�������_���[�W�G�t�F�N�g�𐶐�
		{
			m_pWarningEffects->CreateDamageEffects();// �_���[�W�G�t�F�N�g����
			m_isPlayEffect = false;// �_���[�W�G�t�F�N�g�𐶐�������false
		}
		// �_���[�W���󂯂����̃J�����̏���
		float shakeOffset = sin(m_damageTime * DAMAGE_SHAKE_FREQUENCY) * DAMAGE_SHAKE_AMPLITUDE;// sin�g���g���ď㉺�ɗh�炷
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch() + shakeOffset);// �J�����̒����_��ݒ�
		m_damageTime += elapsedTime;// �_���[�W���Ԃ����Z
		if (m_damageTime >= DAMAGE_DURATION)// �_���[�W���󂯂����Ԃ�0.25�b�𒴂�����
		{
			m_isDamage = false;// �_���[�W�t���O��false�ɂ���
			m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());// �J�����̒����_��ݒ�
			m_damageTime = 0.0f;// �_���[�W���Ԃ�0�ɂ���
		}
	}
	else m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());// �J�����̒����_��ݒ�
}
/*
*	@brief	�U�����悤�Ƃ��Ă���G�Ɋւ��鏈��
*	@details �U�����悤�Ƃ��Ă���G��������G�t�F�N�g�𐶐�����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Player::InComingEnemy()
{
	bool isPlayEffect = false;// ���ꂪture�Ȃ���s
	int count = 0;// �U�����悤�Ƃ��Ă���G�̐�
	for (auto& enemy : m_pEnemyManager->GetEnemies())// �U�����悤�Ƃ��Ă���G����������s
	{
		if (!enemy->GetIsAttack())continue;// �U�����Ă��Ȃ��G�̓X���[
		isPlayEffect = true;// �U�����Ă���G��������true
		count++;// �U�����Ă���G�̐����J�E���g
	}
	if (isPlayEffect)// �_���[�W�G�t�F�N�g����
	{
		m_pWarningEffects->SetWarningEffectCount(count);// �U�����Ă���G�̐���ݒ�
		m_pWarningEffects->CreateInComingEnemy();// �_���[�W�G�t�F�N�g����
	}
}