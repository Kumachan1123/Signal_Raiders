/*
	@file	VerticalAttacker.h
	@brief	�����U���G�N���X
*/
#pragma once
#ifndef VERTICAL_ATTACKER_DEFINED
#define VERTICAL_ATTACKER_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>
#include <memory>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IEnemy.h"
#include "Game/KumachiLib/KumachiLib.h"
#include <Game/KumachiLib/DrawCollision/DrawCollision.h>
#include "Game/Enemy/VerticalAttackerModel/VerticalAttackerModel.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"

//�O���錾
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class VerticalAttackerModel;
class EnemyManager;
class FPS_Camera;

// �����U���G�N���X
class VerticalAttacker : public IEnemy
{
public:
	// �A�N�Z�T
	// �G�̋��E���擾
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	// �G�̒e�̋��E���擾
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	// �G�̒e�̋��E���ݒ�
	void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	// �v���C���[�̋��E���擾
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	// �v���C���[�̋��E���ݒ�
	void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) override { m_playerBS = playerBS; }
	// �}�g���N�X�擾
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }
	// ���W�擾
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }
	// ���W�ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// ���x�擾
	const DirectX::SimpleMath::Vector3& GetVelocity() override { return m_velocity; }
	// ��]�擾
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }
	// �v���C���[�擾
	Player* GetPlayer() const override { return m_pPlayer; }
	// �J�����擾
	FPS_Camera* GetCamera() const override { return m_pCamera; }
	// �J�����ݒ�
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	// �G��HP�擾
	int GetEnemyHP() const override { return m_currentHP; }
	// ���S�t���O�擾
	bool GetEnemyIsDead() const override { return m_isDead; }
	// ���S�t���O�ݒ�
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	// �v���C���[�Ƃ̓����蔻��擾
	bool GetHitToPlayer() const override { return m_isHitToPlayer; }
	// �v���C���[�Ƃ̓����蔻��ݒ�
	void SetHitToPlayer(bool isHitToPlayer) override { m_isHitToPlayer = isHitToPlayer; }
	// ���̑��̓G�Ƃ̓����蔻��擾
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	// ���̑��̓G�Ƃ̓����蔻��ݒ�
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	// �G�̒e���v���C���[�ɓ����������擾
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }
	// �G�̒e���v���C���[�ɓ����������ݒ�
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }
	// �G���v���C���[�̒e�ɓ����������擾
	bool GetEnemyHitByPlayerBullet() const override { return m_isEnemyHitByPlayerBullet; }
	// �G���v���C���[�̒e�ɓ����������ݒ�
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }
	// �v���C���[�ɗ^����_���[�W�擾
	float GetToPlayerDamage() const override { return EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// �U���\���擾
	bool GetCanAttack() const override { return m_canAttack; }
	// �U���\���ݒ�
	void SetCanAttack(bool canAttack) override { m_canAttack = canAttack; }
	// �G��HP�ݒ�(�_���[�W�K�p)
	void ApplyDamageToEnemy(int hp) override { m_currentHP -= hp; }
	// �v���C���[��HP�ݒ�
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// �e�Ǘ��N���X�擾
	BulletManager* GetBulletManager() const override { return m_pBulletManager; }
	// �e�Ǘ��N���X�ݒ�
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }
	// �U�������擾
	bool GetIsAttack() const override { return m_isAttack; }
	// �U�������ݒ�
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }
public:
	// public�֐�
	// �R���X�g���N�^
	VerticalAttacker(Player* pPlayer, CommonResources* resources, int hp);
	// �f�X�g���N�^
	~VerticalAttacker();
	// ������
	void Initialize() override;
	// �X�V
	void Update(float elapsedTime) override;
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// �����蔻��`��
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
private:
	// private�֐�
	// �e������
	void ShootBullet();
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �G�̃��f��
	std::unique_ptr<VerticalAttackerModel>	m_pVerticalAttackerModel;
	// �G��AI
	std::unique_ptr<EnemyAI>		m_pEnemyAI;
	// �G��HP�o�[
	std::unique_ptr<EnemyHPBar>		m_pHPBar;
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	// �J�����̃|�C���^�[
	FPS_Camera* m_pCamera;
	// �e�Ǘ��N���X
	BulletManager* m_pBulletManager;
	// �G�̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �G�̑��x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �G�̉�]
	DirectX::SimpleMath::Vector3 m_rotate;
	// �G�̋��E��
	DirectX::BoundingSphere m_enemyBS;
	// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	DirectX::BoundingSphere m_enemyBSToPlayerArea;
	// �G�̒e�̋��E��
	DirectX::BoundingSphere m_enemyBulletBS;
	// �v���C���[�̋��E��
	DirectX::BoundingSphere m_playerBS;
	// �}�g���N�X
	DirectX::SimpleMath::Matrix m_matrix;
	// �G�̗̑�
	int m_currentHP;
	// ���S�t���O
	bool m_isDead;
	// �v���C���[�Ƃ̔���
	bool m_isHitToPlayer;
	// ���̑��̓G�Ƃ̔���
	bool m_isHitToOtherEnemy;
	// �G���v���C���[�̒e�ɓ���������
	bool m_isEnemyHitByPlayerBullet;
	// �G�̒e���v���C���[�ɓ���������
	bool m_isPlayerHitByEnemyBullet;
	// �U���\��
	bool m_canAttack;
	// �U������
	bool m_isAttack;
	// �U���̃N�[���_�E���^�C��
	float m_attackCooldown;
};
#endif // VERTICAL_ATTACKER_DEFINED

