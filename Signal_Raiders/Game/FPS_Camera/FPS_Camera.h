/*
*	@file	FPS_Camera.h
*	@brief	FPSカメラクラス
*/
#pragma once
// DirectxX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/Screen.h"
class FPS_Camera
{
public:	// アクセサ
	// ビュー行列取得
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
	// プロジェクション行列取得
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_projection; }
	// カメラ座標取得
	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }
	// カメラ座標Y軸設定
	void SetEyePositionY(float eye) { m_eye.y += eye; }
	// 注視点取得
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// 注視点Y軸設定
	void SetTargetPositionY(float target) { m_target.y += target; }
	// カメラの頭の方向取得
	const DirectX::SimpleMath::Vector3& GetUpVector() const { return m_up; }
	// カメラの方向ベクトル取得
	DirectX::SimpleMath::Vector3 GetDirection() const;
public:	//public関数
	// コンストラクタ
	FPS_Camera(
		const DirectX::SimpleMath::Vector3& eye = DirectX::SimpleMath::Vector3{ 0.0f,4.0f,10.0f },// カメラ座標
		const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3{ 0.0f,0.0f,0.0f },// 注視点
		const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY// カメラの頭の方向
	);
	// デストラクタ
	~FPS_Camera() = default;
	// 更新処理
	void Update(const DirectX::SimpleMath::Vector3& newEye, float pitch);
private://private関数
	// ビュー行列を計算する
	void CalculateViewMatrix();
	// プロジェクション行列を計算する
	void CalculateProjectionMatrix();
private:// private定数
	// 画角
	const float FOV = DirectX::XMConvertToRadians(45.0f);
	// 近い投影面
	const float NEAR_PLANE = 1.0f;
	// 遠い投影面
	const float FAR_PLANE = 1000.0f;
private:// private変数
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_projection;
	// カメラ座標
	DirectX::SimpleMath::Vector3 m_eye;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;
	// カメラの頭の方向
	DirectX::SimpleMath::Vector3 m_up;
	// ヨー、ピッチ（回転角度）
	float m_yaw, m_pitch;
};
