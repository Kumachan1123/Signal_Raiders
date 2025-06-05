/*
*	@file		Player.h
*	@brief		プレイヤークラス
*/
#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED
// DirectXTK
#include <DeviceResources.h>
#include <Mouse.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include "Game/CommonResources.h"
// 自作ヘッダーファイル
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/WarningEffect/WarningEffects/WarningEffects.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/FPS_Camera/FPS_Camera.h"

// 前方宣言
class CommonResources;
class PlayerController;
class PlayerUI;
class Goal;
class Reticle;
class PlayerHP;
class Crisis;
class WarningEffects;
class FPS_Camera;

// プレイヤークラス
class Player
{
public:
	// アクセサ
	// プレイヤーのHP
	float GetPlayerHP() const { return m_playerHP; }
	// プレイヤーのHP
	void SetPlayerHP(float playerHP) { m_playerHP = playerHP; }
	// プレイヤーの最大HP
	float GetMaxPlayerHP() const { return m_maxPlayerHP; }
	// プレイヤーの最大HP
	void SetMaxPlayerHP(float maxPlayerHP) { m_maxPlayerHP = maxPlayerHP; }
	// プレイヤーが走れる時間
	float GetDashTime() const { return m_dashTime; }
	// プレイヤーが走れる時間
	void SetDashTime(float dashTime) { m_dashTime = dashTime; }
	// カメラ
	FPS_Camera* GetCamera() { return m_pCamera.get(); }
	// 敵
	EnemyManager* GetEnemies() { return m_pEnemyManager; }
	// 弾マネージャー
	BulletManager* GetBulletManager() { return m_pBulletManager; }
	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 GetPlayerPos() const { return m_playerPos; }
	// プレイヤーの向き
	DirectX::SimpleMath::Vector3 GetPlayerDir() const { return m_playerDir; }
	// プレイヤーコントローラー
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }
	// プレイヤーの境界球
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }
	// プレイヤーと敵との一定範囲内での境界球
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }
	// 音量取得
	float GetVolume() const { return m_SEVolume; }
	// 音量取得
	void SetVolume(float volume) { m_SEVolume = volume; }
	// 音量を補正するときの値
	float GetVolumeCorrection() const { return m_VolumeCorrection; }
	// 音量を補正するときの値
	void SetVolumeCorrection(float volumeCorrection) { m_VolumeCorrection = volumeCorrection; }
	// マウス感度
	float GetMouseSensitive() const { return m_mouseSensitive; }
	// マウス感度
	void SetMouseSensitive(float sensitive) { m_mouseSensitive = sensitive; }
	// 攻撃してきた敵の向き
	DirectX::SimpleMath::Vector3 GetEnemyBulletDirection() const { return m_enemyDir; }
	// 攻撃してきた敵の向き
	void SetEnemyBulletDirection(const DirectX::SimpleMath::Vector3& enemyDir) { m_enemyDir = enemyDir; }
	// プレイヤーがダメージを受けたか
	bool GetisPlayerDamage() const { return m_isDamage; }
	// プレイヤーがダメージを受けたか
	void SetisPlayerDamage(bool isDamage) { m_isDamage = isDamage; }
	// エフェクト再生フラグ
	bool GetisPlayEffect() const { return m_isPlayEffect; }
	// エフェクト再生フラグ
	void SetisPlayEffect(bool isPlayEffect) { m_isPlayEffect = isPlayEffect; }
	// チートコマンド：敵を一掃するフラグ
	bool GetisKillAll() const { return m_isKillAll; }
	// チートコマンドが有効か
	bool GetisCheat() const { return m_isCheat; }
	// チートコマンドが有効か
	void SetisCheat(bool isCheat) { m_isCheat = isCheat; }
	// 弾マネージャーをセット
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }
public:
	// public関数
	// コンストラクタ
	Player(CommonResources* commonResources);
	// デストラクタ
	~Player();
	// 初期化
	void Initialize(EnemyManager* pEnemiyManager);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// 弾を生成する
	void CreateBullet();
private:
	// private関数
	// プレイヤーがダメージを受けた時の処理
	void PlayerDamage(float elapsedTime);
	// 攻撃しようとしている敵に関する処理
	void InComingEnemy();
private:
	// 定数
	// プレイヤーのHP初期値
	const float PLAYER_HP = 100.0f;
	// マウス感度初期値
	const float MOUSE_SENSITIVE = 0.1f;
	// プレイヤーと敵との一定範囲の境界球の半径
	const float IN_PLAYER_AREA_RADIUS = 20.0f;
	// プレイヤーの境界球の半径
	const float PLAYER_SPHERE_RADIUS = 3.0f;
	// ダメージ時のカメラ揺れに関する定数
	// カメラ揺れの周波数
	const float DAMAGE_SHAKE_FREQUENCY = 70.0f;
	// カメラ揺れの振幅
	const float DAMAGE_SHAKE_AMPLITUDE = 0.15f;
	// ダメージエフェクトの継続時間
	const float DAMAGE_DURATION = 0.25f;
private:
	// private変数
	// コモンリソース
	CommonResources* m_pCommonResources;
	// 敵
	EnemyManager* m_pEnemyManager;
	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPos;
	// プレイヤーの向き
	DirectX::SimpleMath::Vector3 m_playerDir;
	// カメラ
	std::unique_ptr<FPS_Camera> m_pCamera;
	// プレイヤーのHP
	float m_playerHP;
	// プレイヤーの最大HP
	float m_maxPlayerHP;
	// プレイヤーが走れる時間
	float m_dashTime;
	// ダメージエフェクトを管理するクラス
	std::unique_ptr<WarningEffects> m_pWarningEffects;
	// プレイヤーコントローラー
	std::unique_ptr<PlayerController> m_pPlayerController;
	// 弾マネージャー（プレイシーンから受け取る）
	BulletManager* m_pBulletManager;
	// プレイヤーと敵との一定範囲内での境界球
	DirectX::BoundingSphere m_inPlayerArea;
	// プレイヤーの境界球
	DirectX::BoundingSphere m_playerSphere;
	// 攻撃してきた敵の向き
	DirectX::SimpleMath::Vector3 m_enemyDir;
	// プレイヤーがダメージを食らった時
	bool m_isDamage;
	// エフェクト再生フラグ
	bool m_isPlayEffect;
	// プレイヤーがダメージを食らった時の時間
	float m_damageTime;
	// SEの音量
	float m_SEVolume;
	// 音量補正
	float m_VolumeCorrection;
	// マウス感度
	float m_mouseSensitive;
	// チートコマンド：敵を一掃するフラグ
	bool m_isKillAll;
	// チートコマンドが有効か
	bool m_isCheat;
	// 経過時間
	float m_timer;
};
#endif // PLAYER_DEFINED