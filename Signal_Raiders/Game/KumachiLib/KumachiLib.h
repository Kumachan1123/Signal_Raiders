/*
*	@file    KumachiLib.h
*	@brief   自作のライブラリのヘッダーファイル
*	@details C++14で標準搭載されていない関数やその他便利な関数のテンプレートなどをまとめたもの
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
#include <cmath>
#include <algorithm>
#include <random>
#include <type_traits> 
// 外部ライブラリ
#include "Game/Screen.h"

/*
*	@brief   2Dベクトルの線形補間
*	@details 2Dベクトルの線形補間を行う関数
*	@param   start 開始点
*	@param   end 終了点
*	@param   t 補間係数 (0.0f <= t <= 1.0f)
*	@return  補間されたベクトル
*/
template <typename T>
inline T Lerp(const T& start, const T& end, float t)
{
	return start + t * (end - start);// 線形補間
}
/*
*	@brief   2Dベクトルのクランプ
*	@details 2Dベクトルのクランプを行う関数
*	@param   value クランプする値
*	@param   min 最小値
*	@param   max 最大値
*	@return  クランプされた値
*/
template <typename T>
inline T Clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;// クランプ
}
/*
*	@brief   2Dベクトルのスフェリカル補間
*	@details 2Dベクトルのスフェリカル補間を行う関数
*	@param   start 開始点
*	@param   end 終了点
*	@param   t 補間係数 (0.0f <= t <= 1.0f)
*	@return  補間されたベクトル
*/
template <typename T>
inline T Slerp(const T& start, const T& end, float t)
{
	float dot = start.Dot(end);// 内積
	dot = Clamp(dot, -1.0f, 1.0f);// クランプ
	float theta = std::acos(dot) * t;// 角度
	T relative = end - start * dot;// 相対ベクトル
	relative.Normalize();// 正規化
	return ((start * std::cos(theta)) + (relative * std::sin(theta)));// スフェリカル補間
}
/*
*	@brief   2Dベクトルの角度を計算
*	@details 2Dベクトルの角度を計算する関数
*	@param   eye 視点
*	@param   target ターゲット
*	@return  角度
*/
inline float CalculateAngle(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target)
{
	DirectX::SimpleMath::Vector3 direction = target - eye;// ベクトルの差分
	direction.Normalize();// 正規化
	float angle = std::atan2(direction.x, direction.z);// アークタンジェント
	return angle;// 角度
}


// 物体Aが物体Bに向かって動く(高さは無視)
inline DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& A,
	const DirectX::SimpleMath::Vector3& B,
	float maxSpeed)
{
	DirectX::SimpleMath::Vector3 desired = B - A;// 目標位置
	desired.Normalize();// 正規化
	desired *= maxSpeed;// 最大速度
	desired.y = 0;// 高さを無視
	return desired;// 目標位置
}

/*
*	@brief   整数型用のランダムな倍率を生成する
*	@details 整数型用のランダムな倍率を生成する関数
*	@param   min 最小値
*	@param   max 最大値
*	@return  ランダムな倍率
*/
inline int GenerateRandomMultiplier(int min, int max)
{
	std::random_device rd;// 乱数生成器
	std::mt19937 gen(rd());// メルセンヌ・ツイスタ法
	std::uniform_int_distribution<int> dis(min, max);// 一様分布
	return dis(gen);// ランダムな倍率を生成
}

/*
*	@brief   浮動小数点型用のランダムな倍率を生成する
*	@details 浮動小数点型用のランダムな倍率を生成する関数
*	@param   min 最小値
*	@param   max 最大値
*	@return  ランダムな倍率
*/
inline float GenerateRandomMultiplier(float min, float max)
{
	std::random_device rd;// 乱数生成器
	std::mt19937 gen(rd());// メルセンヌ・ツイスタ法
	std::uniform_real_distribution<float> dis(min, max);// 一様分布
	return dis(gen);// ランダムな倍率を生成
}

/*
*	@brief   物体同士が衝突したら押し戻す(球体同士)
*	@details 球体同士の衝突判定を行い、押し戻す処理を行う関数
*	@param   A 球体A
*	@param   B 球体B
*	@return  押し戻された位置
*/
inline DirectX::SimpleMath::Vector3 CheckHitOtherObject(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
{
	using namespace DirectX::SimpleMath;
	// 押し戻す処理
	Vector3 diffVector = A.Center - B.Center;	// Ａの中心とＢの中心との差分ベクトル（ＢからＡに向かうベクトル）…①
	float distance = diffVector.Length();	// Ａの中心とＢの中心との距離（①の長さ）…②
	float sumRadius = A.Radius + B.Radius;	// Ａの半径とＢの半径の合計…③
	float penetrationDistance = sumRadius - distance;// （ＡがＢに）めり込んだ距離（③－②）…④
	diffVector.Normalize();// ①を正規化する…⑤
	Vector3 pushBackVec = diffVector * penetrationDistance;// 押し戻すベクトルを計算する（⑤と④で表現する）…⑥
	Vector3 newPosition = A.Center + pushBackVec;// ⑥を使用して、Ａの座標とＡのコライダー座標を更新する（実際に押し戻す）
	A.Center = newPosition;// Ａの座標を更新する
	return newPosition;// 更新されたＡの座標を返す
}
/*
*	@brief   オブジェクト同士が衝突したら押し戻す(境界球と境界ボックスの場合）
*	@details 境界球と境界ボックスの衝突判定を行い、押し戻す処理を行う関数
*	@param   A 球体A
*	@param   B ボックスB
*	@return  押し戻された位置
*/
inline DirectX::SimpleMath::Vector3 CheckHitWall(DirectX::BoundingSphere A, DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	Vector3 pushBackVec = Vector3::Zero;// 押し戻しベクトルを計算
	Vector3 closestPoint; // ボックスの最も近い点
	// 各軸でクランプして、最も近い位置を取得
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));// X軸
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));// Y軸
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));// Z軸
	Vector3 diffVector = A.Center - closestPoint;// 球体の中心と最も近い点のベクトル差
	float distance = diffVector.Length();// 距離を計算
	if (distance < A.Radius)// 距離が球体の半径より小さい場合は押し戻し処理
	{
		float penetrationDistance = A.Radius - distance;// 押し戻し量を計算 (正規化して押し戻しベクトルを作成)
		diffVector.Normalize();// 正規化
		pushBackVec = diffVector * penetrationDistance;// 押し戻しベクトルを計算
		Vector3 newPosition = A.Center + pushBackVec; // 新しい位置を計算
		A.Center = newPosition; // 球体の位置を更新
		return newPosition; // 更新された位置を返す
	}
	return A.Center;// 衝突していない場合は元の位置を返す
}

