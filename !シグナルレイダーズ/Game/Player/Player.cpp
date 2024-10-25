#include "pch.h"
#include "Player.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <Mouse.h>
#include "Libraries/MyLib/InputManager.h"
Player::Player(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_playerHP{ 100.0f },
	m_pPlayerHP{},
	m_pPlayerPointer{},
	m_pPlayerController{},
	m_inPlayerArea{},
	m_playerSphere{},
	m_pCamera{},
	m_pPlayerBullets{},
	m_pEnemies{ nullptr },
	m_mouseSensitive{ 0.1f },
	m_isDamage{ false },
	m_damageTime{ 0.0f },
	m_playerDir{}
{
	// ���E��
	m_inPlayerArea.Radius = 20.0f;
	m_playerSphere.Radius = 2.0f;
}

Player::~Player()
{
}

void Player::Initialize(Enemies* pEnemies)
{
	auto DR = m_commonResources->GetDeviceResources();
	// �G
	m_pEnemies = pEnemies;
	// FPS�J�������쐬����
	m_pCamera = std::make_unique<FPS_Camera>();
	// HP�Q�[�W�쐬
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, 1280, 720);
	// �Ə��쐬
	m_pPlayerPointer = std::make_unique<PlayerPointer>();
	m_pPlayerPointer->Initialize(DR, 1280, 720);
	// �R���g���[���[����
	m_pPlayerController = std::make_unique<PlayerController>(this);
	m_pPlayerController->Initialize(m_commonResources);
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
	// �e
	m_pPlayerBullets = std::make_unique<PlayerBullets>(m_commonResources);
	m_pPlayerBullets->Initialize(this, m_pEnemies);
	// �댯���
	m_pCrisis = std::make_unique<Crisis>(m_commonResources);
	m_pCrisis->Create(DR);
}

void Player::Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb, float elapsedTime)
{
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_pCamera->GetDirection();
#ifdef _DEBUG// �f�o�b�O
	// �E�N���b�N�œG����|
	if (mtracker->GetLastState().rightButton)for (auto& enemy : m_pEnemies->GetEnemies())enemy->SetEnemyHP(0);
	// �X�y�[�X�L�[�Ńv���C���[��HP��0�ɂ���
	if (kb->pressed.Space)SetPlayerHP(0.0f);
#endif
	// ���N���b�N�Œe����
	if (mtracker->GetLastState().leftButton && m_pPlayerBullets->GetIsBullet() == false)
	{
		// �e�𐶐�����
		m_pPlayerBullets->CreateBullet(GetPlayerController()->GetPlayerPosition(), cameraDirection);
	}
	if (!mtracker->GetLastState().leftButton)m_pPlayerBullets->SetIsBullet(false);
	// �v���C���[�R���g���[���[�X�V
	m_pPlayerController->Update(kb, cameraDirection, elapsedTime);
	// �v���C���[�̈ʒu���擾
	m_playerPos = m_pPlayerController->GetPlayerPosition();

	// �J�����X�V
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYawX());
	// �v���C���[�̉�]���擾
	m_playerDir = cameraDirection;
	// �v���C���[���_���[�W���󂯂����A�J������sin�g���g���ď㉺�ɗh�炷
	if (m_isDamage)
	{
		// �U����H�������_���[�W�G�t�F�N�g�𐶐�
		if (m_isPlayEffect == true)
		{
			// �_���[�W�G�t�F�N�g����
			auto damageEffect = std::make_unique<DamageEffect>(m_commonResources);
			damageEffect->Initialize(this);
			damageEffect->Create(m_commonResources->GetDeviceResources());
			damageEffect->SetEnemyDirection(GetEnemyDir());// �_���[�W���󂯂��G�̌�����ݒ�
			m_pDamageEffect.push_back(std::move(damageEffect));
			m_isPlayEffect = false;// �_���[�W�G�t�F�N�g�𐶐�������false
		}
		// �_���[�W���󂯂����̏���
		// �J�������㉺�ɗh�炷
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch() + sin(m_damageTime * 70.0f) * 0.15f);
		m_damageTime += elapsedTime;
		if (m_damageTime >= 0.25f)
		{
			m_isDamage = false;
			m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
			m_damageTime = 0.0f;
		}
	}
	else m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());// �J�����̒����_��ݒ�
	// �e�X�V
	m_pPlayerBullets->Update(elapsedTime);
	// HP�X�V
	m_pPlayerHP->Update(m_playerHP);
	// �̗͂�10�ȉ��ɂȂ������@��ԍX�V
	if (m_playerHP <= 10.0f)m_pCrisis->Update(elapsedTime);
	// �Ə��X�V
	m_pPlayerPointer->Update();
	// �_���[�W�G�t�F�N�g���X�V����
	std::vector<std::unique_ptr<DamageEffect>> newDE;
	for (auto& damageEffect : m_pDamageEffect)
	{
		// �X�V����
		damageEffect->Update(elapsedTime);
		// �Đ����I������_���[�W�G�t�F�N�g�������玟�̃G�t�F�N�g��
		if (damageEffect->Destroy())continue;
		// �Đ����I�����Ă��Ȃ��G�t�F�N�g�͐V�������X�g�Ɉړ�
		newDE.push_back(std::move(damageEffect));
	}
	// �_���[�W�G�t�F�N�g��V�������X�g�ɒu��������
	m_pDamageEffect = std::move(newDE);
	// �v���C���[�̋��E�����X�V
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// �v���C���[�̈ʒu���擾
	m_playerSphere.Center = m_inPlayerArea.Center;// �v���C���[�̈ʒu���擾
}

void Player::Render()
{
	if (m_playerHP <= 10.0f)m_pCrisis->Render();// HP��10�ȉ��Ŋ�@��ԕ`��
	m_pPlayerHP->Render();// HP�`��
	m_pPlayerPointer->Render();// �Ə��`��
	m_pPlayerBullets->Render();// �e�`��
	// �_���[�W�G�t�F�N�g���X�V����
	for (auto& damageEffect : m_pDamageEffect)damageEffect->Render();

}
