/*
	@file	Boss.h
	@brief	�{�X�N���X
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
//�O���錾
class CommonResources;
class PlayScene;
class Player;
class FPS_Camera;
class BossAI;
class BossModel;
class EnemyHPBar;
class EnemyBullet;
class EnemyBullets;
class Enemies;
class Boss : public IEnemy
{
public:
	enum class BossBulletType
	{
		NORMAL = 0,	// �ꔭ
		TWIN,		// ��
		THREE,		// �O��
		SPIRAL,		// �O������
	};
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
	std::unique_ptr<BossAI>		m_pBossAI;// AI
	std::unique_ptr<BossModel>		m_bossModel;//���f��
	std::unique_ptr<EnemyHPBar>		m_HPBar;// HP�o�[
	std::unique_ptr<EnemyBullets>	m_enemyBullets;// �e
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // �e�̃��X�g
	const float BULLET_SIZE = 0.25f; // �e�̑傫��



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
	// �e�̔��ˈʒu
	// ����
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	// ��
	DirectX::SimpleMath::Vector3 m_bulletPosLeft;
	// �E
	DirectX::SimpleMath::Vector3 m_bulletPosRight;
	// ���ˈʒu����]�����邽�߂̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// �e�̃^�C�v
	BossBulletType m_bossBulletType;// Enemies�N���X�Őݒ肷��
	EnemyBullet::BulletType m_bulletType;// EnemyBullet�N���X�ɑ���
	// �e�̔�ԕ���
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// ����
	float m_time;
	// �G�̃X�e�[�^�X
	int m_currentHP;//�G�̗̑�
	bool m_isDead = false;//�G��HP��0�ɂȂ�����True
	bool m_isHit = false;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy = false;// ���̑��̓G�Ƃ̔���
	bool m_isHitToPlayerBullet = false;// �G���v���C���[�̒e�ɓ���������
	bool m_isBullethit = false;// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C
	// �v���C���[�ɗ^����_���[�W
	const float PLAYER_DAMAGE = 5.0f;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere()override { return m_enemyBulletBS; }
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
	void SetPosition(DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	void SetEnemyHP(int hp) override { m_currentHP = hp; }
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) override { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const override { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) override { m_isBullethit = hit; }// �G�̒e���v���C���[�ɓ���������
	void SetHitToPlayerBullet(bool hit)override { m_isHitToPlayerBullet = hit; }
public:
	// �����X�e�[�^�X��ݒ�
	Boss(Player* pPlayer);
	~Boss();
	void Initialize(CommonResources* resources, int hp) override;// ������
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)override;// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// �`��
	void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)override;// �Փ˔���

	void SetBulletType(BossBulletType bossBulletType) { m_bossBulletType = bossBulletType; };// �e�̃^�C�v�ݒ�
	BossBulletType GetBulletType() const { return m_bossBulletType; };// �e�̃^�C�v�擾
private:
	void ShootBullet();// �e����
	void BulletPotsitioning();// �e�̈ʒu�ݒ�
	void CreateBullet();// �e�𐶐�
	void CreateCenterBullet(EnemyBullet::BulletType type);// �����̒e�𐶐�
	void CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type);// ���̒e�𐶐�
	void CreateRightBullet(float angleOffset, EnemyBullet::BulletType type);// �E�̒e�𐶐�
	void CreateVerticalBullet();// �����̒e�𐶐�
};