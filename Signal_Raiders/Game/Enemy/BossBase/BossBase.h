/*
	@file	BossBase.h
	@brief	�{�X�x�[�X�N���X
*/
#pragma once
// �W�����C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Enemy/LastBoss/LastBoss.h"
#include "Game/Enemy/Boss/BossSheild/BossSheild.h"
#include "Game/Enemy/EnemyFactory/EnemyFactory.h"
#include "Game/Enemy/EnemyType/EnemyType.h"

//�O���錾
class CommonResources;
class PlayScene;
class Player;
class FPS_Camera;
class BossAI;
class BossModel;
class BossSheild;
class EnemyHPBar;
class EnemyBullet;
class EnemyManager;

/// �{�X�x�[�X�N���X(��������Boss��LastBoss���h��)
class BossBase : public IEnemy
{
public:

	// �{�X�̒e�̎��
	enum class BossBulletType
	{
		STAGE_1 = 0,	// �X�e�[�W1
		STAGE_2,		// �X�e�[�W2
		STAGE_3,		// �X�e�[�W3
		STAGE_4,		// �X�e�[�W4
		STAGE_5,		// �X�e�[�W5
	};
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_bossBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere()override { return m_enemyBulletBS; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) override { m_playerBS = playerBS; }
	const DirectX::SimpleMath::Matrix& GetMatrix()  override { return m_matrix; }
	const DirectX::SimpleMath::Vector3& GetPosition()  override { return m_position; }
	void SetPosition(DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	const DirectX::SimpleMath::Vector3& GetVelocity()  override { return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetRotate()  override { return m_rotate; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }// �X�P�[���擾
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }// �X�P�[���ݒ�
	DirectX::SimpleMath::Quaternion GetQuaternion() const { return m_quaternion; }// �N�H�[�^�j�I���擾
	void SetQuaternion(DirectX::SimpleMath::Quaternion quaternion) { m_quaternion = quaternion; }// �N�H�[�^�j�I���ݒ�
	DirectX::SimpleMath::Vector3 GetBulletDirection() const { return m_bulletDirection; }// �e�̔�ԕ����擾
	void SetBulletDirection(DirectX::SimpleMath::Vector3 direction) { m_bulletDirection = direction; }// �e�̔�ԕ����ݒ�
	Player* GetPlayer()const override { return m_pPlayer; }
	FPS_Camera* GetCamera()const override { return m_pCamera; }
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	int GetEnemyHP() const override { return m_currentHP; }
	void SetEnemyHP(int hp) override;
	bool GetEnemyIsDead() const override { return m_isDead; }
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	bool GetHitToPlayer()const override { return m_isHitToPlayer; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }// �G�̒e���v���C���[�ɓ���������
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }// �G�̒e���v���C���[�ɓ���������
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }
	void SetEnemyHitByPlayerBullet(bool hit)override { m_isEnemyHitByPlayerBullet = hit; }
	float GetToPlayerDamage() const override { return m_bossDamage; }
	void SetToPlayerDamage(float damage) { m_bossDamage = damage; }// �v���C���[�ɗ^����_���[�W��ݒ�
	DirectX::SimpleMath::Vector3 GetCameraEye()const { return m_cameraEye; }
	DirectX::SimpleMath::Vector3 GetCameraTarget()const { return m_cameraTarget; }
	DirectX::SimpleMath::Vector3 GetCameraUp()const { return m_cameraUp; }
	float GetSheildSEVolume() const { return m_SEVolume - m_SEVolumeCorrection; }//�V�[���h�W�J���̉��ʂ��擾
	void SetPlayerHP(float& HP) const override { HP -= m_bossDamage; }
	void SetBulletCooldown(float cooldown) { m_bulletCooldown = cooldown; }// �V�[���h��W�J������Ɏ��s����
	void SetInitSpecialAttacCooldown(float cooldown) { m_initSpecialAttackCooldown = cooldown; }// ����U���̃N�[���_�E��
	bool GetCanAttack() const override { return m_canAttack; }// �U���\��
	void SetCanAttack(bool canAttack)override { m_canAttack = canAttack; }// �U���\��
	BulletManager* GetBulletManager()const override { return m_pBulletManager; }// �e�̊Ǘ��N���X�擾
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }// �e�̊Ǘ��N���X�ݒ�
	bool GetIsAttack() const override { return m_isAttack; }// �U������
	void SetIsAttack(bool isAttack)override { m_isAttack = isAttack; }// �U������
	void SetBossType(BossType type) { m_bossType = type; }// �{�X�̎�ނ�ݒ�
	BossSheild* GetBossSheild()const { return m_pBossSheild.get(); }// �V�[���h�擾
	BossAI* GetBossAI() { return m_pBossAI.get(); }// AI�擾
	EnemyHPBar* GetHPBar() { return m_pHPBar.get(); }// HP�o�[�擾
	float GetDefaultHitRadius()const { return m_defaultHitRadius; }//�ʏ펞�̓����蔻��̔��a�擾
	void SetDefaultHitRadius(float radius) { m_defaultHitRadius = radius; }//�ʏ펞�̓����蔻��̔��a�ݒ�
	float GetDefensiveHitRadius()const { return m_defensiveHitRadius; }//�h�䎞�̓����蔻��̔��a�擾
	void SetDefensiveHitRadius(float radius) { m_defensiveHitRadius = radius; }//�h�䎞�̓����蔻��̔��a�ݒ�
	float GetBulletSize()const { return m_bulletSize; }// �e�̃T�C�Y�擾
	void SetBulletSize(float size) { m_bulletSize = size; }// �e�̃T�C�Y�ݒ�
