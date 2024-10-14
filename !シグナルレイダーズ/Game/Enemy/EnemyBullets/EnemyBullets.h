/*
	@file	EnemyBullets.h
	@brief	�G�̒e�S�̂��Ǘ�����N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/BulletTrail/BulletTrail.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"

class CommonResources;
class EnemyBullet;
class BulletTrail;
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
	bool m_isBullethit = false;// �G�̒e���v���C���[�ɓ���������
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // �e�̃��X�g
	std::vector<std::unique_ptr<BulletTrail>> m_bulletTrails; // �e�̋O�Ճ��X�g
	DirectX::SimpleMath::Vector3 m_position;		// �G�̍��W
	// �֐�
	EnemyBullets(IEnemy* pEnemy);
	~EnemyBullets();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 enemyPos);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBullet(DirectX::SimpleMath::Vector3 enemyPos, DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 playerPos, float size);
	// Getter
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	// Setter
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBS = playerBS; }
	void SetBulletHitToPlayer(bool hit) { m_isBullethit = hit; }// �G�̒e���v���C���[�ɓ���������
};