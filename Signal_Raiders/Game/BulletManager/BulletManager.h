#pragma once
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <unordered_map>
#include <cassert>
#include "Game/BulletParameters/BulletParameters.h"
class CommonResources;
class Player;
class IEnemy;
class EnemyManager;
class BulletParameters;
class BulletManager
{
public:
	// 弾の種類
	enum class BulletType
	{
		PLAYER,
		ENEMY
	};
private:
	struct GridKey
	{
		int x, y;

		bool operator==(const GridKey& other) const
		{
			return x == other.x && y == other.y;
		}
	};

	struct GridKeyHash
	{
		std::size_t operator()(const GridKey& key) const
		{
			return std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1);
		}
	};
private:
	// コモンリソース
	CommonResources* m_commonResources;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// プレイヤー
	Player* m_pPlayer;
	// 敵全体
	EnemyManager* m_pEnemyManager;
	// 弾を射出した敵のポインター
	IEnemy* m_pShooter;
	// 敵の弾の種類
	EnemyBullet::BulletType m_enemyBulletType;
	// 敵の弾の大きさ
	float m_enemyBulletSize;
	// プレイヤーの弾生成フラグ
	bool m_isPlayerShoot = false;
	// 経過時間
	float m_elapsedTime;
	// リロードタイマー
	float m_reloadTimer;
	// プレイヤーの弾
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	// 敵の弾
	std::vector<std::unique_ptr<EnemyBullet>> m_enemyBullets;
	// グリッドマップ
	std::unordered_map<GridKey, std::vector<PlayerBullet*>, GridKeyHash> m_playerBulletGrid;
	std::unordered_map<GridKey, std::vector<EnemyBullet*>, GridKeyHash> m_enemyBulletGrid;

	// プレイヤーの弾の最大数
	int m_playerBulletCount;
	static constexpr float DELETE_BULLET_POSITION = -0.25f;
	static constexpr int MAX_PLAYER_BULLET_COUNT = 50;
	static constexpr float HIT_VOLUME = 0.8f;
public:
	BulletManager(CommonResources* commonResources);
	~BulletManager();
	void Initialize(Player* pPlayer, EnemyManager* pEnemies);
	void Update(float elapsedTime);
	void Render();
	void CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);
	void CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);
	bool CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy);
	void CheckCollisionWithBullets();	// 敵の弾とプレイヤーの弾の当たり判定
	void RemoveBulletsByShooter(IEnemy* shooter);
	void SetEnemyBulletType(EnemyBullet::BulletType type) { m_enemyBulletType = type; }
	void SetEnemyBulletSize(float size) { m_enemyBulletSize = size; }
	bool GetIsPlayerShoot() const { return m_isPlayerShoot; }
	void SetIsPlayerShoot(bool isPlayerShoot) { m_isPlayerShoot = isPlayerShoot; }
	void SetShooter(IEnemy* pShooter) { m_pShooter = pShooter; }
	void ReLoadPlayerBullet();// 弾の補充
	void ConsumePlayerBullet();// 弾の消費
	void SetAdditionalDamage(int additionalDamage);
	int GetPlayerBulletCount() const { return m_playerBulletCount; }
private:
	void UpdatePlayerBullets(float elapsedTime);
	void UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy);
	void SetSound();
	GridKey GetGridKey(const DirectX::SimpleMath::Vector3& position);
};