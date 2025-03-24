/*
	@file	EnemyAI.h
	@brief	�GAI�N���X
*/
#pragma once
#ifndef ENEMY_AI_DEFINED
#define ENEMY_AI_DEFINED


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
private:
	//���펞
	std::unique_ptr<EnemyIdling> m_pEnemyIdling;
	//�U����
	std::unique_ptr<EnemyAttack> m_pEnemyAttack;
	//������
	std::unique_ptr<EnemySpin> m_pEnemySpin;
	// ���݂̏��
	IState* m_currentState;
	IState::EnemyState m_enemyState;
	// �G�̃|�C���^�[
	IEnemy* m_pEnemy;
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Vector3 m_initialPosition;// ���W�����l
	DirectX::SimpleMath::Vector3 m_bsPosition;//�����蔻��p���W
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x

	float m_knockTime;// knockBack���鎞��
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	float  m_time;  // ���Ԃ̏�����
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��
	bool m_isHitPlayerBullet;// �v���C���[�̒e�ɓ���������
	bool m_canAttack;// �U���\��
	bool m_isAttack;// �U������

public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }
	IState::EnemyState GetState()const { return m_enemyState; }
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; }
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
	EnemyAttack* GetEnemyAttack()const { return m_pEnemyAttack.get(); }
	EnemyIdling* GetEnemyIdling()const { return m_pEnemyIdling.get(); }
	EnemySpin* GetEnemySpin()const { return m_pEnemySpin.get(); }
	IEnemy* GetEnemy()const { return m_pEnemy; }
	IState* GetNowState()const { return m_currentState; }
	bool GetisAttack()const { return m_pEnemy->GetIsAttack(); }
	void SetisAttack(bool isAttack) { m_pEnemy->SetIsAttack(isAttack); }
public:
	EnemyAI(IEnemy* pEnemy);
	~EnemyAI();
	void Initialize();
	void Update(float elapsedTime);
	void ChangeState(IState* newState);
	void KnockBack(float elapsedTime);



};
#endif //ENEMY_AI_DEFINED
