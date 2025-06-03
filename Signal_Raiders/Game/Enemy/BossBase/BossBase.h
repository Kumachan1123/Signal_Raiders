/*
*	@file	BossBase.h
*	@brief	�{�X�x�[�X�N���X
*/
#pragma once
// �W�����C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
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
public:// �񋓌^
	// �{�X�̒e�̎��
	enum class BossBulletType
	{
		STAGE_1 = 0,	// �X�e�[�W1
		STAGE_2,		// �X�e�[�W2
		STAGE_3,		// �X�e�[�W3
		STAGE_4,		// �X�e�[�W4
		STAGE_5,		// �X�e�[�W5
	};
public:	// �A�N�Z�T
	// �{�X�̋��E���擾
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_bossBS; }
	// �e�̋��E���擾
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	// �e�̋��E���ݒ�
	void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	// �v���C���[�̋��E���擾
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	// �v���C���[�̋��E���ݒ�
	void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) override { m_playerBS = playerBS; }
	// �}�g���N�X�擾
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }
	// �ʒu�擾
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }
	// �ʒu�ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// ���x�擾
	const DirectX::SimpleMath::Vector3& GetVelocity() override { return m_velocity; }
	// ��]�擾
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }
	// �X�P�[���擾
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// �X�P�[���ݒ�
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	// �N�H�[�^�j�I���擾
	DirectX::SimpleMath::Quaternion GetQuaternion() const { return m_quaternion; }
	// �N�H�[�^�j�I���ݒ�
	void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { m_quaternion = quaternion; }
	// �e�̔�ԕ����擾
	DirectX::SimpleMath::Vector3 GetBulletDirection() const { return m_bulletDirection; }
	// �e�̔�ԕ����ݒ�
	void SetBulletDirection(const DirectX::SimpleMath::Vector3& direction) { m_bulletDirection = direction; }
	// �v���C���[�̃|�C���^�[�擾
	Player* GetPlayer() const override { return m_pPlayer; }
	// �J�����̃|�C���^�[�擾
	FPS_Camera* GetCamera() const override { return m_pCamera; }
	// �J�����̃|�C���^�[�ݒ�
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	// �G��HP�擾
	int GetEnemyHP() const override { return m_currentHP; }
	// �G��HP�ݒ�(�_���[�W�K�p)
	void ApplyDamageToEnemy(int hp) override;
	// �G�̎��S�t���O�擾
	bool GetEnemyIsDead() const override { return m_isDead; }
	// �G�̎��S�t���O�ݒ�
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	// �v���C���[�Ƃ̓����蔻��擾
	bool GetHitToPlayer() const override { return m_isHitToPlayer; }
	// �v���C���[�Ƃ̓����蔻��ݒ�
	void SetHitToPlayer(bool isHitToPlayer) override { m_isHitToPlayer = isHitToPlayer; }
	// ���̓G�Ƃ̓����蔻��擾
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	// ���̓G�Ƃ̓����蔻��ݒ�
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	// �G�̒e���v���C���[�ɓ���������
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }
	// �G�̒e���v���C���[�ɓ����������ݒ�
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }
	// �G���v���C���[�̒e�ɓ���������
	bool GetEnemyHitByPlayerBullet() const override { return m_isEnemyHitByPlayerBullet; }
	// �G���v���C���[�̒e�ɓ����������ݒ�
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }
	// �v���C���[�ɗ^����_���[�W�擾
	float GetToPlayerDamage() const override { return m_bossDamage; }
	// �v���C���[�ɗ^����_���[�W�ݒ�
	void SetToPlayerDamage(float damage) { m_bossDamage = damage; }
	// �J�����̈ʒu�擾
	DirectX::SimpleMath::Vector3 GetCameraEye() const { return m_cameraEye; }
	// �J�����̃^�[�Q�b�g�擾
	DirectX::SimpleMath::Vector3 GetCameraTarget() const { return m_cameraTarget; }
	// �J�����̏�x�N�g���擾
	DirectX::SimpleMath::Vector3 GetCameraUp() const { return m_cameraUp; }
	// �V�[���h�W�J���̉��ʂ��擾
	float GetSheildSEVolume() const { return m_SEVolume - m_SEVolumeCorrection; }
	// �v���C���[��HP�����炷
	void SetPlayerHP(float& HP) const override { HP -= m_bossDamage; }
	// �e�̃N�[���_�E���ݒ�
	void SetBulletCooldown(float cooldown) { m_bulletCooldown = cooldown; }
	// ����U���̃N�[���_�E�������l�ݒ�
	void SetInitSpecialAttacCooldown(float cooldown) { m_initSpecialAttackCooldown = cooldown; }
	// �U���\���擾
	bool GetCanAttack() const override { return m_canAttack; }
	// �U���\���ݒ�
	void SetCanAttack(bool canAttack) override { m_canAttack = canAttack; }
	// �e�̊Ǘ��N���X�擾
	BulletManager* GetBulletManager() const override { return m_pBulletManager; }
	// �e�̊Ǘ��N���X�ݒ�
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }
	// �U�������擾
	bool GetIsAttack() const override { return m_isAttack; }
	// �U�������ݒ�
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }
	// �{�X�̎�ނ�ݒ�
	void SetBossType(BossType type) { m_bossType = type; }
	// �V�[���h�擾
	BossSheild* GetBossSheild() const { return m_pBossSheild.get(); }
	// AI�擾
	BossAI* GetBossAI() { return m_pBossAI.get(); }
	// HP�o�[�擾
	EnemyHPBar* GetHPBar() { return m_pHPBar.get(); }
	// �ʏ펞�̓����蔻��̔��a�擾
	float GetDefaultHitRadius() const { return m_defaultHitRadius; }
	// �ʏ펞�̓����蔻��̔��a�ݒ�
	void SetDefaultHitRadius(float radius) { m_defaultHitRadius = radius; }
	// �h�䎞�̓����蔻��̔��a�擾
	float GetDefensiveHitRadius() const { return m_defensiveHitRadius; }
	// �h�䎞�̓����蔻��̔��a�ݒ�
	void SetDefensiveHitRadius(float radius) { m_defensiveHitRadius = radius; }
	// �e�̃T�C�Y�擾
	float GetBulletSize() const { return m_bulletSize; }
	// �e�̃T�C�Y�ݒ�
	void SetBulletSize(float size) { m_bulletSize = size; }
	// �o���A�j��̃p�[�e�B�N���T�C�Y�擾
	float GetBarrierBreakSize() const { return m_barrierBreakSize; }
	// �o���A�̔j��p�[�e�B�N���T�C�Y�ݒ�
	void SetBarrierBreakSize(float size) { m_barrierBreakSize = size; }
	// ���S�G�t�F�N�g�̃T�C�Y�擾
	float GetDeadEffectSize() const { return m_deadEffectSize; }
	// ���S�G�t�F�N�g�̃T�C�Y�ݒ�
	void SetDeadEffectSize(float size) { m_deadEffectSize = size; }
