/*
	@file	Attack.h
	@brief	�GAttack�N���X
*/

#pragma once
#ifndef ENEMY_ATTACK_DEFINED
#define ENEMY_ATTACK_DEFINED
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"
class CommonResources;
//�O���錾
class EnemyAI;


class EnemyAttack : public IState
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	EnemyAI* m_enemy;//�GAI
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y

	float m_rotationSpeed;//��]���x
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��

public:
	//	getter
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	float GetCoolTime() { return m_attackCooldown; }
	//  setter
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
public:
	EnemyAttack(EnemyAI* enemy);
	~EnemyAttack();

	void Initialize() override;
	void Update(float elapsedTime) override;
};
#endif //ENEMY_ATTACK_DEFINED
