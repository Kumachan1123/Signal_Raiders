/*
	@file	Boss.h
	@brief	�{�X�N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
// �O���錾
class CommonResources;
class Player;
class Effect;
class EnemyBullets;
class EnemyModel;

class Boss
{
private:
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �v���C���[
	Player* m_pPlayer;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_effect;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// SE�̉���
	float m_SEVolume = 0.0f;
	// �G�����t���O
	bool m_isBossBorn = false;
	// �G�����ς�
	bool m_isBorned = false;
	// �v���C���[�ƓG�̓����蔻��
	bool m_isHitPlayerToBoss = false;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	// �C�����E��
	DirectX::BoundingSphere m_bossBS;	//�G�̋��E��
	DirectX::BoundingSphere m_bossBSToPlayerArea;// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	DirectX::BoundingSphere m_bossBulletBS;// �G�̒e�̋��E��
	DirectX::BoundingSphere m_playerBS;// �v���C���[�̋��E��
	DirectX::SimpleMath::Matrix m_matrix;// �}�g���N�X
	std::unique_ptr<EnemyModel>		m_enemyModel;// �G�̃��f��
	std::unique_ptr<EnemyHPBar>		m_HPBar;// �G��HP�o�[
	std::unique_ptr<EnemyBullets>	m_bossBullets;// �G�̒e
	int m_currentHP;//�G�̗̑�
	bool m_isDead = false;//�G��HP��0�ɂȂ�����True
	bool m_isHit = false;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherBoss = false;// ���̑��̓G�Ƃ̔���
	bool m_isHitToPlayerBullet = false;// �G���v���C���[�̒e�ɓ���������
	bool m_isBullethit = false;// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C
	// �v���C���[�ɗ^����_���[�W
	const float PLAYER_DAMAGE = 5.0f;

public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_bossBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_bossBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	DirectX::SimpleMath::Matrix GetMatrix() const { return m_matrix; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }
	Player* GetPlayer()const { return m_pPlayer; }
	int GetHP() const { return m_currentHP; }
	bool GetEnemyIsDead() const { return m_isDead; }
	bool GetHitToPlayer()const { return m_isHit; }
	bool GetHitToOtherEnemy() const { return m_isHitToOtherBoss; }
	bool GetBulletHitToPlayer() const { return m_isBullethit; }// �G�̒e���v���C���[�ɓ���������
	bool GetHitToPlayerBullet()const { return m_isHitToPlayerBullet; }
	float GetToPlayerDamage() const { return PLAYER_DAMAGE; }
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetEnemyHP(int hp) { m_currentHP = hp; }
	void SetHitToPlayer(bool isHitToPlayer) { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherBoss) { m_isHitToOtherBoss = isHitToOtherBoss; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_bossBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) { m_isBullethit = hit; }// �G�̒e���v���C���[�ɓ���������
	void SetHitToPlayerBullet(bool hit) { m_isHitToPlayerBullet = hit; }
public:
	// �����X�e�[�^�X��ݒ�
	Boss(Player* pPlayer);
	~Boss();
	void Initialize(CommonResources* resources, int hp);
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B);


};