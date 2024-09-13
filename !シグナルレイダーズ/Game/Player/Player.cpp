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
	// 境界球
	m_inPlayerArea.Radius = 20.0f;
	m_playerSphere.Radius = 2.0f;
}

Player::~Player()
{
}

void Player::Initialize()
{
	auto DR = m_commonResources->GetDeviceResources();
	// FPSカメラを作成する
	m_pCamera = std::make_unique<FPS_Camera>();
	// HPゲージ作成
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, 1280, 720);
	// 照準作成
	m_pPlayerPointer = std::make_unique<PlayerPointer>();
	m_pPlayerPointer->Initialize(DR, 1280, 720);
	// コントローラー生成
	m_pPlayerController = std::make_unique<PlayerController>();
	m_pPlayerController->Initialize(m_commonResources);
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
}

void Player::Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb, float elapsedTime)
{
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pCamera->GetDirection();
	// プレイヤーコントローラー更新
	m_pPlayerController->Update(kb, cameraDirection, elapsedTime);
	// カメラ更新
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYawX());
	m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
	// HP更新
	m_pPlayerHP->Update(m_playerHP);
	// 照準更新
	m_pPlayerPointer->Update();
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// プレイヤーの位置を取得
	m_playerSphere.Center = m_inPlayerArea.Center;// プレイヤーの位置を取得
}

void Player::Render()
{
	m_pPlayerHP->Render();// HP描画
	m_pPlayerPointer->Render();// 照準描画

}
