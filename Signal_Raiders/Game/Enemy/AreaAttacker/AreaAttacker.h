/*
	@file	AreaAttacker.h
	@brief	�͈͍U���G�N���X
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Interface/IEnemy.h"
//�O���錾
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class AreaAttackerModel;
class EnemyBullets;
class Enemies;
class FPS_Camera;
class AreaAttacker : public IEnemy
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
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<AreaAttackerModel>		m_pAreaAttackerModel;// �G�̃��f��
	std::unique_ptr<EnemyAI>		m_enemyAI;// �G��AI
	std::unique_ptr<EnemyHPBar>		m_HPBar;// �G��HP�o�[
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
	bool m_isHit;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy;// ���̑��̓G�Ƃ̔���
	bool m_isHitToPlayerBullet;// �G���v���C���[�̒e�ɓ���������
	bool m_isBullethit;// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C
	// �v���C���[�ɗ^����_���[�W
	const float PLAYER_DAMAGE = 1.0f;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// �J����
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	DirectX::SimpleMath::Matrix GetMatrix() const override { return m_matrix; }
	DirectX::SimpleMath::Vector3 GetPosition() const override { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const override { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetRotate() const override { return m_rotate; }
	Player* GetPlayer()const override { return m_pPlayer; }
	FPS_Camera* GetCamera()const override { return m_pCamera; }
	int GetHP() const override { return m_currentHP; }
	bool GetEnemyIsDead() const override { return m_isDead; }
	bool GetHitToPlayer()const override { return m_isHit; }
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	bool GetBulletHitToPlayer() const override { return m_isBullethit; }// �G�̒e���v���C���[�ɓ���������
	bool GetHitToPlayerBullet()const override { return m_isHitToPlayerBullet; }
	float GetToPlayerDamage() const override { return PLAYER_DAMAGE; }
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos)override { m_position = pos; }
	void SetEnemyHP(int hp)override { m_currentHP -= hp; }
	void SetEnemyIsDead(bool isDead)override { m_isDead = isDead; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs)override { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS)override { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const override { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit)override { m_isBullethit = hit; }// �G�̒e���v���C���[�ɓ���������
	void SetHitToPlayerBullet(bool hit) override { m_isHitToPlayerBullet = hit; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye)override { m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target)override { m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up)override { m_cameraUp = up; }
public:
	// �����X�e�[�^�X��ݒ�
	AreaAttacker(Player* pPlayer);
	~AreaAttacker();
	void Initialize(CommonResources* resources, int hp) override;
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
};

