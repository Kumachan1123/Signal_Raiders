/*
*	@file	BossAI.h
*	@brief	�{�XAI�N���X
*/
#pragma once
#ifndef BOSS_AI_DEFINED
#define BOSS_AI_DEFINED
// �W�����C�u����
#include <cassert>
#include <random>
#include <type_traits> // std::enable_if, std::is_integral
// DirectX
#include <SimpleMath.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/Boss/BossAI/BossAttack/BossAttack.h"
#include "Game/Enemy/Boss/BossAI/BossKnockBacking/BossKnockBacking.h"
#include "Game/Player/Player.h"
#include "Game/Interface/IState.h"
//�O���錾
class CommonResources;
class BossAttack;
class BossKnockBacking;
class BossAI
{
public:	// �A�N�Z�T
	// �����ʒu���擾
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	// �ʒu���擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// �ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// ��]���擾
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// ��]��ݒ�
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// ���x���擾
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// ���x��ݒ�
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	// �X�P�[�����擾
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// �X�P�[����ݒ�
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// �U�����̏�Ԃ��擾
	BossAttack* GetBossAttack()const { return m_pBossAttack.get(); }
	// �G���擾
	IEnemy* GetEnemy()const { return m_pBoss; }
	// ���݂̏�Ԃ��擾
	IState* GetNowState()const { return m_pCurrentState; }
	// ��Ԃ��擾
	IState::EnemyState GetState()const { return m_enemyState; }
	// ��Ԃ�ݒ�
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	// �v���C���[�̒e�ɓ����������ݒ�
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
	// �U�������擾
	bool GetIsAttack()const { return m_pBoss->GetIsAttack(); }
	// �U�������ݒ�
	void SetIsAttack(bool attack) { m_pBoss->SetIsAttack(attack); }
	// �U�����̕\������擾
	IState::EnemyState GetAttackState() const { return m_attackState; }
	// �U�����̕\�����ݒ�
	void SetAttackState(IState::EnemyState state) { m_attackState = state; }
	// �m�b�N�o�b�N�������������擾
	bool GetIsKnockBack() const { return m_isKnockBack; }
	// �m�b�N�o�b�N�������������ݒ�
	void SetIsKnockBack(bool isKnockBack) { m_isKnockBack = isKnockBack; }
public:	// public�֐�
	// �R���X�g���N�^
	BossAI(IEnemy* pBoss);
	// �f�X�g���N�^
	~BossAI();
	// ������
	void Initialize();
	// �X�V
	void Update(float elapsedTime);
	// ��ԕύX
	void ChangeState(IState* newState);
private:// private�ϐ�
	//�U����
	std::unique_ptr<BossAttack> m_pBossAttack;
	//�m�b�N�o�b�N��
	std::unique_ptr<BossKnockBacking> m_pBossKnockBacking;
	// ���݂̏��
	IState* m_pCurrentState;
	// ���
	IState::EnemyState m_enemyState;
	// �U�����\���
	IState::EnemyState m_attackState;
	// �{�X
	IEnemy* m_pBoss;
	//�ړ�
	DirectX::SimpleMath::Vector3 m_position;
	// ���W�����l
	DirectX::SimpleMath::Vector3 m_initialPosition;
	//�����蔻��p���W
	DirectX::SimpleMath::Vector3 m_bsPosition;
	//�T�C�Y
	DirectX::SimpleMath::Vector3 m_scale;
	//��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �m�b�N�o�b�N����
	float m_knockTime;
	// �m�b�N�o�b�N�̊J�n�ʒu 
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// �m�b�N�o�b�N�̏I���ʒu
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// �m�b�N�o�b�N�̏����x
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	// ����
	float m_time;
	// �U���̃N�[���_�E���^�C��
	float m_attackCooldown;
	// �v���C���[�̒e�ɓ���������
	bool m_isHitPlayerBullet;
	// ��x�ł��m�b�N�o�b�N������������m�b�N�o�b�N���������Ȃ����߂̃t���O
	bool m_isKnockBack;
	// �U������
	bool m_isAttack;
};
#endif //BOSS_AI_DEFINED
