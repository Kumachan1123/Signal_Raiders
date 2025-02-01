#pragma once
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>

class CommonResources;
class Player;
class IEnemy;
class EnemyManager;
class BulletManager
{
public:
	enum class BulletType
	{
		PLAYER,
		ENEMY
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
	// �G�̒e�̎��
	EnemyBullet::BulletType m_enemyBulletType;
	// �G�̒e�̑傫��
	float m_enemyBulletSize;
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	std::vector<std::unique_ptr<EnemyBullet>> m_enemyBullets;

	static constexpr float DELETE_BULLET_POSITION = -0.25f;
	static constexpr float HIT_VOLUME = 0.8f;
public:
	BulletManager(CommonResources* commonResources);
	~BulletManager();
	void Initialize(Player* pPlayer, EnemyManager* pEnemies);
	void Update(float elapsedTime);
	void Render();
	void CreateBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction, BulletType type);
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);
	bool CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy);
	void SetEnemyBulletType(EnemyBullet::BulletType type) { m_enemyBulletType = type; }
	void SetEnemyBulletSize(float size) { m_enemyBulletSize = size; }
private:
	void UpdatePlayerBullets(float elapsedTime);
	void UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy);
};