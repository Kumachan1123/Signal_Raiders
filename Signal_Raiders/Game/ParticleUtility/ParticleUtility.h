/*
	@file	ParticleUtility.h
	@brief	パーティクルユーティリティクラス
*/
#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
class ParticleUtility
{
public:
	enum class Type
	{
		ENEMYTRAIL,//	敵の弾の軌跡
		PLAYERTRAIL,//	プレイヤーの弾の軌跡
		BARRIERDESTROYED,//	バリア破壊
	};
	//	変数


private:
	//	関数
	//	座標
	DirectX::SimpleMath::Vector3 m_position;

	//	速度
	DirectX::SimpleMath::Vector3 m_velocity;

	// 回転
	DirectX::SimpleMath::Vector3 m_rotate;
	//	加速度
	DirectX::SimpleMath::Vector3 m_accele;
	DirectX::SimpleMath::Vector3 m_accele2;

	//	スケール
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	//	生存時間
	float m_life;
	float m_startLife;

	//	カラー
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;

	//	タイプ
	Type m_type;

public:
	//	関数
	//	コンストラクタ（初期設定）
	ParticleUtility(
		float life,
		DirectX::SimpleMath::Vector3 pos,
		DirectX::SimpleMath::Vector3 velocity,
		DirectX::SimpleMath::Vector3 accele,
		DirectX::SimpleMath::Vector3 accele2,
		DirectX::SimpleMath::Vector3 rotate,
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
		Type type);
	//	デストラクタ
	~ParticleUtility();

	//	更新
	bool Update(float elapsedTime);

	//	getter
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }

	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }

	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }

	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }

	float GetLife() const { return m_life; }
	float GetStartLife() const { return m_startLife; }
	DirectX::SimpleMath::Color GetNowColor() const { return m_nowColor; }
	DirectX::SimpleMath::Color GetStartColor() const { return m_startColor; }
	DirectX::SimpleMath::Color GetEndColor() const { return m_endColor; }

private:
	// タイプごとの処理
	void Trail(float elapsedTime);// 弾の軌跡
	void BarrierDestroyed(float elapsedTime);//	バリア破壊
};