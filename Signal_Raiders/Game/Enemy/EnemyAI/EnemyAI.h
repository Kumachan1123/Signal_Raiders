/*
	@file	EnemyAI.h
	@brief	�GAI�N���X
*/
#pragma once
#ifndef ENEMY_AI_DEFINED
#define ENEMY_AI_DEFINED
//�O���錾
class CommonResources;

#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAttack/EnemyAttack.h"
#include "Game/Enemy/EnemyAI/EnemyIdling/EnemyIdling.h"
#include "Game/Enemy/EnemyAI/EnemySpin/EnemySpin.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IEnemy.h"
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

	float m_knockTime = 0.0f;// knockBack���鎞��
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	float m_rotationSpeed;//��]���x
	const float RANDOM_MAX = 7.0f;
	const float RANDOM_MIN = 5.5f;
	float  m_time = 0.0f;  // ���Ԃ̏�����
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��
	bool m_isHitPlayerBullet = false;
	bool m_canAttack = false;// �U���\��
public:
	//	getter

	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	EnemyAttack* GetEnemyAttack()const { return m_pEnemyAttack.get(); }
	EnemyIdling* GetEnemyIdling()const { return m_pEnemyIdling.get(); }
	EnemySpin* GetEnemySpin()const { return m_pEnemySpin.get(); }
	IState* GetNowState()const { return m_currentState; }
	IState::EnemyState GetState()const { return m_enemyState; }
	//  setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	void KnockBack(float elapsedTime, DirectX::SimpleMath::Vector3& pos, bool& isHitToPlayerBullet, const DirectX::SimpleMath::Vector3& playerPos);
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; }
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
public:
	EnemyAI(IEnemy* pEnemy);
	~EnemyAI();
	void Initialize();
	void Update(float elapsedTime,
		DirectX::SimpleMath::Vector3& pos,
		DirectX::SimpleMath::Vector3& playerPos,
		bool& isHitToPlayer,
		bool& isHitToPlayerBullet);
	void ChangeState(IState* newState);



};
#endif //ENEMY_AI_DEFINED