public:
	// �����X�e�[�^�X��ݒ�
	BossBase(Player* pPlayer, CommonResources* resources, int hp);
	virtual ~BossBase();
	void Initialize() override;// ������
	void Update(float elapsedTime)override;// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// �`��
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;// �Փ˔���̕`��
	void SetBulletType(BossBulletType bossBulletType) { m_bossBulletType = bossBulletType; };// �e�̃^�C�v�ݒ�
	BossBulletType GetBulletType() const { return m_bossBulletType; };// �e�̃^�C�v�擾
	void PlayBarrierSE();// �o���ASE�Đ�
private:
	void ShootBullet();// �e����
	void CreateVerticalBullet();// �����̒e�𐶐�
	void CreateSpiralBullet();// �����e�𐶐�

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	std::unique_ptr<BossAI>		m_pBossAI;// AI
	std::unique_ptr<BossModel>		m_pBossModel;//���f��
	std::unique_ptr<BossSheild>		m_pBossSheild;// �V�[���h
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// HP�o�[

	// ����
	float m_SEVolume;// SE�̉���
	float m_SEVolumeCorrection;// SE�̉��ʕ␳

	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	FPS_Camera* m_pCamera;

	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::SimpleMath::Quaternion m_quaternion;	// ��]�p�N�H�[�^�j�I��
	DirectX::SimpleMath::Vector3 m_scale;		// �X�P�[��
	// �C�����E��
	DirectX::BoundingSphere m_bossBS;	//�G�̋��E��
	DirectX::BoundingSphere m_enemyBSToPlayerArea;// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	DirectX::BoundingSphere m_enemyBulletBS;// �G�̒e�̋��E��
	DirectX::BoundingSphere m_playerBS;// �v���C���[�̋��E��
	DirectX::SimpleMath::Matrix m_matrix;// �}�g���N�X
	// �e�̔��ˈʒu
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	float m_bossDamage;// �{�X���v���C���[�ɗ^����_���[�W
	// ���ˈʒu����]�����邽�߂̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// �e�̃^�C�v
	BossBulletType m_bossBulletType;// Enemies�N���X�Őݒ肷��
	EnemyBullet::BulletType m_bulletType;// EnemyBullet�N���X�ɑ���
	// �{�X�̎��
	BossType m_bossType;
	// ��������Boss
	std::unique_ptr<IBossLogic> m_pBoss;
	// �e�̔�ԕ���
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// ����
	float m_time;
	// �G�̃X�e�[�^�X
	int m_currentHP;//�G�̗̑�
	int m_maxHP;//�G�̍ő�̗�
	bool m_isDead;//�G��HP��0�ɂȂ�����True
	bool m_isHitToPlayer;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy;// ���̑��̓G�Ƃ̔���
	bool m_isEnemyHitByPlayerBullet;// �G���v���C���[�̒e�ɓ���������
	bool m_isPlayerHitByEnemyBullet;// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown;  // �U���̃N�[���_�E��(�t���[�����Ƃɔ��˂��邱�Ƃ�h���p�j
	float m_specialAttackCooldown;  // ����U���̃N�[���_�E��
	float m_initSpecialAttackCooldown;  // ����U���̃N�[���_�E��(�������p)
	float m_bulletCooldown;  // �e�̃N�[���_�E��
	bool m_canAttack;// �U���\��
	bool m_isAttack;// �U������
	// �v���C���[�̃J�����̏��
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;

	float m_defaultHitRadius;// �ʏ펞�̓����蔻��̔��a
	float m_defensiveHitRadius;// �h�䎞�̓����蔻��̔��a

	float m_bulletSize;// �e�̃T�C�Y

};