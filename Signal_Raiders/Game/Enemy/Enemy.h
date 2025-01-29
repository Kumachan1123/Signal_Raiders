/*
	@file	Enemy.h
	@brief	�G�N���X
*/
#pragma once
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
//�O���錾
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class EnemyBullets;
class EnemyManager;
class FPS_Camera;
class Enemy : public IEnemy
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	std::unique_ptr<EnemyModel>		m_enemyModel;// �G�̃��f��
	std::unique_ptr<EnemyAI>		m_enemyAI;// �G��AI
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// �G��HP�o�[
	std::unique_ptr<EnemyBullets>	m_enemyBullets;// �G�̒e
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // �e�̃��X�g
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	FPS_Camera* m_pCamera;
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	// �C�����E��
	DirectX::BoundingSphere m_enemyBS;	//�G�̋��E��
	DirectX::BoundingSphere m_enemyBSToPlayerArea;// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	DirectX::BoundingSphere m_enemyBulletBS;// �G�̒e�̋��E��
	DirectX::BoundingSphere m_playerBS;// �v���C���[�̋��E��
	DirectX::SimpleMath::Matrix m_matrix;// �}�g���N�X

	int m_currentHP;//�G�̗̑�
	bool m_isDead;//�G��HP��0�ɂȂ�����True
	bool m_isHitToPlayer;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy;// ���̑��̓G�Ƃ̔���
	bool m_isEnemyHitByPlayerBullet;// �G���v���C���[�̒e�ɓ���������
	bool m_isPlayerHitByEnemyBullet;// �G�̒e���v���C���[�ɓ���������
	bool m_canAttack;// �U���\��
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��
	// �v���C���[�ɗ^����_���[�W�i�P�_���[�W�j
	const float PLAYER_DAMAGE = 1.0f;
	// �U���̊Ԋu�i�P�b�j
	const float ATTACK_INTERVAL = 1.0f;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	//// �J����
	//DirectX::SimpleMath::Vector3 m_cameraEye;
	//DirectX::SimpleMath::Vector3 m_cameraTarget;
	//DirectX::SimpleMath::Vector3 m_cameraUp;

public:
	//	getter
	const DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	const DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	const DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	const DirectX::SimpleMath::Matrix& GetMatrix()  override { return m_matrix; }
	const DirectX::SimpleMath::Vector3& GetPosition()  override { return m_position; }
	const DirectX::SimpleMath::Vector3& GetVelocity()  override { return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetRotate()  override { return m_rotate; }
	Player* GetPlayer()const override { return m_pPlayer; }
	FPS_Camera* GetCamera()const override { return m_pCamera; }
	void SetAudioManager(AudioManager* audioManager) override { m_audioManager = audioManager; }
	int GetEnemyHP() const override { return m_currentHP; }
	bool GetEnemyIsDead() const override { return m_isDead; }
	bool GetHitToPlayer()const override { return m_isHitToPlayer; }
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }// �G�̒e���v���C���[�ɓ���������
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }// �v���C���[�̒e���G�ɓ���������
	float GetToPlayerDamage() const override { return PLAYER_DAMAGE; }
	bool GetCanAttack() const override { return m_canAttack; }// �U���\��
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos)override { m_position = pos; }
	void SetEnemyHP(int hp)override { m_currentHP -= hp; }
	void SetEnemyIsDead(bool isDead)override { m_isDead = isDead; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs)override { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS)override { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const override { HP -= PLAYER_DAMAGE; }
	void SetPlayerHitByEnemyBullet(bool hit)override { m_isPlayerHitByEnemyBullet = hit; }// �G�̒e���v���C���[�ɓ���������
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }// �G���v���C���[�̒e�ɓ���������
	void SetCanAttack(bool canAttack)override { m_canAttack = canAttack; }// �U���\��
	//void SetCameraEye(DirectX::SimpleMath::Vector3 eye)override { m_cameraEye = eye; }
	//void SetCameraTarget(DirectX::SimpleMath::Vector3 target)override { m_cameraTarget = target; }
	//void SetCameraUp(DirectX::SimpleMath::Vector3 up)override { m_cameraUp = up; }
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
public:
	// �����X�e�[�^�X��ݒ�
	Enemy(Player* pPlayer, CommonResources* resources, int hp);
	~Enemy();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
private:
	void ShootBullet();// �e������
};