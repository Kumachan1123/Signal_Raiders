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
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"

//�O���錾
class CommonResources;
class BossAI;

class BossAttack : public IState
{
public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// ���W�擾
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// ���W�ݒ�
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// ��]�擾
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// ��]�ݒ�
	float GetCoolTime() const { return m_attackCooldown; }// �N�[���^�C���擾
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }// �N�[���^�C���ݒ�
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }// �X�P�[���ݒ�
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }// ��]���x�ݒ�
public:
	// public�����o�֐�
	BossAttack(BossAI* pBoss);// �R���X�g���N�^
	~BossAttack();// �f�X�g���N�^
	void Initialize() override;// ������
	void Update(float elapsedTime) override;// �X�V
private:
	// private�����o�֐�
	void RotateTowardsPlayer(DirectX::SimpleMath::Vector3 playerPos);// �v���C���[�̕����ɉ�]
	void MoveTowardsPlayer(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos);// �v���C���[�̕����Ɉړ�
	void ManageAttackCooldown(float elapsedTime);// �U���N�[���_�E���̊Ǘ�
private:
	// private�萔
	const float ROTATION_SPEED = 5.5f;// ��]���x
	const float RANDOM_MAX = 2.0f;// �����_���̍ő�l
	const float RANDOM_MIN = 0.5f;// �����_���̍ŏ��l
	const float ATTACK_INTERVAL = 1.0f; // �U���Ԋu
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
