/*
	@file	Enemy.h
	@brief	�G�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <SimpleMath.h>
#include <random>
#include <memory>
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/Microsoft/ReadData.h"
// ����w�b�_�[�t�@�C��
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
class Enemy : public IEnemy
{
public:
	// �A�N�Z�T
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }// �G�̋��E���擾
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }// �G�̒e�̋��E���擾
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs)override { m_enemyBulletBS = bs; }// �G�̒e�̋��E���ݒ�
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }// �v���C���[�̋��E���擾
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS)override { m_playerBS = playerBS; }// �v���C���[�̋��E���ݒ�
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }// �}�g���N�X�擾
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }// ���W�擾
	void SetPosition(DirectX::SimpleMath::Vector3& pos)override { m_position = pos; }// ���W�ݒ�
	const DirectX::SimpleMath::Vector3& GetVelocity()override { return m_velocity; }// ���x�擾
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }// ��]�擾
	Player* GetPlayer()const override { return m_pPlayer; }// �v���C���[�擾
	FPS_Camera* GetCamera()const override { return m_pCamera; }// �J�����擾
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }// �J�����ݒ�
	int GetEnemyHP() const override { return m_currentHP; }// �G��HP�擾
	bool GetEnemyIsDead() const override { return m_isDead; }// ���S�t���O�擾
	void SetEnemyIsDead(bool isDead)override { m_isDead = isDead; }// ���S�t���O�ݒ�
	bool GetHitToPlayer()const override { return m_isHitToPlayer; }// �v���C���[�Ƃ̓����蔻��擾
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }// �v���C���[�Ƃ̓����蔻��ݒ�
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }// ���̑��̓G�Ƃ̓����蔻��擾
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }// ���̑��̓G�Ƃ̓����蔻��ݒ�
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }// �G�̒e���v���C���[�ɓ����������擾
	void SetPlayerHitByEnemyBullet(bool hit)override { m_isPlayerHitByEnemyBullet = hit; }// �G�̒e���v���C���[�ɓ����������ݒ�
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }// �G���v���C���[�̒e�ɓ����������擾
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }// �G���v���C���[�̒e�ɓ����������ݒ�
	float GetToPlayerDamage() const override { return EnemyParameters::NORMAL_ENEMY_DAMAGE; }// �v���C���[�ɗ^����_���[�W�擾
	bool GetCanAttack() const override { return m_canAttack; }// �U���\���擾
	void SetCanAttack(bool canAttack)override { m_canAttack = canAttack; }// �U���\���ݒ�
	void SetEnemyMaxHP(int hp)override { m_currentHP -= hp; }// �G��HP�ݒ�
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::NORMAL_ENEMY_DAMAGE; }// �v���C���[��HP�ݒ�
	BulletManager* GetBulletManager()const override { return m_pBulletManager; }// �e�Ǘ��N���X�擾
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }// �e�Ǘ��N���X�ݒ�
	bool GetIsAttack() const override { return m_isAttack; }// �U�������擾
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }// �U�������ݒ�
public:
	//	public�֐�
	Enemy(Player* pPlayer, CommonResources* resources, int hp);// �R���X�g���N�^
	~Enemy();// �f�X�g���N�^
	void Initialize() override;// ������
	void Update(float elapsedTime) override;// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;// �`��
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;// �����蔻��`��
private:
	// private�֐�
	void ShootBullet();// �e������
private:
	// �����o�ϐ�
	CommonResources* m_commonResources;	// ���ʃ��\�[�X
	std::unique_ptr<EnemyModel>		m_enemyModel;// �G�̃��f��
	std::unique_ptr<EnemyAI>		m_enemyAI;// �G��AI
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// �G��HP�o�[
	Player* m_pPlayer;	// �v���C���[�̃|�C���^�[
	FPS_Camera* m_pCamera;	// �J�����̃|�C���^�[
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
	bool m_isAttack;// �U������
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��


};