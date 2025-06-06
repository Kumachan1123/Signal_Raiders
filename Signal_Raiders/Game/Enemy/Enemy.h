/*
*	@file	Enemy.h
*	@brief	�G�N���X
*/
#pragma once
#ifndef ENEMY_DEFINED
#define ENEMY_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "Game/KumachiLib/KumachiLib.h"
#include <Game/KumachiLib/DrawCollision/DrawCollision.h>
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IEnemy.h"

//�O���錾
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class EnemyManager;
class FPS_Camera;

// �G�N���X
class Enemy : public IEnemy
{
public:
	// �A�N�Z�T
	// �G�̓����蔻����擾����
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	// �G�̒e�̓����蔻����擾����
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	// �G�̒e�̓����蔻���ݒ肷��
	void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	// �v���C���[�̓����蔻����擾����
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	// �v���C���[�̓����蔻���ݒ肷��
	void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) override { m_playerBS = playerBS; }
	// �G�̃��[���h�s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }
	// �G�̌��݂̍��W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }
	// �G�̍��W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// �G�̈ړ����x���擾����
	const DirectX::SimpleMath::Vector3& GetVelocity() override { return m_velocity; }
	// �G�̉�]�p�x���擾����
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }
	// �ΏۂƂȂ�v���C���[�̃|�C���^���擾����
	Player* GetPlayer() const override { return m_pPlayer; }
	// �g�p����J�����̃|�C���^���擾����
	FPS_Camera* GetCamera() const override { return m_pCamera; }
	// �J�����̃|�C���^��ݒ肷��
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	// �G�̌���HP���擾����
	int GetEnemyHP() const override { return m_currentHP; }
	// �G�����S��Ԃ��ǂ������擾����
	bool GetEnemyIsDead() const override { return m_isDead; }
	// �G�̎��S��Ԃ�ݒ肷��
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	// �v���C���[�ɓ����������ǂ������擾����
	bool GetHitToPlayer() const override { return m_isHitToPlayer; }
	// �v���C���[�ɓ����������ǂ�����ݒ肷��
	void SetHitToPlayer(bool isHitToPlayer) override { m_isHitToPlayer = isHitToPlayer; }
	// ���̓G�ɓ����������ǂ������擾����
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	// ���̓G�ɓ����������ǂ�����ݒ肷��
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	// �G�̒e���v���C���[�ɖ����������擾����
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }
	// �G�̒e���v���C���[�ɖ����������ݒ肷��
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }
	// �v���C���[�̒e���G�ɖ����������擾����
	bool GetEnemyHitByPlayerBullet() const override { return m_isEnemyHitByPlayerBullet; }
	// �v���C���[�̒e���G�ɖ����������ݒ肷��
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }
	// �v���C���[�ɗ^����_���[�W�ʂ��擾����
	float GetToPlayerDamage() const override { return EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// ���ݍU���\�ȏ�Ԃ��ǂ������擾����
	bool GetCanAttack() const override { return m_canAttack; }
	// �U���\��Ԃ��ǂ�����ݒ肷��
	void SetCanAttack(bool canAttack) override { m_canAttack = canAttack; }
	// �G�Ƀ_���[�W��^����HP�����炷
	void ApplyDamageToEnemy(int hp) override { m_currentHP -= hp; }
	// �v���C���[��HP�Ƀ_���[�W�𔽉f����
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// �e�̊Ǘ��N���X���擾����
	BulletManager* GetBulletManager() const override { return m_pBulletManager; }
	// �e�̊Ǘ��N���X��ݒ肷��
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }
	// �G�����ݍU�������ǂ������擾����
	bool GetIsAttack() const override { return m_isAttack; }
	// �G���U�������ǂ�����ݒ肷��
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }
public:
	//	public�֐�
	// �R���X�g���N�^
	Enemy(Player* pPlayer, CommonResources* resources, int hp);
	// �f�X�g���N�^
	~Enemy();
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
	void ShootBullet(float elapsedTime);
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �G�̃��f��
	std::unique_ptr<EnemyModel>		m_pEnemyModel;
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
	// �G�̉�]�p�x
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
	//�G�̗̑�
	int m_currentHP;
	//�G��HP��0�ɂȂ�����True
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
#endif // ENEMY_DEFINED