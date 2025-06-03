/*
*	@file	Player.cpp
*	@brief	プレイヤークラス
*/
#include <pch.h>
#include "Player.h"
/*
*	@brief	コンストラクタ
*	@details プレイヤークラスのコンストラクタ
*	@param	commonResources	共通リソース
*	@return なし
*/
Player::Player(CommonResources* commonResources)
	: m_pCommonResources{ commonResources }// 共通リソース
	, m_playerHP{  }// プレイヤーのHP
	, m_maxPlayerHP{  }// プレイヤーの最大HP
	, m_pPlayerController{}// プレイヤーコントローラー
	, m_inPlayerArea{}// プレイヤーと敵との一定範囲内での境界球
	, m_playerSphere{}// プレイヤーの境界球
	, m_pCamera{ nullptr }// カメラ
	, m_pEnemyManager{ nullptr }// 敵マネージャー
	, m_pBulletManager{ nullptr }// 弾マネージャー
	, m_mouseSensitive{  }// マウス感度
	, m_isDamage{ false }// ダメージを受けたか
	, m_isPlayEffect{ false }// エフェクト再生フラグ
	, m_damageTime{ 0.0f }// ダメージを受けた時間
	, m_timer{ 0.0f }// 経過時間
	, m_SEVolume{ 0.0f }// SEの音量
	, m_VolumeCorrection{ 0.0f }// 音量補正
	, m_playerDir{}// プレイヤーの向き
	, m_dashTime{ 0.0f }// プレイヤーが走れる時間
	, m_isKillAll{ false }// チートコマンド：敵を一掃するフラグ
	, m_isCheat{ false }// チートコマンドが有効か
{
	// プレイヤーと敵との一定範囲の境界球の半径
	m_inPlayerArea.Radius = IN_PLAYER_AREA_RADIUS;
	// プレイヤーの境界球の半径
	m_playerSphere.Radius = PLAYER_SPHERE_RADIUS;
}
/*
*	@brief	デストラクタ
*	@details プレイヤークラスのデストラクタ
*	@param	なし
*	@return なし
*/
Player::~Player()
{
	// プレイヤーコントローラーの解放
	m_pPlayerController.reset();
	// カメラの解放
	m_pCamera.reset();
	// ダメージエフェクトの解放
	m_pWarningEffects.reset();
	// 弾マネージャーの解放
	m_pBulletManager = nullptr;
	// 敵マネージャーの解放
	m_pEnemyManager = nullptr;
}
/*
*	@brief	初期化
*	@details プレイヤークラスの初期化
*	@param	enemyManager	敵マネージャー
*	@return なし
*/
void Player::Initialize(EnemyManager* pEnemiyManager)
{
	// プレイヤーのHPを設定
	m_playerHP = PLAYER_HP;
	// プレイヤーの最大HPを設定
	m_maxPlayerHP = m_playerHP;
	// 敵マネージャーのポインターを渡す
	m_pEnemyManager = pEnemiyManager;
	// FPSカメラを作成する
	m_pCamera = std::make_unique<FPS_Camera>();
	// プレイヤーのコントローラーを作成する
	m_pPlayerController = std::make_unique<PlayerController>(this);
	// プレイヤーコントローラーの初期化
	m_pPlayerController->Initialize(m_pCommonResources);
	// プレイヤーの位置を設定
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());
	// ダメージエフェクトを管理するクラスを作成する
	m_pWarningEffects = std::make_unique<WarningEffects>(m_pCommonResources);
	// ダメージエフェクトの初期化
	m_pWarningEffects->Initialize(this, m_pEnemyManager);
}
/*
*	@brief	プレイヤーの更新
*	@details プレイヤークラスの更新
*	@param	elapsedTime	経過時間
*	@return なし
*/
void Player::Update(float elapsedTime)
{
	// 経過時間を加算
	m_timer += elapsedTime;
	// カメラが向いている方向を取得する
	m_playerDir = m_pCamera->GetDirection();
	// カメラ更新
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYaw());
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
	// デバッグコマンドを実行
	m_pPlayerController->DebugCommand();
#endif
	// Wi-Fiから得た追加ダメージをプレイヤーの弾に反映
	m_pBulletManager->SetAdditionalDamage(m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());
	// 弾を発射する
	m_pPlayerController->Shoot();
	// 弾のリロードする
	m_pPlayerController->Reload();
	// ダメージエフェクトを更新する
	m_pWarningEffects->Update(elapsedTime);
	// プレイヤーの境界球を更新
	// プレイヤーの位置を取得し、境界球の中心を設定
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();
	m_playerSphere.Center = m_inPlayerArea.Center;
}
/*
*	@brief	プレイヤーの描画
*	@details プレイヤークラスの描画
*	@param	なし
*	@return なし
*/
void Player::Render()
{
	// ダメージエフェクトを更新する
	m_pWarningEffects->Render();
}
/*
*	@brief	弾を生成する
*	@details プレイヤークラスの弾を生成する
*	@param	なし
*	@return なし
*/
void Player::CreateBullet()
{
	// 弾を生成する
	m_pBulletManager->CreatePlayerBullet(GetPlayerController()->GetPlayerPosition(), m_playerDir);
}
/*
*	@brief	プレイヤーがダメージを受けた時の処理
*	@details ダメージを受けた時にカメラを揺らす
*	@param	elapsedTime	経過時間
*	@return なし
*/
void Player::PlayerDamage(float elapsedTime)
{
	// プレイヤーがダメージを受けた時
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
		// sin波を使って上下に揺らす
		float shakeOffset = sin(m_damageTime * DAMAGE_SHAKE_FREQUENCY) * DAMAGE_SHAKE_AMPLITUDE;
		// カメラの注視点を設定
		m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch() + shakeOffset);
		// ダメージ時間を加算
		m_damageTime += elapsedTime;
		// ダメージを受けた時間が0.25秒を超えたら
		if (m_damageTime >= DAMAGE_DURATION)
		{
			// ダメージフラグをfalseにする
			m_isDamage = false;
			// カメラの注視点を設定
			m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
			// ダメージ時間を0にする
			m_damageTime = 0.0f;
		}
	}
	// カメラの注視点を設定
	else m_pCamera->SetTargetPositionY(m_pPlayerController->GetPitch());
}
/*
*	@brief	攻撃しようとしている敵に関する処理
*	@details 攻撃しようとしている敵がいたらエフェクトを生成する
*	@param	なし
*	@return なし
*/
void Player::InComingEnemy()
{
	// これがtureなら実行
	bool isPlayEffect = false;
	// 攻撃しようとしている敵の数
	int count = 0;
	// 攻撃しようとしている敵がいたら実行
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		// 攻撃していない敵はスルー
		if (!enemy->GetIsAttack())continue;
		// 攻撃している敵がいたらtrue
		isPlayEffect = true;
		// 攻撃している敵の数をカウント
		count++;
	}
	// ダメージエフェクト生成
	if (isPlayEffect)
	{
		// 攻撃している敵の数を設定
		m_pWarningEffects->SetWarningEffectCount(count);
		// ダメージエフェクト生成
		m_pWarningEffects->CreateInComingEnemy();
	}
}