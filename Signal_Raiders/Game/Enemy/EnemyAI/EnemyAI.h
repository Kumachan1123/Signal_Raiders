/*
	@file	EnemyAI.h
	@brief	�GAI�N���X
*/
#pragma once
#ifndef ENEMY_AI_DEFINED
#define ENEMY_AI_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>
#include <type_traits>
#include <SimpleMath.h>
// �O�����C�u����
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ���색�C�u����
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAttack/EnemyAttack.h"
#include "Game/Enemy/EnemyAI/EnemyIdling/EnemyIdling.h"
#include "Game/Enemy/EnemyAI/EnemySpin/EnemySpin.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Player/Player.h"
//�O���錾
class CommonResources;
class EnemyAttack;
class EnemySpin;
class EnemyIdling;
class IEnemy;
class EnemyAI
{
public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// �ʒu�擾
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; } // �ʒu�ݒ�
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; } // �����ʒu�擾
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; } // ��]�擾
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; } // ��]�ݒ�
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; } // ���x�擾
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; } // ���x�ݒ�
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; } // �X�P�[���擾
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; } // �X�P�[���ݒ�
	IState::EnemyState GetState()const { return m_enemyState; } // ��Ԏ擾
	void SetState(IState::EnemyState state) { m_enemyState = state; }// ��Ԑݒ�
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; } // �U���\��
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; } // �v���C���[�̒e�ɓ���������
	EnemyAttack* GetEnemyAttack()const { return m_pEnemyAttack.get(); }// �U���N���X�擾
	EnemyIdling* GetEnemyIdling()const { return m_pEnemyIdling.get(); } // �p�j�N���X�擾
	EnemySpin* GetEnemySpin()const { return m_pEnemySpin.get(); } // �����N���X�擾
	IEnemy* GetEnemy()const { return m_pEnemy; } // �G�擾
	IState* GetNowState()const { return m_pCurrentState; } // ���݂̏�Ԏ擾
	bool GetisAttack()const { return m_pEnemy->GetIsAttack(); } // �U������
	void SetIsAttack(bool isAttack) { m_pEnemy->SetIsAttack(isAttack); } // �U�����ɂ���
public:
	// public�����o�֐�
	EnemyAI(IEnemy* pEnemy);// �R���X�g���N�^
	~EnemyAI();// �f�X�g���N�^
	void Initialize();// ������
	void Update(float elapsedTime); // �X�V
	void ChangeState(IState* newState); // ��ԕύX
private:
	// private�����o�֐�
	void KnockBack(float elapsedTime); // �m�b�N�o�b�N
private:
	// private�����o�ϐ�
	std::unique_ptr<EnemyIdling> m_pEnemyIdling;	// ���펞
	std::unique_ptr<EnemyAttack> m_pEnemyAttack;	// �U����
	std::unique_ptr<EnemySpin> m_pEnemySpin;	// �X�s��
	IState* m_pCurrentState;	// ���݂̏��
	IState::EnemyState m_enemyState;// �G�̏��
	IEnemy* m_pEnemy;	// �G�̃|�C���^�[
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Vector3 m_initialPosition;// ���W�����l
	DirectX::SimpleMath::Vector3 m_bsPosition;//�����蔻��p���W
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x
	float m_knockTime;// �m�b�N�o�b�N���鎞��
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	float  m_time;  // ���Ԃ̏�����
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��
	bool m_isHitPlayerBullet;// �v���C���[�̒e�ɓ���������
	bool m_canAttack;// �U���\��
	bool m_isAttack;// �U������
};
#endif //ENEMY_AI_DEFINED
