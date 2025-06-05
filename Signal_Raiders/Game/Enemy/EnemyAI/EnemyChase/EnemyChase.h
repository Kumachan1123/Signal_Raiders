/*
*	@file	EnemyChase.h
*	@brief	�G�ǐՃN���X
*/
#pragma once
#ifndef ENEMY_CHASE_DEFINED
#define ENEMY_CHASE_DEFINED
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
class EnemyChase : public IState
{
public:// �A�N�Z�T
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
public:// public�����o�֐�
	// �R���X�g���N�^
	EnemyChase(EnemyAI* enemyAI);
	// �f�X�g���N�^
	~EnemyChase();
	// ������
	void Initialize() override;
	// �X�V
	void Update(float elapsedTime) override;
private:// �萔
	// ��~����
	const float STOP_DISTANCE = 10.0f;
private:// private�����o�ϐ�
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
	// ��]�p�x
	float m_angle;
	// �o�ߎ���
	float m_time;
};
#endif //ENEMY_CHASE_DEFINED