/*
*	@file	EnemySpin.h
*	@brief	�G�X�s���N���X
*/
#pragma once
#ifndef ENEMY_SPIN_DEFINED
#define ENEMY_SPIN_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>  
#include <SimpleMath.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ���색�C�u����
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"

//�O���錾
class EnemyAI;

// �G�X�s���N���X
class EnemySpin : public IState
{
public:
	// �A�N�Z�T
	// ���݂̈ʒu���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// �ʒu��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// ���݂̉�]���擾����
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// ��]��ݒ肷��
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// ���f���̃X�P�[����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
public:
	// public�����o�֐�
	// �R���X�g���N�^
	EnemySpin(EnemyAI* enemyAI);
	// �f�X�g���N�^
	~EnemySpin();
	// ������
	void Initialize() override;
	// �X�V
	void Update(float elapsedTime) override;
private:
	// private�����o�֐�
	// �X�s���̍X�V
	void UpdateSpin(float elapsedTime);
	// �m�b�N�o�b�N�̍X�V
	void UpdateKnockBack(float elapsedTime);
private:
	// private�����o�ϐ�
	// �GAI
	EnemyAI* m_pEnemyAI;
	// ���݂̈ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// �����ʒu
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �m�b�N�o�b�N�J�n�ʒu
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// �m�b�N�o�b�N�I���ʒu
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// �m�b�N�o�b�N�̏������x
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	// ��]�p�x
	float m_angle;
	// �o�ߎ���
	float m_time;
	// �m�b�N�o�b�N�̎�������
	float m_knockTime;
};
#endif //ENEMY_SPIN_DEFINED