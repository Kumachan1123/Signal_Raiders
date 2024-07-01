/*
	@file	EnemyAI.h
	@brief	敵AIクラス
	作成者：くまち
*/
#pragma once
#ifndef ENEMY_AI_DEFINED
#define ENEMY_AI_DEFINED
//前方宣言
class CommonResources;

#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAttack/EnemyAttack.h"
#include "Game/Enemy/EnemyAI/EnemyIdling/EnemyIdling.h"

#include "Game/Interface/IStateh.h"
class EnemyAttack;
class EnemyIdling;
class EnemyAI
{
private:
	//平常時
	std::unique_ptr<EnemyAttack> m_enemyAttack;
	std::unique_ptr<EnemyIdling> m_enemyIdling;
	//攻撃時

	// 現在の状態
	IState* m_currentState;
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Vector3 m_initialPosition;// 座標初期値
	DirectX::SimpleMath::Vector3 m_bsPosition;//当たり判定用座標
	DirectX::SimpleMath::Vector3 m_scale;//サイズ
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	float m_rotationSpeed;//回転速度
	const float RANDOM_MAX = 2.0f;
	const float RANDOM_MIN = 0.5f;
	float  m_time = 0.0f;  // 時間の初期化
	float m_attackCooldown;  // 攻撃のクールダウンタイム
public:
	//	getter

	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	DirectX::SimpleMath::Quaternion GetRotation() const{ return m_rotation; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	EnemyAttack* GetEnemyAttack()const { return m_enemyAttack.get(); }
	EnemyIdling* GetEnemyIdling()const { return m_enemyIdling.get(); }
	IState* GetNowState()const { return m_currentState; }
	//  setter
	void SetPosition( DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }

public :
	EnemyAI();
	~EnemyAI();
	void Initialize();
	void Update(float elapsedTime,
				DirectX::SimpleMath::Vector3& pos,
				DirectX::SimpleMath::Vector3& playerPos,
				bool& isHitToPlayer);
	void ChangeState(IState* newState);
	// 攻撃ロジックを実装
	template <typename T>
	T GenerateRandomMultiplier(T min, T max);
};
#endif //ENEMY_AI_DEFINED
