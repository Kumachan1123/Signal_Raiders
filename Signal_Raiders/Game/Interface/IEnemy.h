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
class IEnemy
{
protected:
	// protected�ϐ�
	Player* m_pPlayer;// �v���C���[
	BulletManager* m_pBulletManager;// �e�Ǘ��N���X
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	int m_currentHP;// �G��HP
public:
	// �A�N�Z�T										  
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() = 0;// �s��擾
	virtual const  DirectX::SimpleMath::Vector3& GetPosition() = 0;// �ʒu�擾
	virtual void SetPosition(DirectX::SimpleMath::Vector3& pos) = 0;// �ʒu�ݒ�
	virtual const DirectX::SimpleMath::Vector3& GetVelocity() = 0;// ���x�擾
	virtual const DirectX::SimpleMath::Vector3& GetRotate() = 0;// ��]�擾
	virtual int GetEnemyHP() const = 0;// HP�擾
	virtual void SetEnemyMaxHP(int hp) = 0;// HP�ݒ�
	virtual bool GetEnemyIsDead() const = 0;// ���S�t���O�擾
	virtual void SetEnemyIsDead(bool isDead) = 0;// ���S�t���O�ݒ�
	virtual bool GetHitToPlayer()const = 0;// �v���C���[�ɓ����������擾
	virtual void SetHitToPlayer(bool isHitToPlayer) = 0;// �v���C���[�ɓ����������ݒ�
	virtual bool GetHitToOtherEnemy() const = 0;// ���̓G�ɓ����������擾
	virtual void SetHitToOtherEnemy(bool isHitToOtherEnemy) = 0;// ���̓G�ɓ����������ݒ�
	virtual void SetPlayerHP(float& HP) const = 0;// �v���C���[��HP�ݒ�
	virtual bool GetPlayerHitByEnemyBullet() const = 0;// �v���C���[���G�̒e�ɓ����������擾
	virtual void SetPlayerHitByEnemyBullet(bool hit) = 0;// �v���C���[���G�̒e�ɓ����������ݒ�
	virtual bool GetEnemyHitByPlayerBullet()const = 0;// �G���v���C���[�̒e�ɓ����������擾
	virtual void SetEnemyHitByPlayerBullet(bool hit) = 0;// �G���v���C���[�̒e�ɓ����������ݒ�
	virtual float GetToPlayerDamage() const = 0;// �v���C���[�ɗ^����_���[�W�擾
	virtual bool GetCanAttack() const = 0;// �U���\���擾
	virtual void SetCanAttack(bool canAttack) = 0;// �U���\���ݒ�
	virtual bool GetIsAttack() const = 0;// �U�������擾
	virtual void SetIsAttack(bool isAttack) = 0;// �U�������ݒ�
	virtual const DirectX::BoundingSphere& GetBoundingSphere() = 0;// ���E���擾
	virtual void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) = 0;// ���E���ݒ�
	virtual const DirectX::BoundingSphere& GetBulletBoundingSphere() = 0;// �e�̋��E���擾
	virtual void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) = 0;// �v���C���[�̋��E���ݒ�
	virtual const DirectX::BoundingSphere& GetPlayerBoundingSphere() = 0;// �v���C���[�̋��E���擾
	virtual Player* GetPlayer()const = 0;// �v���C���[�擾
	virtual FPS_Camera* GetCamera()const = 0;// �J�����擾
	virtual void SetCamera(FPS_Camera* camera) = 0;// �J�����ݒ�
	virtual BulletManager* GetBulletManager()const = 0;// �e�Ǘ��N���X�擾
	virtual void SetBulletManager(BulletManager* bulletManager) = 0;// �e�Ǘ��N���X�ݒ�

public:
	// public�֐�
	// �R���X�g���N�^
	IEnemy(Player* pPlayer, CommonResources* resources, int hp)
		: m_pPlayer(pPlayer)// �v���C���[
		, m_commonResources(resources)// ���ʃ��\�[�X
		, m_currentHP(hp)// �G��HP
		, m_pBulletManager(nullptr) // �e�Ǘ��N���X
	{}
	virtual ~IEnemy() = default;// �f�X�g���N�^
	virtual void Initialize() = 0;// ������
	virtual void Update(float elapsedTime) = 0;// �X�V
	virtual	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// �`��
	virtual void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// �Փ˕`��


};
#endif		// IENEMY_DEFINED
