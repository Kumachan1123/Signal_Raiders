/*
	@file	Enemy.h
	@brief	�G�N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"

//�O���錾
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class EnemyBullets;
class Enemies;
class Enemy
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
	std::unique_ptr<DirectX::Model> m_model;// �e�p�̃��f��
	std::unique_ptr<EnemyModel>		m_enemyModel;// �G�̃��f��
	std::unique_ptr<EnemyAI>		m_enemyAI;// �G��AI
	std::unique_ptr<EnemyHPBar>		m_HPBar;// �G��HP�o�[
	std::unique_ptr<EnemyBullets>	m_enemyBullets;// �G�̒e
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // �e�̃��X�g
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	// �e�̐[�x�X�e���V���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>m_depthStencilState;
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
	bool m_isDead = false;//�G��HP��0�ɂȂ�����True
	bool m_isHit = false;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy = false;// ���̑��̓G�Ƃ̔���
	bool m_isHitToPlayerBullet = false;// �G���v���C���[�̒e�ɓ���������
	bool m_isBullethit = false;// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C
	// �v���C���[�ɗ^����_���[�W
	const float PLAYER_DAMAGE = 1.0f;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_enemyBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	DirectX::SimpleMath::Matrix GetMatrix() const { return m_matrix; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }
	Player* GetPlayer()const { return m_pPlayer; }
	int GetHP() const { return m_currentHP; }
	bool GetEnemyIsDead() const { return m_isDead; }
	bool GetHitToPlayer()const { return m_isHit; }
	bool GetHitToOtherEnemy() const { return m_isHitToOtherEnemy; }
	bool GetBulletHitToPlayer() const { return m_isBullethit; }// �G�̒e���v���C���[�ɓ���������
	bool GetHitToPlayerBullet()const { return m_isHitToPlayerBullet; }
	float GetToPlayerDamage() const { return PLAYER_DAMAGE; }
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetEnemyHP(int hp) { m_currentHP = hp; }
	void SetHitToPlayer(bool isHitToPlayer) { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) { m_isBullethit = hit; }// �G�̒e���v���C���[�ɓ���������
	void SetHitToPlayerBullet(bool hit) { m_isHitToPlayerBullet = hit; }
public:
	// �����X�e�[�^�X��ݒ�
	Enemy(Player* pPlayer);
	~Enemy();
	void Initialize(CommonResources* resources, int hp);
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B);


};