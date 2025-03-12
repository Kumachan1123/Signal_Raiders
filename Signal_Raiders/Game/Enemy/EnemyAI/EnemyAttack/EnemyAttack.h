/*
	@file	EnemyAttack.h
	@brief	�GAttack�N���X
*/

#pragma once
#ifndef ENEMY_ATTACK_DEFINED
#define ENEMY_ATTACK_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <cassert>
#include <random>  
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ����w�b�_�[�t�@�C��
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
public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// �ʒu���擾
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// �ʒu��ݒ�
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// ��]���擾
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// ��]��ݒ�
	float GetCoolTime() const { return m_attackCooldown; }// �N�[���^�C�����擾
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }// �N�[���^�C����ݒ�
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }// �T�C�Y��ݒ�
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }// ��]���x��ݒ�
public:
	// public�����o�֐�
	EnemyAttack(EnemyAI* enemyAI);// �R���X�g���N�^
	~EnemyAttack();	// �f�X�g���N�^
	void Initialize() override;// ����������
	void Update(float elapsedTime) override;// �X�V����
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �GAI
	EnemyAI* m_enemyAI;
	// �ړ�
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �T�C�Y
	DirectX::SimpleMath::Vector3 m_scale;
	// ��]���x
	float m_rotationSpeed;
	// �U���̃N�[���_�E���^�C��
	float m_attackCooldown;
};
#endif //ENEMY_ATTACK_DEFINED
