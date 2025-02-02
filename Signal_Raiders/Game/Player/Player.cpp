#include "pch.h"
#include "Player.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <Mouse.h>
#include "Libraries/MyLib/InputManager.h"
Player::Player(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_playerHP{  },
	m_pPlayerController{},
	m_inPlayerArea{},
	m_playerSphere{},
	m_pCamera{},
	m_pEnemyManager{ nullptr },
	m_pBulletManager{ nullptr },
	m_mouseSensitive{  },
	m_isDamage{ false },
	m_isPlayEffect{ false },
	m_damageTime{ 0.0f },
	m_timer{ 0.0f },
	m_SEVolume{ 0.0f },
	m_VolumeCorrection{ 0.0f },
	m_playerDir{},
	m_isKillAll{ false },
	m_isCheat{ false }
{
	// ���E��
	m_inPlayerArea.Radius = IN_PLAYER_AREA_RADIUS;
	m_playerSphere.Radius = PLAYER_SPHERE_RADIUS;
}

Player::~Player()
{
}

void Player::Initialize(EnemyManager* pEnemies)
{
	// �v���C���[��HP��ݒ�
	m_playerHP = PLAYER_HP;
	// �G
	m_pEnemyManager = pEnemies;
	// FPS�J�������쐬����
	m_pCamera = std::make_unique<FPS_Camera>();
	// �R���g���[���[����
	m_pPlayerController = std::make_unique<PlayerController>(this);
	m_pPlayerController->Initialize(m_commonResources);
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
	// �_���[�W�G�t�F�N�g���Ǘ�����N���X
	m_pDamageEffects = std::make_unique<DamageEffects>(m_commonResources);
	m_pDamageEffects->Initialize(this);

}

void Player::Update(float elapsedTime)
{
	m_timer += elapsedTime;
	// �J�����������Ă���������擾����
	m_playerDir = m_pCamera->GetDirection();
	// �J�����X�V
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYawX());

	// �v���C���[�R���g���[���[�X�V
	m_pPlayerController->Update(elapsedTime);

	// �_���[�W��H��������̏���
	PlayerDamage(elapsedTime);
	// �v���C���[�̈ʒu���擾
	m_playerPos = m_pPlayerController->GetPlayerPosition();


#ifdef _DEBUG// �f�o�b�O �`�[�g�R�}���h
	m_pPlayerController->DebugCommand();
#endif
	// �v���C���[�̒e����
	m_pPlayerController->Shoot();// �e����
	// �_���[�W�G�t�F�N�g���X�V����
	m_pDamageEffects->Update(elapsedTime);
	// �v���C���[�̋��E�����X�V
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// �v���C���[�̈ʒu���擾
	m_playerSphere.Center = m_inPlayerArea.Center;// �v���C���[�̈ʒu���擾

}

void Player::Render()
{
	// �_���[�W�G�t�F�N�g���X�V����
	m_pDamageEffects->Render();
}

void Player::CreateBullet()
{
	// �e�𐶐�����
	m_pBulletManager->CreatePlayerBullet(GetPlayerController()->GetPlayerPosition(), m_playerDir);

}

void Player::PlayerDamage(float elapsedTime)
{
	// �v���C���[���_���[�W���󂯂����A�J������sin�g���g���ď㉺�ɗh�炷
	if (m_isDamage)
	{
		// �U����H�������_���[�W�G�t�F�N�g�𐶐�
		if (m_isPlayEffect == true)
		{
			// �_���[�W�G�t�F�N�g����
			m_pDamageEffects->Create(m_commonResources->GetDeviceResources());
			// �_���[�W�G�t�F�N�g�𐶐�������false
			m_isPlayEffect = false;
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
