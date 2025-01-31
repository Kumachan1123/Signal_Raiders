/*
	@file	BossIdling.h
	@brief	�{�X�p�j�N���X
*/
#pragma once
#ifndef BOSS_IDLING_DEFINED
#define BOSS_IDLING_DEFINED
#include "Game/Interface/IState.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
class BossAI;
//�O���錾

class BossIdling : public IState
{
private:
	BossAI* m_pBoss;//�GAI
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Vector3 m_initialPosition;// ���W�����l
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x
	float m_rotationSpeed;//��]���x
	float  m_time;  // ���Ԃ̏�����
	float m_angle;//�p�x

public:
	//	getter
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	//  setter
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }

public:
	BossIdling(BossAI* pBoss);
	virtual ~BossIdling();

	void Initialize() override;
	void Update(float elapsedTime) override;
};
#endif //BOSS_IDLING_DEFINED