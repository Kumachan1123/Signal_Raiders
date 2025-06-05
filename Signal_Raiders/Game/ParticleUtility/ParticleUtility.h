/*
*	@file	ParticleUtility.h
*	@brief	パーティクルユーティリティクラス
*/
#pragma once
#ifndef PARTICLEUTILITY_DEFINED
#define PARTICLEUTILITY_DEFINED
// 標準ライブラリ
#include <vector>
#include <cmath>
#include <random>
// DirectXライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>

// パーティクルユーティリティクラス
class ParticleUtility
{
public:
	// 列挙型
	// パーティクルの種類
	enum class Type
	{
		ENEMYTRAIL,//	敵の弾の軌跡
		PLAYERTRAIL,//	プレイヤーの弾の軌跡
		BARRIERBREAK,//	バリア破壊
	};
public:
	// アクセサ
	// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 速度取得
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// 加速度取得
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }
	// 現在のスケール取得
	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }
	// 開始スケール取得
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }
	// 終了スケール取得
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }
	// 生存時間取得
	float GetLife() const { return m_life; }
	// 開始生存時間取得
	float GetStartLife() const { return m_startLife; }
	// 現在の色取得
	DirectX::SimpleMath::Color GetNowColor() const { return m_nowColor; }
	// 開始色取得
	DirectX::SimpleMath::Color GetStartColor() const { return m_startColor; }
	// 終了色取得
	DirectX::SimpleMath::Color GetEndColor() const { return m_endColor; }
public:
	// public関数
	// コンストラクタ　
	ParticleUtility(
		float life,// 生存時間
		const DirectX::SimpleMath::Vector3& pos,//	座標
		const DirectX::SimpleMath::Vector3& velocity,// 速度
		const DirectX::SimpleMath::Vector3& accele,// 加速度
		const DirectX::SimpleMath::Vector3& rotateAccele,// 回転加速度
		const DirectX::SimpleMath::Vector3& rotate,// 回転
		const DirectX::SimpleMath::Vector3& startScale, const DirectX::SimpleMath::Vector3& endScale,// スケール(最初の大きさ、最後の大きさ)
		const DirectX::SimpleMath::Color& startColor, const DirectX::SimpleMath::Color& endColor,// カラー(最初の色、最後の色)
		Type type);// タイプ
	// デストラクタ
	~ParticleUtility();
	// 更新
	bool Update(float elapsedTime);
private:
	// private関数
	// タイプの切り替え
	void SwitchType(float elapsedTime);
	// タイプごとの処理
	// 弾の軌跡
	void Trail(float elapsedTime);
	//	バリア破壊
	void BarrierDestroyed(float elapsedTime);
private:
	// private変数
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 初期位置
	DirectX::SimpleMath::Vector3 m_startPosition;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転
	DirectX::SimpleMath::Vector3 m_rotate;
	// 加速度
	DirectX::SimpleMath::Vector3 m_accele;
	// 回転加速度
	DirectX::SimpleMath::Vector3 m_rotateAccele;
	// 現在のスケール
	DirectX::SimpleMath::Vector3 m_nowScale;
	// 開始スケール
	DirectX::SimpleMath::Vector3 m_startScale;
	// 終了スケール
	DirectX::SimpleMath::Vector3 m_endScale;
	// 生存時間
	float m_life;
	// 開始生存時間
	float m_startLife;
	// 経過時間
	float m_elapsedTime;
	// 現在の色
	DirectX::SimpleMath::Color m_nowColor;
	// 開始色
	DirectX::SimpleMath::Color m_startColor;
	// 終了色
	DirectX::SimpleMath::Color m_endColor;
	// タイプ
	Type m_type;
};
#endif // PARTICLEUTILITY_DEFINED