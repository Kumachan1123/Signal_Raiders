/*
	@file	EnemySpin.h
	@brief	�G�����N���X
*/
#pragma once
#ifndef ENEMY_ESCAPE_DEFINED
#define ENEMY_ESCAPE_DEFINED
#include "Game/Interface/IState.h"
class EnemyAI;
//�O���錾

class EnemySpin : public IState
{
private:
	EnemyAI* m_enemy;//�GAI
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Vector3 m_initialPosition;// ���W�����l
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x
	float m_rotationSpeed;//��]���x
	float m_angle;//�p�x
	const float RANDOM_MAX = 2.0f;
	const float RANDOM_MIN = 0.5f;
	float  m_time = 0.0f;  // ���Ԃ̏�����
public:
	//	getter
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	//  setter
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }

public:
	EnemySpin(EnemyAI* enemy);
	virtual ~EnemySpin();

	void Initialize() override;
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer) override;
};
#endif //ENEMY_ESCAPE_DEFINED