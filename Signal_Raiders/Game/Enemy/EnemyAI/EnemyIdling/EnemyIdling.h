/*
	@file	EnemyIdling.h
	@brief	�G�p�j�N���X
*/
#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>  
#include <SimpleMath.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/Interface/IState.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/KumachiLib.h"

//�O���錾
class EnemyAI;

// �G�p�j�N���X
class EnemyIdling : public IState
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
	EnemyIdling(EnemyAI* enemyAI);
	// �f�X�g���N�^
	~EnemyIdling();
	// ������
	void Initialize() override;
	// �X�V
	void Update(float elapsedTime) override;
private:
	// private�ϐ�
	// �GAI�ւ̃|�C���^
	EnemyAI* m_pEnemyAI;
	// ���݂̈ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// �����ʒu
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �ړ����x�x�N�g��
	DirectX::SimpleMath::Vector3 m_velocity;
	// ��]���x
	float m_rotationSpeed;
	// �o�ߎ��Ԃ̊Ǘ��p�ϐ�
	float m_time;

};
#endif //ENEMY_IDLING_DEFINED