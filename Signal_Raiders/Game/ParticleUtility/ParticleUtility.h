/*
*	@file	ParticleUtility.h
*	@brief	パーティクルユーティリティクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cmath>
#include <random>
// DirectXライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>
class ParticleUtility
{
public:
	// 列挙型
	enum class Type// パーティクルの種類
	{
		ENEMYTRAIL,//	敵の弾の軌跡
		PLAYERTRAIL,//	プレイヤーの弾の軌跡
		BARRIERBREAK,//	バリア破壊
	};
public:
	// public関数
	ParticleUtility(	// コンストラクタ　
		float life,// 生存時間
		DirectX::SimpleMath::Vector3 pos,//	座標
		DirectX::SimpleMath::Vector3 velocity,// 速度
		DirectX::SimpleMath::Vector3 accele,// 加速度
		DirectX::SimpleMath::Vector3 rotateAccele,// 回転加速度
		DirectX::SimpleMath::Vector3 rotate,// 回転
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,// スケール(最初の大きさ、最後の大きさ)
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,// カラー(最初の色、最後の色)
		Type type);// タイプ
	~ParticleUtility();// デストラクタ


	bool Update(float elapsedTime);// 更新

	// アクセサ
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 座標取得
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }// 速度取得
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }// 加速度取得
	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }// 現在のスケール取得
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }// 開始スケール取得
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }// 終了スケール取得
	float GetLife() const { return m_life; }// 生存時間取得
	float GetStartLife() const { return m_startLife; } // 開始生存時間取得
	DirectX::SimpleMath::Color GetNowColor() const { return m_nowColor; }// 現在の色取得
	DirectX::SimpleMath::Color GetStartColor() const { return m_startColor; } // 開始色取得
	DirectX::SimpleMath::Color GetEndColor() const { return m_endColor; } // 終了色取得
private:
	// private関数
	void SwitchType(float elapsedTime);// タイプの切り替え
	// タイプごとの処理
	void Trail(float elapsedTime);// 弾の軌跡
	void BarrierDestroyed(float elapsedTime);//	バリア破壊
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
	// スケール
	DirectX::SimpleMath::Vector3 m_nowScale;// 現在のスケール
	DirectX::SimpleMath::Vector3 m_startScale;// 開始スケール
	DirectX::SimpleMath::Vector3 m_endScale;// 終了スケール
	// 生存時間
	float m_life;
	// 開始生存時間
	float m_startLife;
	// 経過時間
	float m_elapsedTime;
	// カラー
	DirectX::SimpleMath::Color m_nowColor;// 現在の色
	DirectX::SimpleMath::Color m_startColor;// 開始色
	DirectX::SimpleMath::Color m_endColor;// 終了色
	// タイプ
	Type m_type;
};