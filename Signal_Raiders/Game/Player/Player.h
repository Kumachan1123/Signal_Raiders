#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullets/PlayerBullets.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/PlayerPointer/PlayerPointer.h"
#include "Game/Player/PlayerUI/Crisis/Crisis.h"
#include "Game/DamageEffect/DamageEffect.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/FPS_Camera/FPS_Camera.h"
// 前方宣言
class CommonResources;
class PlayerController;
class PlayerUI;
class PlayerBullets;
class PlayerPointer;
class PlayerHP;
class Crisis;
class DamageEffect;
class FPS_Camera;
class Enemies;
class Player
{
private:
	// コモンリソース
	CommonResources* m_commonResources;
	// 敵
	Enemies* m_pEnemies;
	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPos;
	// プレイヤーの向き
	DirectX::SimpleMath::Vector3 m_playerDir;
	// カメラ
	std::unique_ptr<FPS_Camera> m_pCamera;
	// プレイヤーのHP
	float m_playerHP;
	// プレイヤーのHPのUI
	std::unique_ptr <PlayerHP> m_pPlayerHP;
	// プレイヤーの弾
	std::unique_ptr<PlayerBullets> m_pPlayerBullets;
	// 照準
	std::unique_ptr<PlayerPointer> m_pPlayerPointer;
	// ダメージエフェクト
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;
	// プレイヤーコントローラー
	std::unique_ptr<PlayerController> m_pPlayerController;
	// 危機状態
	std::unique_ptr<Crisis> m_pCrisis;
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

public:
	Player(CommonResources* commonResources);
	~Player();

	// 初期化
	void Initialize(Enemies* pEnemies);
	// 更新
	void Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb, float elapsedTime);
	// 描画
	void Render();
	// Getter
public:
	float GetPlayerHP() const { return m_playerHP; }	// プレイヤーのHP
	FPS_Camera* GetCamera() { return m_pCamera.get(); }// カメラ
	DirectX::SimpleMath::Vector3 GetPlayerPos() const { return m_playerPos; }// プレイヤーの位置
	DirectX::SimpleMath::Vector3 GetPlayerDir() const { return m_playerDir; }// プレイヤーの向き
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }// プレイヤーコントローラー
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }// プレイヤーの境界球
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }// プレイヤーと敵との一定範囲内での境界球
	float GetVolume() const { return m_SEVolume; }// 音量取得
	float GetMouseSensitive() const { return m_mouseSensitive; }// マウス感度
	DirectX::SimpleMath::Vector3 GetEnemyDir() const { return m_enemyDir; }// 攻撃してきた敵の向き
	bool GetisPlayerDamage() const { return m_isDamage; }	// プレイヤーがダメージを受けたか
	bool GetisPlayEffect() const { return m_isPlayEffect; }// エフェクト再生フラグ
	bool GetisKillAll() const { return m_isKillAll; }// チートコマンド：敵を一掃するフラグ
	// Setter
public:
	void SetPlayerHP(float playerHP) { m_playerHP = playerHP; }	// プレイヤーのHP
	void SetisPlayerDamage(bool isDamage) { m_isDamage = isDamage; }// プレイヤーがダメージを受けたか
	void SetVolume(float volume) { m_SEVolume = volume; }// 音量取得
	void SetMouseSensitive(float sensitive) { m_mouseSensitive = sensitive; }// マウス感度
	void SetEnemyBulletDirection(DirectX::SimpleMath::Vector3 enemyDir) { m_enemyDir = enemyDir; }// 攻撃してきた敵の向き
	void SetisPlayEffect(bool isPlayEffect) { m_isPlayEffect = isPlayEffect; }// エフェクト再生フラグ
};