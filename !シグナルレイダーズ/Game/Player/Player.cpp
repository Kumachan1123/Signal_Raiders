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
	m_pEnemies{ nullptr }
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
	m_pPlayerController = std::make_unique<PlayerController>();
	m_pPlayerController->Initialize(m_commonResources);
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
	// �e
	m_pPlayerBullets = std::make_unique<PlayerBullets>(m_commonResources);
	m_pPlayerBullets->Initialize(this, m_pEnemies);
}

void Player::Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb, float elapsedTime)
{

	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_pCamera->GetDirection();
#ifdef _DEBUG// �f�o�b�O
	// �E�N���b�N�œG����|
	if (mtracker->GetLastState().rightButton)for (auto& enemy : m_pEnemies->GetEnemy())enemy->SetEnemyHP(0);
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
	// �J�����X�V
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYawX());

	// �v���C���[���_���[�W���󂯂����A�J������sin�g���g���ď㉺�ɗh�炷
	if (m_isDamage)
	{
		// �_���[�W���󂯂����̏���
		// �J�������㉺�ɗh�炷
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch() + sin(m_damageTime * 100.0f) * 0.1f);
		m_damageTime += elapsedTime;
		if (m_damageTime >= 0.5f)
		{
			m_isDamage = false;
			m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
			m_damageTime = 0.0f;
		}


	}
	else
	{
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
	}
	// �e�X�V
	m_pPlayerBullets->Update(elapsedTime);
	// HP�X�V
	m_pPlayerHP->Update(m_playerHP);
	// �Ə��X�V
	m_pPlayerPointer->Update();
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// �v���C���[�̈ʒu���擾
	m_playerSphere.Center = m_inPlayerArea.Center;// �v���C���[�̈ʒu���擾

}

void Player::Render()
{
	m_pPlayerHP->Render();// HP�`��
	m_pPlayerPointer->Render();// �Ə��`��
	m_pPlayerBullets->Render();// �e�`��

}