public:// public�֐�
	// �R���X�g���N�^
	BossBase(Player* pPlayer, CommonResources* resources, int hp);
	// �f�X�g���N�^
	virtual ~BossBase();
	// ������
	void Initialize() override;
	// �X�V
	void Update(float elapsedTime) override;
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// �Փ˔���̕`��
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// �e�̃^�C�v�ݒ�
	void SetBulletType(BossBulletType bossBulletType) { m_bossBulletType = bossBulletType; }
	// �e�̃^�C�v�擾
	BossBulletType GetBulletType() const { return m_bossBulletType; }
	// �o���ASE�Đ�
	void PlayBarrierSE();
private:// private�֐�
	// �e����
	void ShootBullet();
	// �����̒e�𐶐�
	void CreateVerticalBullet();
	// �����e�𐶐�
	void CreateSpiralBullet();
private:// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// AI
	std::unique_ptr<BossAI> m_pBossAI;
	// ���f��
	std::unique_ptr<BossModel> m_pBossModel;
	// �V�[���h
	std::unique_ptr<BossSheild> m_pBossSheild;
	// HP�o�[
	std::unique_ptr<EnemyHPBar> m_pHPBar;
	// SE�̉���
	float m_SEVolume;
	// SE�̉��ʕ␳
	float m_SEVolumeCorrection;
	// �o���ASE�̍Đ��t���O
	bool m_isBarrierSEPlay;
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	// �J�����̃|�C���^�[
	FPS_Camera* m_pCamera;
	// �e�Ǘ��N���X
	BulletManager* m_pBulletManager;
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// ��]
	DirectX::SimpleMath::Vector3 m_rotate;
	// ��]�p�N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_quaternion;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// �}�g���N�X
	DirectX::SimpleMath::Matrix m_matrix;
	// �G�̋��E��
	DirectX::BoundingSphere m_bossBS;
	// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	DirectX::BoundingSphere m_enemyBSToPlayerArea;
	// �G�̒e�̋��E��
	DirectX::BoundingSphere m_enemyBulletBS;
	// �v���C���[�̋��E��
	DirectX::BoundingSphere m_playerBS;
	// �e�̔��ˈʒu
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	// �{�X���v���C���[�ɗ^����_���[�W
	float m_bossDamage;
	// ���ˈʒu����]�����邽�߂̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// Enemies�N���X�Őݒ肷��
	BossBulletType m_bossBulletType;
	// EnemyBullet�N���X�ɑ���
	BulletType m_bulletType;
	// �{�X�̎��
	BossType m_bossType;
	// ��������Boss
	std::unique_ptr<IBossLogic> m_pBoss;
	// �e�̔�ԕ���
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// ����
	float m_time;
	// �G�̗̑�
	int m_currentHP;
	// �G�̍ő�̗�
	int m_maxHP;
	// �G��HP��0�ɂȂ�����True
	bool m_isDead;
	// �v���C���[�Ƃ̔���
	bool m_isHitToPlayer;
	// ���̑��̓G�Ƃ̔���
	bool m_isHitToOtherEnemy;
	// �G���v���C���[�̒e�ɓ���������
	bool m_isEnemyHitByPlayerBullet;
	// �G�̒e���v���C���[�ɓ���������
	bool m_isPlayerHitByEnemyBullet;
	// �U���̃N�[���_�E��(�t���[�����Ƃɔ��˂��邱�Ƃ�h���p�j
	float m_attackCooldown;
	// ����U���̃N�[���_�E��
	float m_specialAttackCooldown;
	// ����U���̃N�[���_�E��(�������p)
	float m_initSpecialAttackCooldown;
	// �e�̃N�[���_�E��
	float m_bulletCooldown;
	// �U���\��
	bool m_canAttack;
	// �U������
	bool m_isAttack;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// �J�����̃^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// �J�����̏�x�N�g��
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �ʏ펞�̓����蔻��̔��a
	float m_defaultHitRadius;
	// �h�䎞�̓����蔻��̔��a
	float m_defensiveHitRadius;
	// �e�̃T�C�Y
	float m_bulletSize;
	// �o���A�̔j��p�[�e�B�N���T�C�Y
	float m_barrierBreakSize;
	// ���S�G�t�F�N�g�̃T�C�Y
	float m_deadEffectSize;
};