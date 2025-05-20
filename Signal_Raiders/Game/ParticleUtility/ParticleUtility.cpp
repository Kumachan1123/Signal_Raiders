/*
*	@file	ParticleUtility.cpp
*	@brief	パーティクルユーティリティクラス
*/
#include "pch.h"
#include "ParticleUtility.h"

const static float ENDLESS = -100.0f;// 無限大の値

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
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 rotateAccele,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
	Type type)
{
	m_startLife = m_life = life;// 寿命を設定
	m_position = pos;// 座標を設定
	m_startPosition = pos;// 初期座標を設定
	m_velocity = velocity;// 速度を設定
	m_accele = accele;// 加速度を設定
	m_rotateAccele = rotateAccele;// 回転加速度を設定
	m_rotate = rotate;// 回転を設定
	m_startScale = m_nowScale = startScale;// 現在のスケールを設定
	m_endScale = endScale;// 最終スケールを設定
	m_startColor = m_nowColor = startColor;// 現在の色を設定
	m_endColor = endColor;// 最終色を設定
	m_type = type;// タイプを設定
	m_elapsedTime = 0.0f;// 経過時間を初期化
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
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);// スケールの補間
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);// 色の補間
	m_velocity += m_accele * elapsedTime;// 加速度の適用（破片が飛び散った後、ゆっくりと減速）
	m_position += m_velocity * elapsedTime;// 座標更新
	m_life -= elapsedTime;// ライフの減少
	SwitchType(elapsedTime);// タイプによって処理を変更
	if (m_life < 0.0f)return false;// ライフが 0 以下になったら false を返す
	else return true;// ライフが 0 より大きい場合は true を返す
}
/*
*	@brief	パーティクルの種類の切替
*	@detail タイプによって処理を変更する
*	@param float elapsedTime 経過時間
*	@return なし
*/
void ParticleUtility::SwitchType(float elapsedTime)
{
	switch (m_type)// タイプによって処理を変更
	{
	case ParticleUtility::Type::ENEMYTRAIL:// 敵の弾の軌跡
	case ParticleUtility::Type::PLAYERTRAIL:// プレイヤーの弾の軌跡
		Trail(elapsedTime);// 弾の軌跡の処理
		break;
	case ParticleUtility::Type::BARRIERBREAK:// バリア破壊
		BarrierDestroyed(elapsedTime);// バリア破壊の処理
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
	UNREFERENCED_PARAMETER(elapsedTime);// 未使用変数の警告を無視
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
	std::random_device seed;// シード値を生成
	std::default_random_engine engine(seed());// エンジンを初期化
	if (m_life < m_startLife * 0.75f)// ライフが 3/4 未満の時、微細なランダム動きを追加
	{
		// ランダムな力の範囲
		std::uniform_real_distribution<> dist(-3.0f, 3.0f);// -3.0f から 3.0f の範囲で乱数を生成
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;// X 軸方向の速度に加算
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;// Y 軸方向の速度に加算
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime * 0.5f; // Z 軸方向の速度に加算
		m_velocity *= 0.95f; // 減速効果を適用
	}
	if (m_life < m_startLife * 0.5f)// ライフが 1/2 未満の時、破片がより急激に減速しつつ、ランダムな動きを強化(少し強めのランダムな力)
	{
		std::uniform_real_distribution<> dist(-5.0f, 5.0f);// -5.0f から 5.0f の範囲で乱数を生成
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;		 // X 軸方向の速度に加算
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 2;	 // Y 軸方向の速度に加算
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;		 // Z 軸方向の速度に加算
		m_velocity *= 0.8f; // 速度をさらに抑える
	}
	if (m_life < m_startLife * 0.75f)// 寿命が 1/2 未満の場合に回転を追加
	{
		float rotationSpeed = 5.0f * (1.0f - m_life / m_startLife);// 回転角速度を決定（寿命に応じて徐々に速くなる）
		SimpleMath::Vector3 relativePos = m_position - m_startPosition;// 原点からの相対位置を計算
		// 回転行列を使用して相対位置を回転
		float sinTheta = std::sin(rotationSpeed * elapsedTime);// サイン値を計算
		float cosTheta = std::cos(rotationSpeed * elapsedTime); // コサイン値を計算
		// 相対位置を回転
		float newX = relativePos.x * cosTheta - relativePos.z * sinTheta;// 新しい X 座標
		float newZ = relativePos.x * sinTheta + relativePos.z * cosTheta; // 新しい Z 座標
		// 更新された相対位置を元に戻す
		relativePos.x = newX; // 新しい X 座標を設定
		relativePos.z = newZ; // 新しい Z 座標を設定
		m_position = m_startPosition + relativePos;// 新しい位置を設定
	}
	if (m_life < m_startLife * 0.25f)// ライフが 1/4 未満の時、破片がほとんど消える直前に動きを加速
	{
		// 最後の激しいランダムな力
		std::uniform_real_distribution<> dist(-10.0f, 10.0f);// -10.0f から 10.0f の範囲で乱数を生成
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime; // X 軸方向の速度に加算
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime; // Y 軸方向の速度に加算
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime; // Z 軸方向の速度に加算
		m_velocity *= 1.2f;// 一瞬だけ加速感を与える
	}
}