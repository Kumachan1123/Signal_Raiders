/*
	@file	BossAI.h
	@brief	�{�XAI�N���X
*/
#pragma once
#ifndef BOSS_AI_DEFINED
#define BOSS_AI_DEFINED
//�O���錾
class CommonResources;

#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAttack/BossAttack.h"
#include "Game/Enemy/Boss/BossAI/BossIdling/BossIdling.h"
#include "Game/Player/Player.h"
#include "Game/Interface/IState.h"
class BossAttack;
class BossIdling;
class BossAI
{
private:

	//�U����
	std::unique_ptr<BossAttack> m_pBossAttack;
	//�ҋ@��
	std::unique_ptr<BossIdling> m_pBossIdling;
	// ���݂̏��
	IState* m_currentState;
	IState::EnemyState m_enemyState;
	IEnemy* m_pBoss;

	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Vector3 m_initialPosition;// ���W�����l
	DirectX::SimpleMath::Vector3 m_bsPosition;//�����蔻��p���W
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x

	float m_knockTime;// knockBack���鎞��
	DirectX::SimpleMath::Vector3 m_knockStartPosition, m_knockEndPosition, m_initialVelocity;

	float m_time;  // ���Ԃ̏�����
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��
	bool m_isHitPlayerBullet;
	// ��x�ł��m�b�N�o�b�N������������m�b�N�o�b�N���������Ȃ����߂̃t���O
	bool m_isKnockBack;


public:
	//	getter

	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	BossAttack* GetBossAttack()const { return m_pBossAttack.get(); }
	IEnemy* GetEnemy()const { return m_pBoss; }
	IState* GetNowState()const { return m_currentState; }
	IState::EnemyState GetState()const { return m_enemyState; }
	//  setter
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	void KnockBack(float elapsedTime);
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
public:
	BossAI(IEnemy* pBoss);
	~BossAI();
	void Initialize();
	void Update(float elapsedTime);
	void ChangeState(IState* newState);
};
#endif //BOSS_AI_DEFINED
