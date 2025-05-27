/*
*	@file	Player.cpp
*	@brief	プレイヤークラス
*/
#include "pch.h"
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
	m_inPlayerArea.Radius = IN_PLAYER_AREA_RADIUS;// プレイヤーと敵との一定範囲の境界球の半径
	m_playerSphere.Radius = PLAYER_SPHERE_RADIUS;// プレイヤーの境界球の半径
}
/*
*	@brief	デストラクタ
*	@details プレイヤークラスのデストラクタ
*	@param	なし
*	@return なし
*/
Player::~Player()
{
	m_pPlayerController.reset();// プレイヤーコントローラーの解放
	m_pCamera.reset();// カメラの解放
	m_pWarningEffects.reset();// ダメージエフェクトの解放
	m_pBulletManager = nullptr;// 弾マネージャーの解放
	m_pEnemyManager = nullptr;// 敵マネージャーの解放
}
/*
*	@brief	初期化
*	@details プレイヤークラスの初期化
*	@param	enemyManager	敵マネージャー
*	@return なし
*/
void Player::Initialize(EnemyManager* pEnemiyManager)
{
	m_playerHP = PLAYER_HP;// プレイヤーのHPを設定
	m_maxPlayerHP = m_playerHP;// プレイヤーの最大HPを設定
	m_pEnemyManager = pEnemiyManager;// 敵マネージャーのポインターを渡す
	m_pCamera = std::make_unique<FPS_Camera>();// FPSカメラを作成する
	m_pPlayerController = std::make_unique<PlayerController>(this);// プレイヤーのコントローラーを作成する
	m_pPlayerController->Initialize(m_pCommonResources);// プレイヤーコントローラーの初期化
	m_pPlayerController->SetPlayetPosition(m_pCamera->GetEyePosition());// プレイヤーの位置を設定
	m_pWarningEffects = std::make_unique<WarningEffects>(m_pCommonResources);// ダメージエフェクトを管理するクラスを作成する
	m_pWarningEffects->Initialize(this, m_pEnemyManager);// ダメージエフェクトの初期化
}
/*
*	@brief	プレイヤーの更新
*	@details プレイヤークラスの更新
*	@param	elapsedTime	経過時間
*	@return なし
*/
void Player::Update(float elapsedTime)
{
	m_timer += elapsedTime;// 経過時間を加算
	m_playerDir = m_pCamera->GetDirection();// カメラが向いている方向を取得する
	m_pCamera->Update(m_pPlayerController->GetPlayerPosition(), m_pPlayerController->GetYaw());// カメラ更新
	m_pPlayerController->Update(elapsedTime);// プレイヤーコントローラー更新
	InComingEnemy();// 敵が攻撃してきた時の処理
	PlayerDamage(elapsedTime);// ダメージを食らった時の処理
	m_playerPos = m_pPlayerController->GetPlayerPosition();// プレイヤーの位置を取得
	m_dashTime = m_pPlayerController->GetDashTime();// 走れる時間を更新
#ifdef _DEBUG// デバッグ チートコマンド
	m_pPlayerController->DebugCommand();// デバッグコマンドを実行
#endif
	m_pBulletManager->SetAdditionalDamage(m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());// Wi-Fiから得た追加ダメージをプレイヤーの弾に反映
	m_pPlayerController->Shoot();// 弾発射
	m_pPlayerController->Reload();// 弾のリロード
	m_pWarningEffects->Update(elapsedTime);// ダメージエフェクトを更新する
	// プレイヤーの境界球を更新
	m_inPlayerArea.Center = GetPlayerController()->GetPlayerPosition();// プレイヤーの位置を取得
	m_playerSphere.Center = m_inPlayerArea.Center;// プレイヤーの位置を取得
}
/*
*	@brief	プレイヤーの描画
*	@details プレイヤークラスの描画
*	@param	なし
*	@return なし
*/
void Player::Render()
{
	m_pWarningEffects->Render();// ダメージエフェクトを更新する
}
/*
*	@brief	弾を生成する
*	@details プレイヤークラスの弾を生成する
*	@param	なし
*	@return なし
*/
void Player::CreateBullet()
{
	m_pBulletManager->CreatePlayerBullet(GetPlayerController()->GetPlayerPosition(), m_playerDir);// 弾を生成する
}
/*
*	@brief	プレイヤーがダメージを受けた時の処理
*	@details ダメージを受けた時にカメラを揺らす
*	@param	elapsedTime	経過時間
*	@return なし
*/
void Player::PlayerDamage(float elapsedTime)
{
	if (m_isDamage)// プレイヤーがダメージを受けた時
	{
		if (m_isPlayEffect == true)// 攻撃を食らったらダメージエフェクトを生成
		{
			m_pWarningEffects->CreateDamageEffects();// ダメージエフェクト生成
			m_isPlayEffect = false;// ダメージエフェクトを生成したらfalse
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
/*
*	@brief	攻撃しようとしている敵に関する処理
*	@details 攻撃しようとしている敵がいたらエフェクトを生成する
*	@param	なし
*	@return なし
*/
void Player::InComingEnemy()
{
	bool isPlayEffect = false;// これがtureなら実行
	int count = 0;// 攻撃しようとしている敵の数
	for (auto& enemy : m_pEnemyManager->GetEnemies())// 攻撃しようとしている敵がいたら実行
	{
		if (!enemy->GetIsAttack())continue;// 攻撃していない敵はスルー
		isPlayEffect = true;// 攻撃している敵がいたらtrue
		count++;// 攻撃している敵の数をカウント
	}
	if (isPlayEffect)// ダメージエフェクト生成
	{
		m_pWarningEffects->SetWarningEffectCount(count);// 攻撃している敵の数を設定
		m_pWarningEffects->CreateInComingEnemy();// ダメージエフェクト生成
	}
}