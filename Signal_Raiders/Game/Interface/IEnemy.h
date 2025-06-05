/*
*	@file IEnemy.h
*	@brief �G�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
#ifndef IENEMY_DEFINED
#define IENEMY_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/AudioManager/AudioManager.h"

// �O���錾
class Player;
class BulletManager;
class FPS_Camera;
class CommonResources;

// �G�̃C���^�[�t�F�[�X�N���X
class IEnemy
{
public:
	// �A�N�Z�T										  
	// �s����擾����
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() = 0;
	// ���݂̈ʒu���擾����
	virtual const DirectX::SimpleMath::Vector3& GetPosition() = 0;
	// �ʒu��ݒ肷��
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
	// �ړ����x���擾����
	virtual const DirectX::SimpleMath::Vector3& GetVelocity() = 0;
	// ��]�p�x���擾����
	virtual const DirectX::SimpleMath::Vector3& GetRotate() = 0;
	// �G�̌���HP���擾����
	virtual int GetEnemyHP() const = 0;
	// �G�Ƀ_���[�W��K�p����HP�����炷
	virtual void ApplyDamageToEnemy(int hp) = 0;
	// �G�����S���Ă��邩�ǂ������擾����
	virtual bool GetEnemyIsDead() const = 0;
	// �G�̎��S��Ԃ�ݒ肷��
	virtual void SetEnemyIsDead(bool isDead) = 0;
	// �v���C���[�ɓ����������ǂ������擾����
	virtual bool GetHitToPlayer() const = 0;
	// �v���C���[�ɓ����������ǂ�����ݒ肷��
	virtual void SetHitToPlayer(bool isHitToPlayer) = 0;
	// ���̓G�ɓ����������ǂ������擾����
	virtual bool GetHitToOtherEnemy() const = 0;
	// ���̓G�ɓ����������ǂ�����ݒ肷��
	virtual void SetHitToOtherEnemy(bool isHitToOtherEnemy) = 0;
	// �v���C���[��HP��ݒ肷��i�_���[�W���f�j
	virtual void SetPlayerHP(float& HP) const = 0;
	// �v���C���[���G�̒e�ɓ����������ǂ������擾����
	virtual bool GetPlayerHitByEnemyBullet() const = 0;
	// �v���C���[���G�̒e�ɓ����������ǂ�����ݒ肷��
	virtual void SetPlayerHitByEnemyBullet(bool hit) = 0;
	// �G���v���C���[�̒e�ɓ����������ǂ������擾����
	virtual bool GetEnemyHitByPlayerBullet() const = 0;
	// �G���v���C���[�̒e�ɓ����������ǂ�����ݒ肷��
	virtual void SetEnemyHitByPlayerBullet(bool hit) = 0;
	// �v���C���[�ɗ^����_���[�W�ʂ��擾����
	virtual float GetToPlayerDamage() const = 0;
	// �U���\�ȏ�Ԃ��ǂ������擾����
	virtual bool GetCanAttack() const = 0;
	// �U���\��Ԃ�ݒ肷��
	virtual void SetCanAttack(bool canAttack) = 0;
	// �U�������ǂ������擾����
	virtual bool GetIsAttack() const = 0;
	// �U�������ǂ�����ݒ肷��
	virtual void SetIsAttack(bool isAttack) = 0;
	// �G�̓����蔻��i���E���j���擾����
	virtual const DirectX::BoundingSphere& GetBoundingSphere() = 0;
	// �G�̒e�̓����蔻��i���E���j��ݒ肷��
	virtual void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) = 0;
	// �G�̒e�̓����蔻��i���E���j���擾����
	virtual const DirectX::BoundingSphere& GetBulletBoundingSphere() = 0;
	// �v���C���[�̓����蔻��i���E���j��ݒ肷��
	virtual void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) = 0;
	// �v���C���[�̓����蔻��i���E���j���擾����
	virtual const DirectX::BoundingSphere& GetPlayerBoundingSphere() = 0;
	// �v���C���[�擾
	virtual Player* GetPlayer()const = 0;
	// �J�����擾
	virtual FPS_Camera* GetCamera()const = 0;
	// �J�����ݒ�
	virtual void SetCamera(FPS_Camera* camera) = 0;
	// �e�Ǘ��N���X�擾
	virtual BulletManager* GetBulletManager()const = 0;
	// �e�Ǘ��N���X�ݒ�
	virtual void SetBulletManager(BulletManager* bulletManager) = 0;
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IEnemy() = default;
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update(float elapsedTime) = 0;
	// �`��
	virtual	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// �Փ˕`��
	virtual void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
};
#endif		// IENEMY_DEFINED
