/*
	@file	BossAttack.h
	@brief	�{�X�U���N���X
	�쐬�ҁF���܂�
*/

#pragma once
#ifndef BOSS_ATTACK_DEFINED
#define BOSS_ATTACK_DEFINED
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"
class CommonResources;
//�O���錾
class BossAI;


class BossAttack : public IState
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	BossAI* m_pBoss;//�GAI
	DirectX::SimpleMath::Vector3 m_position;//�ړ�
	DirectX::SimpleMath::Quaternion m_rotation;//��]
	DirectX::SimpleMath::Vector3 m_velocity;// �ړ����x
	DirectX::SimpleMath::Vector3 m_scale;//�T�C�Y

	float m_rotationSpeed;//��]���x
	const float RANDOM_MAX = 2.0f;
	const float RANDOM_MIN = 0.5f;
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��

public:
	//	getter
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }
	float GetCoolTime() { return m_attackCooldown; }
	//  setter
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
public:
	BossAttack(BossAI* pBoss);
	~BossAttack();

	void Initialize() override;
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer) override;
};
#endif //BOSS_ATTACK_DEFINED
