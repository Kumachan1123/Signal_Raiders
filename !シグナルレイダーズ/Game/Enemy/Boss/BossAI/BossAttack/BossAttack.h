/*
	@file	BossAttack.h
	@brief	ボス攻撃クラス
	作成者：くまち
*/

#pragma once
#ifndef BOSS_ATTACK_DEFINED
#define BOSS_ATTACK_DEFINED
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"
class CommonResources;
//前方宣言
class BossAI;


class BossAttack : public IState
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	BossAI* m_pBoss;//敵AI
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	DirectX::SimpleMath::Vector3 m_scale;//サイズ

	float m_rotationSpeed;//回転速度
	const float RANDOM_MAX = 2.0f;
	const float RANDOM_MIN = 0.5f;
	float m_attackCooldown;  // 攻撃のクールダウンタイム

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
