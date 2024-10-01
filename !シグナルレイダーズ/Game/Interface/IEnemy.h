/*
	@file	EnemyBase.h
	@brief	�G�̊��N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"

class Player;
class EnemyModel;
class EnemyBullets;
class EnemyHPBar;
class CommonResources;
class IEnemy
{
protected:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;	// ���W
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::SimpleMath::Matrix m_matrix;		// �}�g���N�X
	DirectX::BoundingSphere m_boundingSphere;	// �G�̋��E��
	DirectX::BoundingSphere m_bulletBoundingSphere;	// �G�̒e�̋��E��
	DirectX::BoundingSphere m_playerBoundingSphere;	// �v���C���[�̋��E��
	// �̗�
	int m_currentHP;
	bool m_isDead = false;	// �G�����񂾂��ǂ���
	bool m_isHit = false;	// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy = false;	// ���̓G�Ƃ̔���
	bool m_isHitToPlayerBullet = false;	// �v���C���[�̒e�ɓ���������
	bool m_isBulletHit = false;	// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown = 3.0f;	// �U���̃N�[���_�E��
	const float PLAYER_DAMAGE = 1.0f; // �v���C���[�ɗ^����_���[�W
	// ���f��
	std::unique_ptr<EnemyModel> m_enemyModel;
	// HP�o�[
	std::unique_ptr<EnemyHPBar> m_HPBar;
	// �e
	std::unique_ptr<EnemyBullets> m_enemyBullets;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	IEnemy(Player* pPlayer);
	virtual ~IEnemy() = default;

	// getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_bulletBoundingSphere; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBoundingSphere; }
	DirectX::SimpleMath::Matrix GetMatrix() const { return m_matrix; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }
	Player* GetPlayer() const { return m_pPlayer; }
	int GetHP() const { return m_currentHP; }
	bool IsDead() const { return m_isDead; }
	bool IsHitToPlayer() const { return m_isHit; }
	bool IsHitToOtherEnemy() const { return m_isHitToOtherEnemy; }
	bool IsBulletHitToPlayer() const { return m_isBulletHit; }
	bool IsHitToPlayerBullet() const { return m_isHitToPlayerBullet; }
	float GetToPlayerDamage() const { return PLAYER_DAMAGE; }
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetHP(int hp) { m_currentHP = hp; }
	void SetHitToPlayer(bool isHitToPlayer) { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_bulletBoundingSphere = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBoundingSphere = playerBS; }
	void SetPlayerHP(float& HP) const { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) { m_isBulletHit = hit; }
	void SetHitToPlayerBullet(bool hit) { m_isHitToPlayerBullet = hit; }

	// ���z�֐�
	virtual void Initialize(CommonResources* resources, int hp) = 0;
	virtual void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos) = 0;
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	virtual void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B) = 0;

};