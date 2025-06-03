/*
*	@file	ParticleUtility.cpp
*	@brief	パーティクルユーティリティクラス
*/
#include <pch.h>
#include "ParticleUtility.h"
/*
*	@brief	コンストラクタ
*	@detail パーティクルの初期化を行う
*	@param float life パーティクルの寿命
*	@param DirectX::SimpleMath::Vector3 pos パーティクルの位置
*	@param DirectX::SimpleMath::Vector3 velocity パーティクルの速度
*	@param DirectX::SimpleMath::Vector3 accele パーティクルの加速度
*	@param DirectX::SimpleMath::Vector3 rotateAccele パーティクルの回転加速度
*	@param DirectX::SimpleMath::Vector3 rotate パーティクルの回転
*	@param DirectX::SimpleMath::Vector3 startScale パーティクルの初期スケール
*	@param DirectX::SimpleMath::Vector3 endScale パーティクルの最終スケール
*	@param DirectX::SimpleMath::Color startColor パーティクルの初期色
*	@param DirectX::SimpleMath::Color endColor パーティクルの最終色
*	@param Type type パーティクルのタイプ
*	@return なし
*/
ParticleUtility::ParticleUtility(
	float life,
	const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& accele,
	const DirectX::SimpleMath::Vector3& rotateAccele,
	const DirectX::SimpleMath::Vector3& rotate,
	const DirectX::SimpleMath::Vector3& startScale,
	const DirectX::SimpleMath::Vector3& endScale,
	const DirectX::SimpleMath::Color& startColor,
	const DirectX::SimpleMath::Color& endColor,
	Type type)
{
	// 寿命を設定
	m_startLife = m_life = life;
	// 座標を設定
	m_position = pos;
	// 初期座標を設定
	m_startPosition = pos;
	// 速度を設定
	m_velocity = velocity;
	// 加速度を設定
	m_accele = accele;
	// 回転加速度を設定
	m_rotateAccele = rotateAccele;
	// 回転を設定
	m_rotate = rotate;
	// 現在のスケールを設定
	m_startScale = m_nowScale = startScale;
	// 最終スケールを設定
	m_endScale = endScale;
	// 現在の色を設定
	m_startColor = m_nowColor = startColor;
	// 最終色を設定
	m_endColor = endColor;
	// タイプを設定
	m_type = type;
	// 経過時間を初期化
	m_elapsedTime = 0.0f;
}
/*
*	@brief	デストラクタ
*	@detail 何もしない
*	@param なし
*	@return なし
*/
ParticleUtility::~ParticleUtility() {/* do nothing. */ }
/*
*	@brief	パーティクルの更新
*	@detail パーティクルの更新を行う
*	@param float elapsedTime 経過時間
*	@return bool true:まだ再生する false:再生しない
*/
bool ParticleUtility::Update(float elapsedTime)
{
	using namespace DirectX;
	// スケールの補間
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	// 色の補間
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);
	// 加速度の適用（破片が飛び散った後、ゆっくりと減速）
	m_velocity += m_accele * elapsedTime;
	// 座標更新
	m_position += m_velocity * elapsedTime;
	// ライフの減少
	m_life -= elapsedTime;
	// タイプによって処理を変更
	SwitchType(elapsedTime);
	// ライフが0以下になったらfalseを返し、ライフが0より大きい場合はtrueを返す
	if (m_life < 0.0f)return false;
	else return true;
}
/*
*	@brief	パーティクルの種類の切替
*	@detail タイプによって処理を変更する
*	@param float elapsedTime 経過時間
*	@return なし
*/
void ParticleUtility::SwitchType(float elapsedTime)
{
	// タイプによって処理を変更
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:// 敵の弾の軌跡
	case ParticleUtility::Type::PLAYERTRAIL:// プレイヤーの弾の軌跡
		// 弾の軌跡の処理
		Trail(elapsedTime);
		break;
	case ParticleUtility::Type::BARRIERBREAK:// バリア破壊
		// バリア破壊の処理
		BarrierDestroyed(elapsedTime);
		break;
	default:// その他
		break;
	}
}

/*
*	@brief	軌跡の処理
*	@detail 軌跡の更新処理(ここでは何もしない)
*	@param float elapsedTime 経過時間
*	@return なし
*/
void ParticleUtility::Trail(float elapsedTime)
{
	// 未使用変数の警告を無視
	UNREFERENCED_PARAMETER(elapsedTime);
}
/*
*	@brief	バリア破壊の処理
*	@detail バリア破壊の更新処理
*	@param float elapsedTime 経過時間
*	@return なし
*/
void ParticleUtility::BarrierDestroyed(float elapsedTime)
{
	using namespace DirectX;
	// シード値を生成
	std::random_device seed;
	// エンジンを初期化
	std::default_random_engine engine(seed());
	// ライフが 3/4 未満の時、微細なランダム動きを追加
	if (m_life < m_startLife * 0.75f)
	{
		// -3.0f から 3.0f の範囲で乱数を生成
		std::uniform_real_distribution<> dist(-3.0f, 3.0f);
		// X 軸方向の速度に加算
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		// Y 軸方向の速度に加算
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		// Z 軸方向の速度に加算
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		// 減速効果を適用
		m_velocity *= 0.95f;
	}
	// ライフが 1/2 未満の時、破片がより急激に減速しつつ、ランダムな動きを強化(少し強めのランダムな力)
	if (m_life < m_startLife * 0.5f)
	{
		// -5.0f から 5.0f の範囲で乱数を生成
		std::uniform_real_distribution<> dist(-5.0f, 5.0f);
		// X 軸方向の速度に加算
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		// Y 軸方向の速度に加算
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 2;
		// Z 軸方向の速度に加算
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;
		// 速度をさらに抑える
		m_velocity *= 0.8f;
	}
	// 寿命が 1/2 未満の場合に回転を追加
	if (m_life < m_startLife * 0.75f)
	{
		// 回転角速度を決定（寿命に応じて徐々に速くなる）
		float rotationSpeed = 5.0f * (1.0f - m_life / m_startLife);
		// 原点からの相対位置を計算
		SimpleMath::Vector3 relativePos = m_position - m_startPosition;
		// 回転行列を使用して相対位置を回転
		// サイン値を計算
		float sinTheta = std::sin(rotationSpeed * elapsedTime);
		// コサイン値を計算
		float cosTheta = std::cos(rotationSpeed * elapsedTime);
		// 相対位置を回転
		// 新しいX座標
		float newX = relativePos.x * cosTheta - relativePos.z * sinTheta;
		// 新しいZ座標
		float newZ = relativePos.x * sinTheta + relativePos.z * cosTheta;
		// 更新された相対位置を元に戻す
		// 新しいX座標を設定
		relativePos.x = newX;
		// 新しいZ座標を設定
		relativePos.z = newZ;
		// 新しい位置を設定
		m_position = m_startPosition + relativePos;
	}
	// ライフが 1/4 未満の時、破片がほとんど消える直前に動きを加速
	if (m_life < m_startLife * 0.25f)
	{
		// -10.0f から 10.0f の範囲で乱数を生成
		std::uniform_real_distribution<> dist(-10.0f, 10.0f);
		// X 軸方向の速度に加算
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		// Y 軸方向の速度に加算
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime;
		// Z 軸方向の速度に加算
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;
		// 一瞬だけ加速感を与える
		m_velocity *= 1.2f;
	}
}