/*
*	@file	BossAttack.h
*	@brief	�{�X�U���N���X
*/
#pragma once
#ifndef BOSS_ATTACK_DEFINED
#define BOSS_ATTACK_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>
#include <cmath>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"

//�O���錾
class CommonResources;
class BossAI;

// �{�X�U���N���X
class BossAttack : public IState
{
public:
	// �A�N�Z�T
	// ���W�擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// ���W�ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// ��]�擾
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// ��]�ݒ�
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// �N�[���^�C���擾
	float GetCoolTime() const { return m_attackCooldown; }
	// �N�[���^�C���ݒ�
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }
	// �X�P�[���ݒ�
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// ��]���x�ݒ�
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
public:
	// public�����o�֐�
	// �R���X�g���N�^
	BossAttack(BossAI* pBoss);
	// �f�X�g���N�^
	~BossAttack();
	// ������
	void Initialize() override;
	// �X�V
	void Update(float elapsedTime) override;
private:
	// private�����o�֐�
	// �v���C���[�̕����ɉ�]
	void RotateTowardsPlayer(const DirectX::SimpleMath::Vector3& playerPos);
	// �v���C���[�̕����Ɉړ�
	void MoveTowardsPlayer(float elapsedTime, const DirectX::SimpleMath::Vector3& playerPos);
	// �U���N�[���_�E���̊Ǘ�
	void ManageAttackCooldown(float elapsedTime);
private:
	// private�萔
	// ��]���x
	const float ROTATION_SPEED = 5.5f;
	// �����_���̍ő�l
	const float RANDOM_MAX = 2.0f;
	// �����_���̍ŏ��l
	const float RANDOM_MIN = 0.5f;
	// �U���Ԋu
	const float ATTACK_INTERVAL = 1.0f;
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �GAI
	BossAI* m_pBoss;
	// �ړ�
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �T�C�Y
	DirectX::SimpleMath::Vector3 m_scale;
	// �U���̃N�[���_�E���^�C��
	float m_attackCooldown;
	// ��]���x
	float m_rotationSpeed;
};
#endif //BOSS_ATTACK_DEFINED
