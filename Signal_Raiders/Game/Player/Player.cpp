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
	m_pPlayerController{},
	m_inPlayerArea{},
	m_playerSphere{},
	m_pCamera{},
	m_pPlayerBullets{},
	m_pEnemyManager{ nullptr },
	m_mouseSensitive{ 0.1f },
	m_isDamage{ false },
	m_damageTime{ 0.0f },
	m_timer{ 0.0f },
	m_playerDir{},
	m_isKillAll{ false },
	m_isCheat{ false }
{
	// 境界球
	m_inPlayerArea.Radius = 20.0f;
	m_playerSphere.Radius = 2.0f;
}

Player::~Player()
{
}

void Player::Initialize(EnemyManager* pEnemies)
{
	// 敵
	m_pEnemyManager = pEnemies;
	// FPSカメラを作成する
	m_pCamera = std::make_unique<FPS_Camera>();

	// コントローラー生成
	m_pPlayerController = std::make_unique<PlayerController>(this);
	m_pPlayerController->Initialize(m_commonResources);
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
	// 弾
	m_pPlayerBullets = std::make_unique<PlayerBullets>(m_commonResources);
	m_pPlayerBullets->Initialize(this, m_pEnemyManager);
	// ダメージエフェクトを管理するクラス
	m_pDamageEffects = std::make_unique<DamageEffects>(m_commonResources);
	m_pDamageEffects->Initialize(this);
}

void Player::Update(float elapsedTime)
{
	m_timer += elapsedTime;
	// カメラが向いている方向を取得する
	m_playerDir = m_pCamera->GetDirection();
	// カメラ更新
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYawX());

	// プレイヤーコントローラー更新
	m_pPlayerController->Update(elapsedTime);

	// ダメージを食らった時の処理
	PlayerDamage(elapsedTime);
	// プレイヤーの位置を取得
	m_playerPos = m_pPlayerController->GetPlayerPosition();


#ifdef _DEBUG// デバッグ チートコマンド
	m_pPlayerController->DebugCommand();
#endif
	// プレイヤーの弾生成
	m_pPlayerController->Shoot();// 弾発射
	// 弾更新
	m_pPlayerBullets->Update(elapsedTime);
	// ダメージエフェクトを更新する
	m_pDamageEffects->Update(elapsedTime);
	// プレイヤーの境界球を更新
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// プレイヤーの位置を取得
	m_playerSphere.Center = m_inPlayerArea.Center;// プレイヤーの位置を取得

}

void Player::Render()
{
	m_pPlayerBullets->Render();// 弾描画


	// ダメージエフェクトを更新する
	m_pDamageEffects->Render();
}

void Player::CreateBullet()
{
	// 弾を生成する
	m_pPlayerBullets->CreateBullet(GetPlayerController()->GetPlayerPosition(), m_playerDir);
}

void Player::PlayerDamage(float elapsedTime)
{
	// プレイヤーがダメージを受けた時、カメラをsin波を使って上下に揺らす
	if (m_isDamage)
	{
		// 攻撃を食らったらダメージエフェクトを生成
		if (m_isPlayEffect == true)
		{
			// ダメージエフェクト生成
			m_pDamageEffects->Create(m_commonResources->GetDeviceResources());
			// ダメージエフェクトを生成したらfalse
			m_isPlayEffect = false;
		}
		// ダメージを受けた時のカメラの処理
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch() + sin(m_damageTime * 70.0f) * 0.15f);// カメラの注視点を設定
		m_damageTime += elapsedTime;// ダメージ時間を加算
		if (m_damageTime >= 0.25f)// ダメージを受けた時間が0.25秒を超えたら
		{
			m_isDamage = false;// ダメージフラグをfalseにする
			m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());// カメラの注視点を設定
			m_damageTime = 0.0f;// ダメージ時間を0にする
		}
	}
	else m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());// カメラの注視点を設定
}
