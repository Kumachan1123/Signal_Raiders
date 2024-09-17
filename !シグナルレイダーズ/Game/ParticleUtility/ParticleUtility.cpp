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
#include <cmath> // 新しく追加したヘッダ
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
	DirectX::SimpleMath::Vector3 accele2,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
	Type type)
{
	m_startLife = m_life = life;
	m_position = pos;
	m_velocity = velocity;
	m_accele = accele;
	m_accele2 = accele2;
	m_rotate = rotate;
	m_startScale = m_nowScale = startScale;
	m_endScale = endScale;
	m_startColor = m_nowColor = startColor;
	m_endColor = endColor;
	m_type = type;
}

ParticleUtility::~ParticleUtility()
{
}

/// <summary>
/// 更新関数
/// </summary>
bool ParticleUtility::Update(float elapsedTime)
{
	switch (m_type)
	{
		case ParticleUtility::Type::SPIRAL:
			Spiral(elapsedTime);
			break;
		case ParticleUtility::Type::ONIBI:
			Onibi(elapsedTime);
			break;
		case ParticleUtility::Type::KAMEHAMEHA:
			Kamehameha(elapsedTime);
			break;
		case ParticleUtility::Type::ENEMYTRAIL:
			EnemyTrail(elapsedTime);
			break;
		case ParticleUtility::Type::PLAYERTRAIL:
			PlayerTrail(elapsedTime);
			break;
		default:
			break;
	}
	if (m_life < 0.0f)
	{
		return false;
	}
	return true;
}

// タイプ::スパイラルの処理
void ParticleUtility::Spiral(float elapsedTime)
{
	// スケールと色の変化
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// 加速度の適用
	if (m_life < 0.6f)
	{
		m_velocity += m_accele2 * elapsedTime;
	}
	else
	{
		m_velocity += m_accele * elapsedTime;
	}

	// スパイラル動きの追加
	float spiralFactor = 0.1f; // スパイラルの強さ
	m_position.x += sin(m_life * 10.0f) * spiralFactor;
	m_position.y += cos(m_life * 10.0f) * spiralFactor;

	// 波打つ動きの追加
	float waveFactor = 0.5f; // 波の強さ
	m_position.z += sin(m_life * 20.0f) * waveFactor;

	// 座標更新
	m_position += m_velocity * elapsedTime;

	// 回転の適用
	m_rotate += SimpleMath::Vector3{ 45.0f, 0.0f, 0.0f } *elapsedTime;

	// ライフの減少
	m_life -= elapsedTime;
}

// タイプ::鬼火の処理
void ParticleUtility::Onibi(float elapsedTime)
{
	// スケールと色の変化を炎の燃え上がりを意識して設定
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(
		SimpleMath::Vector4(1.0f, 0.2f, 0.1f, 1.0f),  // 炎の基部（赤）
		SimpleMath::Vector4(1.0f, 1.0f, 0.1f, 0.5f),  // 炎の先端（黄色）
		1.0f - m_life / m_startLife
	);

	// 加速度を適用して上方向に勢いよく移動
	m_velocity += SimpleMath::Vector3(0.0f, 1.0f, 0.0f) * elapsedTime * 50.0f; // 上昇を強調
	m_velocity += m_accele * elapsedTime; // 元々の加速度も適用

	// 座標を更新
	m_position += m_velocity * elapsedTime;

	// 回転の適用は炎の揺らぎを再現
	m_rotate += SimpleMath::Vector3{ 45.0f, 15.0f, 0.0f } *elapsedTime; // 炎が揺れる感じを強調

	// ライフを減少
	m_life -= elapsedTime;
}

// タイプ::かめはめ波の処理
void ParticleUtility::Kamehameha(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 残りのライフ比率
	float lifeRatio = m_life / m_startLife;

	// 初期の集まり動作
	if (lifeRatio > 0.7f)
	{
		// 集まる速度を調整（集まる力を強くする）
		SimpleMath::Vector3 gatherForce = -m_position * 0.05f;
		m_velocity += gatherForce * elapsedTime;
	}
	else
	{
		// ビーム発射後の速度を増加させる
		m_velocity += m_accele * elapsedTime * 10.0f; // ビームの速度を速くする
	}

	// サイズの拡大処理 (ビームが発射されるときにサイズが大きくなる)
	if (lifeRatio < 0.5f)
	{
		m_nowScale = SimpleMath::Vector3::Lerp(m_nowScale, m_endScale * 3.0f, elapsedTime * 5.0f); // サイズを大きく
	}
	else
	{
		m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - lifeRatio);
	}

	// 色の変化 (ビーム発射時に明るい色に変わる)
	if (lifeRatio < 0.5f)
	{
		m_nowColor = SimpleMath::Color::Lerp(m_nowColor, Vector4(0, 0.5, 1, 10), elapsedTime * 2.0f); // 青白い光に変化
	}
	else
	{
		m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - lifeRatio);
	}

	// 座標の計算
	m_position += m_velocity * elapsedTime;

	// 回転を加える（必要に応じて調整可能）
	m_rotate += SimpleMath::Vector3{ 0.0f, 10.0f, 0.0f } *elapsedTime;

	// ライフを減らす
	m_life -= elapsedTime;
}

// タイプ::敵の弾の処理
void ParticleUtility::EnemyTrail(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// スケールと色の変化
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// 加速度の適用
	m_velocity += m_accele * elapsedTime;

	// 座標更新
	m_position += m_velocity * elapsedTime;

	// ライフの減少
	m_life -= elapsedTime;
}

// タイプ::軌跡の処理
void ParticleUtility::PlayerTrail(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// スケールと色の変化
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// 加速度の適用
	m_velocity += m_accele * elapsedTime;

	// 座標更新
	m_position += m_velocity * elapsedTime;

	// ライフの減少
	m_life -= elapsedTime;
}