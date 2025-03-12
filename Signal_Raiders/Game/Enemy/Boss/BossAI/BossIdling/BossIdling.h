/*
	@file	BossIdling.h
	@brief	�{�X�p�j�N���X
*/
#pragma once
#ifndef BOSS_IDLING_DEFINED
#define BOSS_IDLING_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <cassert>
#include <random>
#include <cmath>
// �O�����C�u����
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"

//�O���錾
class BossAI;

class BossIdling : public IState
{
private:
	// �GAI
	BossAI* m_pBoss;
	// �ړ�
	DirectX::SimpleMath::Vector3 m_position;
	// ���W�����l
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// �T�C�Y
	DirectX::SimpleMath::Vector3 m_scale;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// ��]���x
	float m_rotationSpeed;
	// ���Ԃ̏�����
	float  m_time;
	// �p�x
	float m_angle;

public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// ���W�擾
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// ���W�ݒ�
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// ��]�擾
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// ��]�ݒ�
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }// �X�P�[���ݒ�
public:
	// public�����o�֐�
	BossIdling(BossAI* pBoss);// �R���X�g���N�^
	virtual ~BossIdling();// �f�X�g���N�^
	void Initialize() override;// ������
	void Update(float elapsedTime) override;// �X�V
};
#endif //BOSS_IDLING_DEFINED