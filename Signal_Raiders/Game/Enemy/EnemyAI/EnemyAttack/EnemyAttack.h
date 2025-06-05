/*
*	@file	EnemyAttack.h
*	@brief	�GAttack�N���X
*/
#pragma once
#ifndef ENEMY_ATTACK_DEFINED
#define ENEMY_ATTACK_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <cassert>
#include <random>  
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
//�O���錾
class CommonResources;
class EnemyAI;
class EnemyAttack : public IState
{
public:	// �A�N�Z�T
	// ���݂̈ʒu���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// �ʒu��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// ���݂̉�]���擾����
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// ��]��ݒ肷��
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// �U���̃N�[���^�C�����擾����
	float GetCoolTime() const { return m_attackCooldown; }
	// �U���̃N�[���^�C����ݒ肷��
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }
	// ���f���̃X�P�[����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// ��]�X�s�[�h��ݒ肷��
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
public:// public�����o�֐�
	// �R���X�g���N�^
	EnemyAttack(EnemyAI* enemyAI);
	// �f�X�g���N�^
	~EnemyAttack();
	// ����������
	void Initialize() override;
	// �X�V����
	void Update(float elapsedTime) override;
private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �GAI
	EnemyAI* m_pEnemyAI;
	// �ړ�
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �����ʒu
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// �ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �T�C�Y
	DirectX::SimpleMath::Vector3 m_scale;
	// ��]���x
	float m_rotationSpeed;
	// �U���̃N�[���_�E���^�C��
	float m_attackCooldown;
	// ����
	float m_time;
};
#endif //ENEMY_ATTACK_DEFINED
