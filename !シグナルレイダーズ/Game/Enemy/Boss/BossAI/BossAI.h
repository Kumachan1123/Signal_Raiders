/*
	@file	EnemyAI.h
	@brief	�GAI�N���X
	�쐬�ҁF���܂�
*/
#pragma once
#ifndef BOSS_AI_DEFINED
#define BOSS_AI_DEFINED
//�O���錾
class CommonResources;

#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAttack/BossAttack.h"
#include "Game/Enemy/Boss/BossAI/BossIdling/BossIdling.h"

#include "Game/Interface/IState.h"
class BossAttack;
class BossIdling;
class BossAI
{
private:
	//���펞

	std::unique_ptr<BossIdling> m_enemyIdling;
	//�U����
	std::unique_ptr<BossAttack> m_enemyAttack;
	// ���݂̏��
	IState* m_currentState;
	IState::EnemyState m_enemyState;
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Vector3 m_initialPosition;// ���W�����l
	DirectX::SimpleMath::Vector3 m_bsPosition;//�����蔻��p���W
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x

	float m_knockTime = 0.0f;// knockBack���鎞��
	DirectX::SimpleMath::Vector3 m_knockStartPosition, m_knockEndPosition, m_initialVelocity;
	float m_rotationSpeed;//��]���x
	const float RANDOM_MAX = 2.0f;
	const float RANDOM_MIN = 0.5f;
	float  m_time = 0.0f;  // ���Ԃ̏�����
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��
	bool m_isHitPlayerBullet = false;
public:
	//	getter

	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	BossAttack* GetEnemyAttack()const { return m_enemyAttack.get(); }
	BossIdling* GetEnemyIdling()const { return m_enemyIdling.get(); }
	IState* GetNowState()const { return m_currentState; }
	IState::EnemyState GetState()const { return m_enemyState; }
	//  setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	void KnockBack(float elapsedTime, DirectX::SimpleMath::Vector3& pos, bool& isHitToPlayerBullet, const DirectX::SimpleMath::Vector3& playerPos);
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
public:
	BossAI();
	~BossAI();
	void Initialize();
	void Update(float elapsedTime,
				DirectX::SimpleMath::Vector3& pos,
				DirectX::SimpleMath::Vector3& playerPos,
				bool& isHitToPlayer,
				bool& isHitToPlayerBullet);
	void ChangeState(IState* newState);
	// �U�����W�b�N������

	// �����^�p�̃����_���Ȕ{���𐶐�����֐�
	int GenerateRandomMultiplier(int min, int max);


	// ���������_�^�p�̃����_���Ȕ{���𐶐�����֐�
	float GenerateRandomMultiplier(float min, float max);

};
#endif //BOSS_AI_DEFINED
