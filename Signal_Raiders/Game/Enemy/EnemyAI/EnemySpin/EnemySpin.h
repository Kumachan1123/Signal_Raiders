/*
	@file	EnemySpin.h
	@brief	敵逃避クラス
*/
#pragma once
#ifndef ENEMY_ESCAPE_DEFINED
#define ENEMY_ESCAPE_DEFINED
#include "Game/Interface/IState.h"
class EnemyAI;
//前方宣言

class EnemySpin : public IState
{
private:
	EnemyAI* m_enemy;//敵AI
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Vector3 m_initialPosition;// 座標初期値
	DirectX::SimpleMath::Vector3 m_scale;//サイズ
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	float m_rotationSpeed;//回転速度
	float m_angle;//角度
	const float RANDOM_MAX = 2.0f;
	const float RANDOM_MIN = 0.5f;
	float  m_time = 0.0f;  // 時間の初期化
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