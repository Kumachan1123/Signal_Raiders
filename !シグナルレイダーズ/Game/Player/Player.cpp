#include "pch.h"
#include "Player.h"

Player::Player(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_playerHP{ 100.0f },
	m_pPlayerHP{},
	m_pPlayerPointer{},
	m_pPlayerController{},
	m_inPlayerArea{},
	m_playerSphere{},
	m_pCamera{}
{
	// ���E��
	m_inPlayerArea.Radius = 20.0f;
	m_playerSphere.Radius = 2.0f;
}

Player::~Player()
{
}

void Player::Initialize()
{
	auto DR = m_commonResources->GetDeviceResources();
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
}

void Player::Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb, float elapsedTime)
{
	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_pCamera->GetDirection();
	// �v���C���[�R���g���[���[�X�V
	m_pPlayerController->Update(kb, cameraDirection, elapsedTime);
	// �J�����X�V
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYawX());
	m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
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

}
