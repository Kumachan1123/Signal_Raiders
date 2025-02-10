#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/DamageEffect/DamageEffects/DamageEffects.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/FPS_Camera/FPS_Camera.h"
// 前方宣言
class CommonResources;
class PlayerController;
class PlayerUI;
class PlayerBullets;
class Goal;
class Reticle;
class PlayerHP;
class Crisis;
class DamageEffects;
class FPS_Camera;
class Enemies;
class Player
{
private:
	// コモンリソース
	CommonResources* m_commonResources;
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
	std::unique_ptr<DamageEffects> m_pDamageEffects;
	// プレイヤーコントローラー
	std::unique_ptr<PlayerController> m_pPlayerController;
	// 弾マネージャー（プレイシーンから受け取る）
	BulletManager* m_pBulletManager;
	// 境界球
	DirectX::BoundingSphere m_inPlayerArea;// プレイヤーと敵との一定範囲内での境界球
	DirectX::BoundingSphere m_playerSphere;// プレイヤーの境界球
	// 攻撃してきた敵の向き
	DirectX::SimpleMath::Vector3 m_enemyDir;
	// プレイヤーがダメージを食らった時
	bool m_isDamage;
	bool m_isPlayEffect;// エフェクト再生フラグ
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
private:// 定数
	// プレイヤーのHP初期値
	const float PLAYER_HP = 100.0f;
	// マウス感度初期値
	const float MOUSE_SENSITIVE = 0.1f;
	// プレイヤーと敵との一定範囲の境界球の半径
	const float IN_PLAYER_AREA_RADIUS = 20.0f;
	// プレイヤーの境界球の半径
	const float PLAYER_SPHERE_RADIUS = 3.0f;
	// ダメージ時のカメラ揺れに関する定数
	const float DAMAGE_SHAKE_FREQUENCY = 70.0f;   // カメラ揺れの周波数
	const float DAMAGE_SHAKE_AMPLITUDE = 0.15f;  // カメラ揺れの振幅
	const float DAMAGE_DURATION = 0.25f;         // ダメージエフェクトの継続時間
public:
	Player(CommonResources* commonResources);
	~Player();

	// 初期化
	void Initialize(EnemyManager* pEnemies);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// Getter
public:
	float GetPlayerHP() const { return m_playerHP; }	// プレイヤーのHP
	void SetPlayerHP(float playerHP) { m_playerHP = playerHP; }	// プレイヤーのHP
	float GetMaxPlayerHP() const { return m_maxPlayerHP; }	// プレイヤーの最大HP
	void SetMaxPlayerHP(float maxPlayerHP) { m_maxPlayerHP = maxPlayerHP; }	// プレイヤーの最大HP
	float GetDashTime() const { return m_dashTime; }	// プレイヤーが走れる時間
	void SetDashTime(float dashTime) { m_dashTime = dashTime; }	// プレイヤーが走れる時間
	FPS_Camera* GetCamera() { return m_pCamera.get(); }// カメラ
	EnemyManager* GetEnemies() { return m_pEnemyManager; }// 敵
	BulletManager* GetBulletManager() { return m_pBulletManager; }// 弾マネージャー
	DirectX::SimpleMath::Vector3 GetPlayerPos() const { return m_playerPos; }// プレイヤーの位置
	DirectX::SimpleMath::Vector3 GetPlayerDir() const { return m_playerDir; }// プレイヤーの向き
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }// プレイヤーコントローラー
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }// プレイヤーの境界球
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }// プレイヤーと敵との一定範囲内での境界球
	float GetVolume() const { return m_SEVolume; }// 音量取得
	void SetVolume(float volume) { m_SEVolume = volume; }// 音量取得
	float GetVolumeCorrection() const { return m_VolumeCorrection; }// 音量を補正するときの値
	void SetVolumeCorrection(float volumeCorrection) { m_VolumeCorrection = volumeCorrection; }// 音量を補正するときの値
	float GetMouseSensitive() const { return m_mouseSensitive; }// マウス感度
	void SetMouseSensitive(float sensitive) { m_mouseSensitive = sensitive; }// マウス感度
	DirectX::SimpleMath::Vector3 GetEnemyBulletDirection() const { return m_enemyDir; }// 攻撃してきた敵の向き
	void SetEnemyBulletDirection(DirectX::SimpleMath::Vector3 enemyDir) { m_enemyDir = enemyDir; }// 攻撃してきた敵の向き
	bool GetisPlayerDamage() const { return m_isDamage; }	// プレイヤーがダメージを受けたか
	void SetisPlayerDamage(bool isDamage) { m_isDamage = isDamage; }// プレイヤーがダメージを受けたか
	bool GetisPlayEffect() const { return m_isPlayEffect; }// エフェクト再生フラグ
	void SetisPlayEffect(bool isPlayEffect) { m_isPlayEffect = isPlayEffect; }// エフェクト再生フラグ
	bool GetisKillAll() const { return m_isKillAll; }// チートコマンド：敵を一掃するフラグ
	bool GetisCheat() const { return m_isCheat; }// チートコマンドが有効か
	void SetisCheat(bool isCheat) { m_isCheat = isCheat; }// チートコマンドが有効か
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }// 弾マネージャーをセット
public:
	void CreateBullet();// 弾を生成する
private:
	void PlayerDamage(float elapsedTime);// プレイヤーがダメージを受けた時の処理

};