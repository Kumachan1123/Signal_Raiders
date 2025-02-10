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
	// �e�̎��
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
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// �v���C���[
	Player* m_pPlayer;
	// �G�S��
	EnemyManager* m_pEnemyManager;
	// �e���ˏo�����G�̃|�C���^�[
	IEnemy* m_pShooter;
	// �G�̒e�̎��
	EnemyBullet::BulletType m_enemyBulletType;
	// �G�̒e�̑傫��
	float m_enemyBulletSize;
	// �v���C���[�̒e�����t���O
	bool m_isPlayerShoot = false;
	// �o�ߎ���
	float m_elapsedTime;
	// �����[�h�^�C�}�[
	float m_reloadTimer;
	// �v���C���[�̒e
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	// �G�̒e
	std::vector<std::unique_ptr<EnemyBullet>> m_enemyBullets;
	// �O���b�h�}�b�v
	std::unordered_map<GridKey, std::vector<PlayerBullet*>, GridKeyHash> m_playerBulletGrid;
	std::unordered_map<GridKey, std::vector<EnemyBullet*>, GridKeyHash> m_enemyBulletGrid;

	// �v���C���[�̒e�̍ő吔
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
	void CheckCollisionWithBullets();	// �G�̒e�ƃv���C���[�̒e�̓����蔻��
	void RemoveBulletsByShooter(IEnemy* shooter);
	void SetEnemyBulletType(EnemyBullet::BulletType type) { m_enemyBulletType = type; }
	void SetEnemyBulletSize(float size) { m_enemyBulletSize = size; }
	bool GetIsPlayerShoot() const { return m_isPlayerShoot; }
	void SetIsPlayerShoot(bool isPlayerShoot) { m_isPlayerShoot = isPlayerShoot; }
	void SetShooter(IEnemy* pShooter) { m_pShooter = pShooter; }
	void ReLoadPlayerBullet();// �e�̕�[
	void ConsumePlayerBullet();// �e�̏���
	void SetAdditionalDamage(int additionalDamage);
	int GetPlayerBulletCount() const { return m_playerBulletCount; }
private:
	void UpdatePlayerBullets(float elapsedTime);
	void UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy);
	void SetSound();
	GridKey GetGridKey(const DirectX::SimpleMath::Vector3& position);
};