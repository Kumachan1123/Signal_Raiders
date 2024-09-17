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
	// 境界球
	m_inPlayerArea.Radius = 20.0f;
	m_playerSphere.Radius = 2.0f;
}

Player::~Player()
{
}

void Player::Initialize(Enemies* pEnemies)
{
	auto DR = m_commonResources->GetDeviceResources();
	// 敵
	m_pEnemies = pEnemies;
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
	// 弾
	m_pPlayerBullets = std::make_unique<PlayerBullets>(m_commonResources);
	m_pPlayerBullets->Initialize(this, m_pEnemies);
}

void Player::Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb, float elapsedTime)
{

	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pCamera->GetDirection();
#ifdef _DEBUG// デバッグ
	// 右クリックで敵を一掃
	if (mtracker->GetLastState().rightButton)for (auto& enemy : m_pEnemies->GetEnemy())enemy->SetEnemyHP(0);
	// スペースキーでプレイヤーのHPを0にする
	if (kb->pressed.Space)SetPlayerHP(0.0f);
#endif
	// 左クリックで弾発射
	if (mtracker->GetLastState().leftButton && m_pPlayerBullets->GetIsBullet() == false)
	{
		// 弾を生成する
		m_pPlayerBullets->CreateBullet(GetPlayerController()->GetPlayerPosition(), cameraDirection);
	}
	if (!mtracker->GetLastState().leftButton)m_pPlayerBullets->SetIsBullet(false);

	// プレイヤーコントローラー更新
	m_pPlayerController->Update(kb, cameraDirection, elapsedTime);
	// カメラ更新
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYawX());

	// プレイヤーがダメージを受けた時、カメラをsin波を使って上下に揺らす
	if (m_isDamage)
	{
		// ダメージを受けた時の処理
		// カメラを上下に揺らす
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
	// 弾更新
	m_pPlayerBullets->Update(elapsedTime);
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
	m_pPlayerBullets->Render();// 弾描画

}
