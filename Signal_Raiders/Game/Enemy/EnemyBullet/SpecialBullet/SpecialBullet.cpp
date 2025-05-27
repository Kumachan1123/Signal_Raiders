/*
*	@file SpecialBullet.cpp
*	@brief 敵弾クラス(特殊弾)
*/
#include <pch.h>
#include "SpecialBullet.h"
/*
*	@brief コンストラクタ
*	@return なし
*/
SpecialBullet::SpecialBullet()
	: m_pEnemyBullet(nullptr)// 敵弾ポインター
	, m_pCommonResources(nullptr)// 共通リソース
	, m_rotationSpeed(0.0f)// 弾の回転速度
	, m_distance(5.0f)// 弾の敵との距離
	, m_height(0.0f)// 弾の高さ
	, m_spiralAngle(0.0f)// 回転弾の角度
	, m_elapsedTime(0.0f)// 経過時間
	, m_time(0.0f)// 時間
	, m_basePos(DirectX::SimpleMath::Vector3::Zero)// 弾の基準位置
	, m_positionOffSet(DirectX::SimpleMath::Vector3::Zero)// 弾の位置オフセット
	, m_currentTarget(DirectX::SimpleMath::Vector3::Zero)// 現在のターゲットの位置
	, m_seVolume(0.0f)// SE音量
	, m_isPlayChargeSE(false)// チャージSE再生フラグ
	, m_isPlayShotSE(false)// 発射SE再生フラグ
{
}
/*
*	@brief デストラクタ
*	@return なし
*/
SpecialBullet::~SpecialBullet() {/*do nothing*/ }
/*
*	@brief 初期化関数
*	@return なし
*/
void SpecialBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// 弾の座標
	m_velocity = m_pEnemyBullet->GetVelocity();// 弾の速度
	m_direction = m_pEnemyBullet->GetDirection();// 弾の方向
	m_spiralAngle = m_pEnemyBullet->GetAngle();// 回転弾の角度
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// 「弾」境界球
}
/*
*	@brief 更新関数
*	@param[in] float elapsedTime 更新時間
*	@return なし
*/
void SpecialBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources->GetAudioManager()->Update();// オーディオマネージャーの更新
	m_time += elapsedTime;// 時間の加算
	m_elapsedTime = elapsedTime;// 経過時間を保存
	m_spiralAngle += m_rotationSpeed * elapsedTime;	// 時計回りに回転するための角度
	// XY平面上で円運動 (時計回り)
	float xOffset = cosf(m_spiralAngle) * m_distance;// X座標のオフセット
	float zOffset = sinf(m_spiralAngle) * m_distance;// Z座標のオフセット
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);	// もともとのY座標の動きは変更しない
	Expand();// 子オブジェクトを展開
	Shot();// 子オブジェクトを発射
	StopExpand();// 子オブジェクトを収納
	ComeBack();// 子オブジェクトを戻す
	m_position = m_basePos + m_positionOffSet;	// プレイヤーに向かいつつスパイラルを描いて移動
	m_boundingSphere.Center = m_position;	// 境界球の中心座標を弾の座標に更新
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)	// 弾の寿命に応じてフラグを切り替える
		m_pEnemyBullet->SetIsShot(true);// 弾を発射する
	// 弾の各種情報を更新
	m_pEnemyBullet->SetDirection(m_direction);// 弾の方向
	m_pEnemyBullet->SetVelocity(m_velocity);// 弾の速度
	m_pEnemyBullet->SetPosition(m_position);// 弾の座標
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// 「弾」境界球
}
/*
*	@brief 回転弾を展開する
*	@return なし
*/
void SpecialBullet::Expand()
{
	if (!m_pEnemyBullet->GetIsExpand())return;// 展開していない場合は何もしない
	m_rotationSpeed = 1.0f; // 速度調整用（値を大きくすると速く回転する）
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);// 弾の距離を補完
	m_height = 2.0f; // 弾の高さを補完
	if (!m_isPlayChargeSE)// SEが再生されていない場合
	{
		m_pCommonResources->GetAudioManager()->PlaySound("ChargeSpecial", m_seVolume);// SEを再生
		m_isPlayChargeSE = true;// SE再生フラグを立てる
	}

}
/*
*	@brief 回転弾を発射する
*	@return なし
*/
void SpecialBullet::Shot()
{
	if (!m_pEnemyBullet->GetIsShot()) return;// 発射していない場合は何もしない
	m_rotationSpeed = 3.0f; // 速度調整用（値を大きくすると速く回転する）
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);// 弾の距離を補完
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetCurrentTarget(), m_elapsedTime * 2);// 基準点を目的地に向かって線形補完
	if (!m_isPlayShotSE)// SEが再生されていない場合
	{
		m_pCommonResources->GetAudioManager()->PlaySound("SpecialAttack", m_seVolume);// SEを再生
		m_isPlayShotSE = true;// SE再生フラグを立てる
	}

}
/*
*	@brief 回転弾の展開をやめる
*	@return なし
*/
void SpecialBullet::StopExpand()
{
	if (m_pEnemyBullet->GetIsExpand())return;// 展開していない場合は何もしない
	m_rotationSpeed = 0.0f; // 速度調整用（値を大きくすると速く回転する）
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);// 弾の距離を補完
	m_height = 1.50f; // 弾の高さを補正
}
/*
*	@brief 回転弾を自分の周りに戻す
*	@return なし
*/
void SpecialBullet::ComeBack()
{
	if (m_pEnemyBullet->GetIsShot()) return;// 発射していない場合は何もしない
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);	// 基準点を親が向いている方向に動かす
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetEnemyPosition(), m_elapsedTime * 50);// 基準点を目的地に向かって線形補完
}
