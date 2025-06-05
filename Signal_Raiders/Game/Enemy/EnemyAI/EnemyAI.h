/*
*	@file	EnemyAI.h
*	@brief	�GAI�N���X
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
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
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
public:// �A�N�Z�T
	// ���݂̈ʒu���擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// ���݂̈ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// �����ʒu���擾
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	// ��]�����擾�i�N�H�[�^�j�I���j
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// ��]����ݒ�i�N�H�[�^�j�I���j
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// ���݂̑��x�x�N�g�����擾
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// ���݂̑��x�x�N�g����ݒ�
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	// �X�P�[�������擾
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// �X�P�[������ݒ�
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// ���݂̓G�̏�Ԃ��擾
	IState::EnemyState GetState() const { return m_enemyState; }
	// �G�̏�Ԃ�ݒ�
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	// �U���\�t���O��ݒ�
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; }
	// �v���C���[�̒e�ɓ����������̃t���O���擾
	bool GetHitPlayerBullet() const { return m_isHitPlayerBullet; }
	// �v���C���[�̒e�ɓ����������̃t���O��ݒ�
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
	// �G�̍U���N���X���擾
	EnemyAttack* GetEnemyAttack() const { return m_pEnemyAttack.get(); }
	// �G�̜p�j�N���X���擾
	EnemyIdling* GetEnemyIdling() const { return m_pEnemyIdling.get(); }
	// �G�̉�]�N���X�i�X�s���j���擾
	EnemySpin* GetEnemySpin() const { return m_pEnemySpin.get(); }
	// ���̂ƂȂ�G�L�����̏����擾
	IEnemy* GetEnemy() const { return m_pEnemy; }
	// ���݂̃X�e�[�g�i��ԃN���X�j���擾
	IState* GetNowState() const { return m_pCurrentState; }
	// �U�������ǂ������擾
	bool GetisAttack() const { return m_pEnemy->GetIsAttack(); }
	// �U�����t���O��ݒ�
	void SetIsAttack(bool isAttack) { m_pEnemy->SetIsAttack(isAttack); }
public:// public�����o�֐�
	// �R���X�g���N�^
	EnemyAI(IEnemy* pEnemy);
	// �f�X�g���N�^
	~EnemyAI();
	// ������
	void Initialize();
	// �X�V
	void Update(float elapsedTime);
	// ��ԕύX
	void ChangeState(IState* newState);
private:// private�����o�ϐ�
	// �p�j��Ԃ̃|�C���^�[
	std::unique_ptr<EnemyIdling> m_pEnemyIdling;
	// �U����Ԃ̃|�C���^�[
	std::unique_ptr<EnemyAttack> m_pEnemyAttack;
	// �X�s����Ԃ̃|�C���^�[
	std::unique_ptr<EnemySpin>	 m_pEnemySpin;
	// ���݂̏��
	IState* m_pCurrentState;
	// �G�̏��
	IState::EnemyState m_enemyState;
	// �G�̃|�C���^�[
	IEnemy* m_pEnemy;
	// �G�̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �G�̍��W�����l
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// �G�̓����蔻��p���W
	DirectX::SimpleMath::Vector3 m_bsPosition;
	// �G�̃X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// �G�̉�]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �G�̈ړ����x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �m�b�N�o�b�N���鎞��
	float m_knockTime;
	// �m�b�N�o�b�N�̊J�n�ʒu
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// �m�b�N�o�b�N�̏I���ʒu
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// �m�b�N�o�b�N�̏������x
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	// ����
	float  m_time;
	// �U���̃N�[���_�E���^�C��
	float m_attackCooldown;
	// �v���C���[�̒e�ɓ���������
	bool m_isHitPlayerBullet;
	// �U���\��
	bool m_canAttack;
	// �U������
	bool m_isAttack;
};
#endif //ENEMY_AI_DEFINED
