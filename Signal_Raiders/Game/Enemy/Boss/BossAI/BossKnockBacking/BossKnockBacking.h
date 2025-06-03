/*
*	@file	BossKnockBacking.h
*	@brief	�{�X�̃m�b�N�o�b�N���̏����N���X
*/
#pragma once
#ifndef BOSS_KNOCKBACKING_DEFINED
#define BOSS_KNOCKBACKING_DEFINED
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
class BossAI;
class BossKnockBacking : public IState
{
public:	// �A�N�Z�T
	// ���W�擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// ���W�ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// ��]�擾
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// ��]�ݒ�
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// �X�P�[���ݒ�
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
public:	// public�����o�֐�
	// �R���X�g���N�^
	BossKnockBacking(BossAI* pBoss);
	// �f�X�g���N�^
	virtual ~BossKnockBacking();
	// ������
	void Initialize() override;
	// �X�V
	void Update(float elapsedTime) override;
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
	// �m�b�N�o�b�N����
	float m_knockTime;
	// �m�b�N�o�b�N�̊J�n�ʒu 
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// �m�b�N�o�b�N�̏I���ʒu
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// �m�b�N�o�b�N�̏����x
	DirectX::SimpleMath::Vector3 m_initialVelocity;
};
#endif //BOSS_KNOCKBACKING_DEFINED