#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullets/PlayerBullets.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/PlayerPointer/PlayerPointer.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/FPS_Camera/FPS_Camera.h"
// 前方宣言
class CommonResources;
class PlayerController;
class PlayerUI;
class PlayerBullets;
class FPS_Camera;
class Enemies;
class Player
{
private:
	// コモンリソース
	CommonResources* m_commonResources;
	// 敵
	Enemies* m_pEnemies;
	// カメラ
	std::unique_ptr<FPS_Camera> m_pCamera;
	// プレイヤーのHP
	float m_playerHP = 100.0f;
	// プレイヤーのHPのUI
	std::unique_ptr <PlayerHP> m_pPlayerHP;
	// プレイヤーの弾
	std::unique_ptr<PlayerBullets> m_pPlayerBullets;
	// 照準
	std::unique_ptr<PlayerPointer> m_pPlayerPointer;
	// プレイヤーコントローラー
	std::unique_ptr<PlayerController> m_pPlayerController;
	// 境界球
	DirectX::BoundingSphere m_inPlayerArea;// プレイヤーと敵との一定範囲内での境界球
	DirectX::BoundingSphere m_playerSphere;// プレイヤーの境界球
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
	float GetPlayerHP() { return m_playerHP; }	// プレイヤーのHP
	FPS_Camera* GetCamera() { return m_pCamera.get(); }// カメラ
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }// プレイヤーコントローラー
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }// プレイヤーの境界球
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }// プレイヤーと敵との一定範囲内での境界球
	// Setter
public:
	void SetPlayerHP(float playerHP) { m_playerHP = playerHP; }	// プレイヤーのHP
};