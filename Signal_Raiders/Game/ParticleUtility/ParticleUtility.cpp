/*
	@file	ParticleUtility.cpp
	@brief	パーティクルユーティリティクラス
*/
#include "pch.h"
#include "ParticleUtility.h"
#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <cmath>
#include <random>

using namespace DirectX;

const static float ENDLESS = -100.0f;

/// <summary>
/// コンストラクタ
/// 引数は初期設定用の数値
/// </summary>
ParticleUtility::ParticleUtility(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 rotateAccele,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
	Type type)
{
	m_startLife = m_life = life;
	m_position = pos;
	m_startPosition = pos;
	m_velocity = velocity;
	m_accele = accele;
	m_rotateAccele = rotateAccele;
	m_rotate = rotate;
	m_startScale = m_nowScale = startScale;
	m_endScale = endScale;
	m_startColor = m_nowColor = startColor;
	m_endColor = endColor;
	m_type = type;
	m_elapsedTime = 0.0f;
}

ParticleUtility::~ParticleUtility()
{
}

/// <summary>
/// 更新関数
/// </summary>
bool ParticleUtility::Update(float elapsedTime)
{

	// スケールと色の変化
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// 加速度の適用（破片が飛び散った後、ゆっくりと減速）
	m_velocity += m_accele * elapsedTime;

	// 座標更新
	m_position += m_velocity * elapsedTime;

	// ライフの減少
	m_life -= elapsedTime;

	// タイプによって処理を変更
	SwitchType(elapsedTime);
	if (m_life < 0.0f)
	{
		return false;
	}
	return true;
}


void ParticleUtility::SwitchType(float elapsedTime)
{
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:
	case ParticleUtility::Type::PLAYERTRAIL:
		Trail(elapsedTime);
		break;
	case ParticleUtility::Type::BARRIERDESTROYED:
		BarrierDestroyed(elapsedTime);
		break;
	default:
		break;
	}
}

// タイプ::弾の軌跡の処理
void ParticleUtility::Trail(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

}

void ParticleUtility::BarrierDestroyed(float elapsedTime)
{
	// ランダムな動きを追加するための設定
	std::random_device seed;
	std::default_random_engine engine(seed());

	// ライフが 3/4 未満の時、微細なランダム動きを追加
	if (m_life < m_startLife * 0.75f)
	{
		std::uniform_real_distribution<> dist(-3.0f, 3.0f); // ランダムな力の範囲
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;

		// 減速効果を適用
		m_velocity *= 0.95f; // 速度を減少
	}

	// ライフが 1/2 未満の時、破片がより急激に減速しつつ、ランダムな動きを強化
	if (m_life < m_startLife * 0.5f)
	{
		std::uniform_real_distribution<> dist(-5.0f, 5.0f); // 少し強めのランダムな力
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 2;
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;

		// 減速をさらに強調
		m_velocity *= 0.8f; // 速度をさらに抑える
	}

	// 寿命が 1/2 未満の場合に回転を追加
	if (m_life < m_startLife * 0.75f)
	{
		// 回転角速度を決定（寿命に応じて徐々に速くなる）
		float rotationSpeed = 5.0f * (1.0f - m_life / m_startLife); // 回転速度を寿命に比例

		// 原点からの相対位置を計算
		SimpleMath::Vector3 relativePos = m_position - m_startPosition;

		// 回転処理（Y軸回りの回転）
		float sinTheta = std::sin(rotationSpeed * elapsedTime);
		float cosTheta = std::cos(rotationSpeed * elapsedTime);

		float newX = relativePos.x * cosTheta - relativePos.z * sinTheta;
		float newZ = relativePos.x * sinTheta + relativePos.z * cosTheta;

		// 更新された相対位置を元に戻す
		relativePos.x = newX;
		relativePos.z = newZ;

		// 新しい位置を設定
		m_position = m_startPosition + relativePos;
	}

	// ライフが 1/4 未満の時、破片がほとんど消える直前に動きを加速
	if (m_life < m_startLife * 0.25f)
	{
		std::uniform_real_distribution<> dist(-10.0f, 10.0f); // 最後の激しいランダムな力
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime;
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;

		// 一瞬だけ加速感を与える
		m_velocity *= 1.2f; // 速度を増加
	}
}

