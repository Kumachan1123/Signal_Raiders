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
	// �񋓌^
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
	// �A�N�Z�T
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_bossBS; }// �{�X�̋��E���擾
	DirectX::BoundingSphere& GetBulletBoundingSphere()override { return m_enemyBulletBS; }// �e�̋��E���擾
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }// �e�̋��E���ݒ�
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }// �v���C���[�̋��E���擾
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) override { m_playerBS = playerBS; }// �v���C���[�̋��E���ݒ�
	const DirectX::SimpleMath::Matrix& GetMatrix()  override { return m_matrix; }// �}�g���N�X�擾
	const DirectX::SimpleMath::Vector3& GetPosition()  override { return m_position; }// �ʒu�擾
	void SetPosition(DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }// �ʒu�ݒ�
	const DirectX::SimpleMath::Vector3& GetVelocity()  override { return m_velocity; }// ���x�擾
	const DirectX::SimpleMath::Vector3& GetRotate()  override { return m_rotate; }// ��]�擾
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }// �X�P�[���擾
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }// �X�P�[���ݒ�
	DirectX::SimpleMath::Quaternion GetQuaternion() const { return m_quaternion; }// �N�H�[�^�j�I���擾
	void SetQuaternion(DirectX::SimpleMath::Quaternion quaternion) { m_quaternion = quaternion; }// �N�H�[�^�j�I���ݒ�
	DirectX::SimpleMath::Vector3 GetBulletDirection() const { return m_bulletDirection; }// �e�̔�ԕ����擾
	void SetBulletDirection(DirectX::SimpleMath::Vector3 direction) { m_bulletDirection = direction; }// �e�̔�ԕ����ݒ�
	Player* GetPlayer()const override { return m_pPlayer; }// �v���C���[�̃|�C���^�[�擾
	FPS_Camera* GetCamera()const override { return m_pCamera; }// �J�����̃|�C���^�[�擾
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }// �J�����̃|�C���^�[�ݒ�
	int GetEnemyHP() const override { return m_currentHP; }// �G��HP�擾
	void SetEnemyHP(int hp) override;// �G��HP�ݒ�
	bool GetEnemyIsDead() const override { return m_isDead; }// �G�̎��S�t���O�擾
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }// �G�̎��S�t���O�ݒ�
	bool GetHitToPlayer()const override { return m_isHitToPlayer; }// �v���C���[�Ƃ̓����蔻��擾
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }// �v���C���[�Ƃ̓����蔻��ݒ�
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }// ���̓G�Ƃ̓����蔻��擾
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }// ���̓G�Ƃ̓����蔻��ݒ�
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }// �G�̒e���v���C���[�ɓ���������
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }// �G�̒e���v���C���[�ɓ���������
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }// �G���v���C���[�̒e�ɓ���������
	void SetEnemyHitByPlayerBullet(bool hit)override { m_isEnemyHitByPlayerBullet = hit; }// �G���v���C���[�̒e�ɓ���������
	float GetToPlayerDamage() const override { return m_bossDamage; }// �v���C���[�ɗ^����_���[�W�擾
	void SetToPlayerDamage(float damage) { m_bossDamage = damage; }// �v���C���[�ɗ^����_���[�W��ݒ�
	DirectX::SimpleMath::Vector3 GetCameraEye()const { return m_cameraEye; }// �J�����̈ʒu�擾
	DirectX::SimpleMath::Vector3 GetCameraTarget()const { return m_cameraTarget; }// �J�����̃^�[�Q�b�g�擾
	DirectX::SimpleMath::Vector3 GetCameraUp()const { return m_cameraUp; }// �J�����̏�x�N�g���擾
	float GetSheildSEVolume() const { return m_SEVolume - m_SEVolumeCorrection; }//�V�[���h�W�J���̉��ʂ��擾
	void SetPlayerHP(float& HP) const override { HP -= m_bossDamage; }// �v���C���[��HP�����炷
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
	float GetBarrierBreakSize()const { return m_barrierBreakSize; }// �o���A�j��̃p�[�e�B�N���T�C�Y�擾
	void SetBarrierBreakSize(float size) { m_barrierBreakSize = size; }// �o���A�̔j��p�[�e�B�N���T�C�Y�ݒ�
	float GetDeadEffectSize()const { return m_deadEffectSize; }// ���S�G�t�F�N�g�̃T�C�Y�擾
	void SetDeadEffectSize(float size) { m_deadEffectSize = size; }// ���S�G�t�F�N�g�̃T�C�Y�ݒ�
public:
	// public�֐�
	BossBase(Player* pPlayer, CommonResources* resources, int hp);// �R���X�g���N�^
	virtual ~BossBase();// �f�X�g���N�^
	void Initialize() override;// ������
	void Update(float elapsedTime)override;// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// �`��
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;// �Փ˔���̕`��
	void SetBulletType(BossBulletType bossBulletType) { m_bossBulletType = bossBulletType; };// �e�̃^�C�v�ݒ�
	BossBulletType GetBulletType() const { return m_bossBulletType; };// �e�̃^�C�v�擾
	void PlayBarrierSE();// �o���ASE�Đ�
private:
	// private�֐�
	void ShootBullet();// �e����
	void CreateVerticalBullet();// �����̒e�𐶐�
	void CreateSpiralBullet();// �����e�𐶐�

private:
	// private�����o�ϐ�
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
	// �J�����̃|�C���^�[
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
	BulletType m_bulletType;// EnemyBullet�N���X�ɑ���
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
	DirectX::SimpleMath::Vector3 m_cameraEye;// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraTarget;// �J�����̃^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_cameraUp;// �J�����̏�x�N�g��
	float m_defaultHitRadius;// �ʏ펞�̓����蔻��̔��a
	float m_defensiveHitRadius;// �h�䎞�̓����蔻��̔��a
	float m_bulletSize;// �e�̃T�C�Y
	float m_barrierBreakSize;// �o���A�̔j��p�[�e�B�N���T�C�Y
	float m_deadEffectSize;//���S�G�t�F�N�g�̃T�C�Y

};