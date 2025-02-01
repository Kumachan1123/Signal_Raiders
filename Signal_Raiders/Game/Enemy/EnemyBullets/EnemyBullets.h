/*
	@file	EnemyBullets.h
	@brief	�G�̒e�S�̂��Ǘ�����N���X
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Particle/Particle.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"

class CommonResources;
class EnemyBullet;
class Particle;
class Player;
class Enemy;
class IEnemy;
class EnemyBullets
{
public:
	// �ϐ�
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	DirectX::BoundingSphere m_enemyBulletBS;
	DirectX::BoundingSphere m_playerBS;
	IEnemy* m_pEnemy;// �G�̃|�C���^�[
	bool m_isPlayerHitByEnemyBullet = false;// �G�̒e���v���C���[�ɓ���������
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // �e�̃��X�g
	std::vector<std::unique_ptr<Particle>> m_bulletTrails; // �e�̋O�Ճ��X�g
	DirectX::SimpleMath::Vector3 m_bulletSpawnPos;		// �G�̒e�̔��ˈʒu
	DirectX::SimpleMath::Vector3 m_direction;		// �e�̕���
	// �֐�
	EnemyBullets(IEnemy* pEnemy);
	~EnemyBullets();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBullet(float size, EnemyBullet::BulletType type);
	// Getter
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	// Setter
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBS = playerBS; }
	void SetPlayerHitByEnemyBullet(bool hit) { m_isPlayerHitByEnemyBullet = hit; }// �G�̒e���v���C���[�ɓ���������
	void SetRotateDirection(int direction);
	void SetDirection(DirectX::SimpleMath::Vector3 dir) { m_direction = dir; }
	void SetEnemyBulletSpawnPosition(DirectX::SimpleMath::Vector3 pos) { m_bulletSpawnPos = pos; }
};