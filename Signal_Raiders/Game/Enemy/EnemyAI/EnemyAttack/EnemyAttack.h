/*
	@file	Attack.h
	@brief	敵Attackクラス
*/

#pragma once
#ifndef ENEMY_ATTACK_DEFINED
#define ENEMY_ATTACK_DEFINED
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"
class CommonResources;
//前方宣言
class EnemyAI;


class EnemyAttack : public IState
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	EnemyAI* m_enemy;//敵AI
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	DirectX::SimpleMath::Vector3 m_scale;//サイズ

	float m_rotationSpeed;//回転速度
	float m_attackCooldown;  // 攻撃のクールダウンタイム

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
