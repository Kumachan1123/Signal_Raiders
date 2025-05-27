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
public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }// �����ʒu���擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// �ʒu���擾
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// �ʒu��ݒ�
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// ��]���擾
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// ��]��ݒ�
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }// ���x���擾
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }// ���x��ݒ�
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }// �X�P�[�����擾
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }// �X�P�[����ݒ�
	BossAttack* GetBossAttack()const { return m_pBossAttack.get(); }// �U�����̏�Ԃ��擾
	IEnemy* GetEnemy()const { return m_pBoss; }// �G���擾
	IState* GetNowState()const { return m_pCurrentState; }// ���݂̏�Ԃ��擾
	IState::EnemyState GetState()const { return m_enemyState; }// ��Ԃ��擾
	void SetState(IState::EnemyState state) { m_enemyState = state; }// ��Ԃ�ݒ�
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }// �v���C���[�̒e�ɓ����������ݒ�
	bool GetIsAttack()const { return m_pBoss->GetIsAttack(); }// �U�������擾
	void SetIsAttack(bool attack) { m_pBoss->SetIsAttack(attack); }// �U�������ݒ�
public:
	// public�֐�
	BossAI(IEnemy* pBoss);// �R���X�g���N�^
	~BossAI();	// �f�X�g���N�^
	void Initialize();// ������
	void Update(float elapsedTime);// �X�V
	void ChangeState(IState* newState);// �X�e�[�g�ύX
private:// private�֐�
	void KnockBack(float elapsedTime);// �m�b�N�o�b�N
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
