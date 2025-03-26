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
	m_maxPlayerHP{  },
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
	m_dashTime{ 0.0f },
	m_isKillAll{ false },
	m_isCheat{ false }
{
	// 境界球
	m_inPlayerArea.Radius = IN_PLAYER_AREA_RADIUS;
	m_playerSphere.Radius = PLAYER_SPHERE_RADIUS;
}

Player::~Player()
{
}

void Player::Initialize(EnemyManager* pEnemies)
{
	// プレイヤーのHPを設定
	m_playerHP = PLAYER_HP;
	m_maxPlayerHP = m_playerHP;
	// 敵
	m_pEnemyManager = pEnemies;
	// FPSカメラを作成する
	m_pCamera = std::make_unique<FPS_Camera>();
	// コントローラー生成
	m_pPlayerController = std::make_unique<PlayerController>(this);
	m_pPlayerController->Initialize(m_commonResources);
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
	// ダメージエフェクトを管理するクラス
	m_pWarningEffects = std::make_unique<WarningEffects>(m_commonResources);
	m_pWarningEffects->Initialize(this, m_pEnemyManager);

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

	// 敵が攻撃してきた時の処理
	InComingEnemy();
	// ダメージを食らった時の処理
	PlayerDamage(elapsedTime);
	// プレイヤーの位置を取得
	m_playerPos = m_pPlayerController->GetPlayerPosition();
	// 走れる時間を更新
	m_dashTime = m_pPlayerController->GetDashTime();

#ifdef _DEBUG// デバッグ チートコマンド
	m_pPlayerController->DebugCommand();
#endif
	// Wi-Fiから得た追加ダメージをプレイヤーの弾に反映
	m_pBulletManager->SetAdditionalDamage(m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());
	// 弾発射
	m_pPlayerController->Shoot();
	// 弾のリロード
	m_pPlayerController->Reload();
	// ダメージエフェクトを更新する
	m_pWarningEffects->Update(elapsedTime);
	// プレイヤーの境界球を更新
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// プレイヤーの位置を取得
	m_playerSphere.Center = m_inPlayerArea.Center;// プレイヤーの位置を取得

}

void Player::Render()
{
	// ダメージエフェクトを更新する
	m_pWarningEffects->Render();
}

void Player::CreateBullet()
{
	// 弾を生成する
	m_pBulletManager->CreatePlayerBullet(GetPlayerController()->GetPlayerPosition(), m_playerDir);

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
			m_pWarningEffects->CreateDamageEffects();
			// ダメージエフェクトを生成したらfalse
			m_isPlayEffect = false;
		}
		// ダメージを受けた時のカメラの処理
		float shakeOffset = sin(m_damageTime * DAMAGE_SHAKE_FREQUENCY) * DAMAGE_SHAKE_AMPLITUDE;// sin波を使って上下に揺らす
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch() + shakeOffset);// カメラの注視点を設定
		m_damageTime += elapsedTime;// ダメージ時間を加算
		if (m_damageTime >= DAMAGE_DURATION)// ダメージを受けた時間が0.25秒を超えたら
		{
			m_isDamage = false;// ダメージフラグをfalseにする
			m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());// カメラの注視点を設定
			m_damageTime = 0.0f;// ダメージ時間を0にする
		}
	}
	else m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());// カメラの注視点を設定
}

void Player::InComingEnemy()
{
	// これがtureなら実行
	bool isPlayEffect = false;
	// 攻撃しようとしている敵の数
	int count = 0;
	// 攻撃しようとしている敵がいたら実行
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		if (enemy->GetIsAttack() == true)
		{
			isPlayEffect = true;
			count++;
		}
	}
	// ダメージエフェクト生成
	if (isPlayEffect)
	{
		m_pWarningEffects->SetWarningEffectCount(count);
		m_pWarningEffects->CreateInComingEnemy();
	}

}
