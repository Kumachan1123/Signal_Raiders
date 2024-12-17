/*
	@file	PlayerBullets.h
	@brief	プレイヤーの弾全体を管理するクラス
*/
#pragma once
#include "Game/Player/Player.h"
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>

class CommonResources;
class Player;
class Enemies;
class PlayerBullets
{
public:
	// コモンリソース
	CommonResources* m_commonResources;
	// プレイヤーの弾
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullet;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// プレイヤー
	Player* m_pPlayer;
	// 敵全体
	Enemies* m_pEnemies;
	// 弾生成フラグ
	bool m_isBullet = false;
	// 弾生成済み
	bool m_isBulletBorned = false;
	// 弾を消す座標
	const float DELETE_BULLET_POSITION = -0.25f;
	// ヒットしたときの音の音量
	const float HIT_VOLUME = 0.8f;
public:
	PlayerBullets(CommonResources* commonResources);
	~PlayerBullets();
	void Initialize(Player* pPlayer, Enemies* pEnemies);
	void Update(float elapsedTime);
	void Render();
	void CreateBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);
	// Getter
public:
	bool GetIsBullet() const { return m_isBullet; }
	bool GetIsBulletBorned() const { return m_isBulletBorned; }
	std::vector<std::unique_ptr<PlayerBullet>>& GetPlayerBullet() { return m_playerBullet; }
	// Setter
public:
	void SetIsBullet(bool isBullet) { m_isBullet = isBullet; }
	void SetIsBulletBorned(bool isBulletBorned) { m_isBulletBorned = isBulletBorned; }
private:
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);
};