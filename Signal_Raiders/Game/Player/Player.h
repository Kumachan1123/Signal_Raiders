#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullets/PlayerBullets.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"

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
	// プレイヤーの弾
	std::unique_ptr<PlayerBullets> m_pPlayerBullets;

	// ダメージエフェクトを管理するクラス
	std::unique_ptr<DamageEffects> m_pDamageEffects;
	// プレイヤーコントローラー
	std::unique_ptr<PlayerController> m_pPlayerController;


	// 境界球
	DirectX::BoundingSphere m_inPlayerArea;// プレイヤーと敵との一定範囲内での境界球
	DirectX::BoundingSphere m_playerSphere;// プレイヤーの境界球
	// 攻撃してきた敵の向き
	DirectX::SimpleMath::Vector3 m_enemyDir;
	// プレイヤーがダメージを食らった時
	bool m_isDamage = false;
	bool m_isPlayEffect = false;// エフェクト再生フラグ
	// プレイヤーがダメージを食らった時の時間
	float m_damageTime = 0.0f;
	// SEの音量
	float m_SEVolume = 0.0f;
	// マウス感度
	float m_mouseSensitive;
	// チートコマンド：敵を一掃するフラグ
	bool m_isKillAll;
	// チートコマンドが有効か
	bool m_isCheat = false;
	// 経過時間
	float m_timer = 0.0f;
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
	FPS_Camera* GetCamera() { return m_pCamera.get(); }// カメラ
	EnemyManager* GetEnemies() { return m_pEnemyManager; }// 敵
	PlayerBullets* GetPlayerBullets() { return m_pPlayerBullets.get(); }// プレイヤーの弾
	DirectX::SimpleMath::Vector3 GetPlayerPos() const { return m_playerPos; }// プレイヤーの位置
	DirectX::SimpleMath::Vector3 GetPlayerDir() const { return m_playerDir; }// プレイヤーの向き
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }// プレイヤーコントローラー
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }// プレイヤーの境界球
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }// プレイヤーと敵との一定範囲内での境界球
	float GetVolume() const { return m_SEVolume; }// 音量取得
	void SetVolume(float volume) { m_SEVolume = volume; }// 音量取得
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
public:
	void CreateBullet();// 弾を生成する
private:
	void PlayerDamage(float elapsedTime);// プレイヤーがダメージを受けた時の処理

};