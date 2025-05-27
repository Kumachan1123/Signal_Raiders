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
	CalculateViewMatrix();// ビュー行列計算
	CalculateProjectionMatrix();// プロジェクション行列計算
}
/*
*	@brief 更新処理
*	@details カメラの位置と注視点を更新する
*	@param newEye	新しいeye座標
*	@param pitch	ピッチ角度
*/
void FPS_Camera::Update(const DirectX::SimpleMath::Vector3& newEye, float pitch)
{
	DirectX::SimpleMath::Vector3 oldEye = m_eye;	// 古いeyeを保持する
	m_eye = newEye;	// eyeの位置を更新する
	DirectX::SimpleMath::Vector3 velocity = m_eye - oldEye;	// 移動ベクトルを求める
	m_target += velocity;	// targetの位置を更新する
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationY(pitch); 	// 回転行列を作成する
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;	// カメラの方向ベクトルを計算する
	direction = DirectX::SimpleMath::Vector3::Transform(direction, rotation);	// 回転行列を使って方向ベクトルを回転させる
	m_target = m_eye + direction;	// 回転後の方向ベクトルを使ってtargetを更新する
	CalculateViewMatrix();	// ビュー行列を更新する
}
/*
*	@brief ビュー行列を計算する
*	@details カメラの位置と注視点を基にビュー行列を計算する
*	@param なし
*	@return なし
*/
void FPS_Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);// ビュー行列を計算する
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
	const float width = static_cast<float>(Screen::WIDTH);// ウィンドウ幅
	const float height = static_cast<float>(Screen::HEIGHT);// ウィンドウ高さ
	const float aspectRatio = width / height;// アスペクト比計算
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(// プロジェクション行列を計算する
		FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
}
/*
*	@brief カメラの方向ベクトルを取得する
*	@details カメラの注視点と位置から方向ベクトルを計算し、正規化して返す
*	@param なし
*	@return カメラの方向ベクトル
*/
DirectX::SimpleMath::Vector3 FPS_Camera::GetDirection() const
{
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;// targetとeyeの差から方向ベクトルを計算
	direction.Normalize();	// 正規化
	return direction;// 方向ベクトルを返す
}

