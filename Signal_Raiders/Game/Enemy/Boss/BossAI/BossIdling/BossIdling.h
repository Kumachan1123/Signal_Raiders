/*
	@file	BossIdling.h
	@brief	ボス徘徊クラス
*/
#pragma once
#ifndef BOSS_IDLING_DEFINED
#define BOSS_IDLING_DEFINED
#include "Game/Interface/IState.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
class BossAI;
//前方宣言

class BossIdling : public IState
{
private:
	BossAI* m_pBoss;//敵AI
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Vector3 m_initialPosition;// 座標初期値
	DirectX::SimpleMath::Vector3 m_scale;//サイズ
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	float m_rotationSpeed;//回転速度
	float  m_time;  // 時間の初期化
	float m_angle;//角度

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