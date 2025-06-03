/*
*	@file	FPS_Camera.cpp
*	@brief	FPSカメラクラス
*/
#include <pch.h>
#include "FPS_Camera.h"
/*
*	@brief	コンストラクタ
*	@details FPSカメラの初期化を行う
*	@param   eye	カメラ座標
*	@param   target	注視点
*	@param   up		カメラの頭の方向
*/
FPS_Camera::FPS_Camera(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target,
	const DirectX::SimpleMath::Vector3& up)
	: m_view{}// ビュー行列
	, m_projection{} // プロジェクション行列
	, m_eye{ eye }// カメラ座標
	, m_target{ target }// 注視点
	, m_up{ up }// カメラの頭の方向
{
	// ビュー行列計算
	CalculateViewMatrix();
	// プロジェクション行列計算
	CalculateProjectionMatrix();
}
/*
*	@brief 更新処理
*	@details カメラの位置と注視点を更新する
*	@param newEye	新しいeye座標
*	@param pitch	ピッチ角度
*/
void FPS_Camera::Update(const DirectX::SimpleMath::Vector3& newEye, float pitch)
{
	// 古いeyeを保持する
	DirectX::SimpleMath::Vector3 oldEye = m_eye;
	// eyeの位置を更新する
	m_eye = newEye;
	// 移動ベクトルを求める
	DirectX::SimpleMath::Vector3 velocity = m_eye - oldEye;
	// targetの位置を更新する
	m_target += velocity;
	// 回転行列を作成する
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationY(pitch);
	// カメラの方向ベクトルを計算する
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;
	// 回転行列を使って方向ベクトルを回転させる
	direction = DirectX::SimpleMath::Vector3::Transform(direction, rotation);
	// 回転後の方向ベクトルを使ってtargetを更新する
	m_target = m_eye + direction;
	// ビュー行列を更新する
	CalculateViewMatrix();
}
/*
*	@brief ビュー行列を計算する
*	@details カメラの位置と注視点を基にビュー行列を計算する
*	@param なし
*	@return なし
*/
void FPS_Camera::CalculateViewMatrix()
{
	// ビュー行列を計算する
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}
/*
*	@brief プロジェクション行列を計算する
*	@details ウィンドウサイズを基にプロジェクション行列を計算する
*	@param なし
*	@return なし
*/
void FPS_Camera::CalculateProjectionMatrix()
{
	// ウィンドウサイズを取得
	// ウィンドウ幅
	const float width = static_cast<float>(Screen::WIDTH);
	// ウィンドウ高さ
	const float height = static_cast<float>(Screen::HEIGHT);
	// アスペクト比を計算
	const float aspectRatio = width / height;
	// プロジェクション行列を計算する
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
}
/*
*	@brief カメラの方向ベクトルを取得する
*	@details カメラの注視点と位置から方向ベクトルを計算し、正規化して返す
*	@param なし
*	@return カメラの方向ベクトル
*/
DirectX::SimpleMath::Vector3 FPS_Camera::GetDirection() const
{
	// targetとeyeの差から方向ベクトルを計算
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;
	// 方向ベクトルを正規化
	direction.Normalize();
	// 方向ベクトルを返す
	return direction;
}

