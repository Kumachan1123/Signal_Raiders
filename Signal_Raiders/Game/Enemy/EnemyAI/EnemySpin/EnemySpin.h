/*
	@file	EnemySpin.h
	@brief	�G�X�s���N���X
*/
#pragma once
#ifndef ENEMY_SPIN_DEFINED
#define ENEMY_SPIN_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>  
#include <SimpleMath.h>
// �O�����C�u����
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ���색�C�u����
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"
//�O���錾
class EnemyAI;
class EnemySpin : public IState
{
public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// �ʒu�擾
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// �ʒu�ݒ�
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// ��]�擾
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// ��]�ݒ�
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; } // �X�P�[���ݒ�
public:
	// public�����o�֐�
	EnemySpin(EnemyAI* enemyAI);// �R���X�g���N�^
	~EnemySpin();// �f�X�g���N�^
	void Initialize() override;// ������
	void Update(float elapsedTime) override;// �X�V
private:
	EnemyAI* m_enemyAI;//�GAI
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Vector3 m_initialPosition;// ���W�����l
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x
	float m_angle;//�p�x
	float  m_time;  // ���Ԃ̏�����
};
#endif //ENEMY_SPIN_DEFINED