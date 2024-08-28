/*
@file	FPS_Camera.h
@brief	FPSカメラクラス
*/
#pragma once
#include <SimpleMath.h>

class FPS_Camera
{
private:
	// 画角
	const float FOV = DirectX::XMConvertToRadians(45.0f);

	// 近い投影面
	const float NEAR_PLANE = 1.0f;

	// 遠い投影面
	const float FAR_PLANE = 1000.0f;

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

	float m_yaw, m_pitch;
public:
	// コンストラクタ
	FPS_Camera(
		const DirectX::SimpleMath::Vector3& eye = DirectX::SimpleMath::Vector3{ 0.0f,2.0f,10.0f },
		const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3{ 0.0f,0.0f,0.0f },
		const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY


	);

	// デストラクタ
	~FPS_Camera() = default;

	// 更新処理
	void Update(const DirectX::SimpleMath::Vector3& newEye, float yawX);


	// getter
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }

	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_projection; }

	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }

	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }

	const DirectX::SimpleMath::Vector3& GetUpVector() const { return m_up; }

	// setter
	void SetTargetPositionY(float target) { m_target.y += target; }

	DirectX::SimpleMath::Vector3 GetDirection() const;
private:
	// ビュー行列を計算する
	void CalculateViewMatrix();
	// 数値補正
	//float correctValue(float value);

	// プロジェクション行列を計算する
	void CalculateProjectionMatrix();
};
