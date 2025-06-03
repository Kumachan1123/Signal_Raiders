/*
*	@file SpecialBullet.cpp
*	@brief 敵弾クラス(特殊弾)
*/
#include <pch.h>
#include "SpecialBullet.h"
/*
*	@brief コンストラクタ
*	@details 敵弾クラス(特殊弾)のコンストラクタ
*	@param なし
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
*	@details 各種ポインターをnullptrに設定
*	@param なし
*	@return なし
*/
SpecialBullet::~SpecialBullet()
{
	// 共通リソースのポインターをnullptrに設定
	m_pCommonResources = nullptr;
	// 敵弾のポインターをnullptrに設定
	m_pEnemyBullet = nullptr;
}
/*
*	@brief 初期化関数
*	@details 敵弾クラス(特殊弾)の初期化関数
*	@param なし
*	@return なし
*/
void SpecialBullet::Initialize()
{
	// 弾の座標を設定
	m_position = m_pEnemyBullet->GetPosition();
	// 弾の速度を設定
	m_velocity = m_pEnemyBullet->GetVelocity();
	// 弾の方向を設定
	m_direction = m_pEnemyBullet->GetDirection();
	// 回転弾の角度を設定
	m_spiralAngle = m_pEnemyBullet->GetAngle();
	// 弾の境界球を設定
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();
}
/*
*	@brief 更新関数
*	@details 敵弾クラス(特殊弾)の更新関数
*	@param float elapsedTime 更新時間
*	@return なし
*/
void SpecialBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// オーディオマネージャーの更新
	m_pCommonResources->GetAudioManager()->Update();
	// 時間の加算
	m_time += elapsedTime;
	// 経過時間を保存
	m_elapsedTime = elapsedTime;
	// 時計回りに回転するための角度
	m_spiralAngle += m_rotationSpeed * elapsedTime;
	// XY平面上で円運動 (時計回り)
	// X座標のオフセット
	float xOffset = cosf(m_spiralAngle) * m_distance;
	// Z座標のオフセット
	float zOffset = sinf(m_spiralAngle) * m_distance;
	// もともとのY座標の動きは変更しない
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);
	// 子オブジェクトを展開
	Expand();
	// 子オブジェクトを発射
	Shot();
	// 子オブジェクトを収納
	StopExpand();
	// 子オブジェクトを戻す
	ComeBack();
	// プレイヤーに向かいつつスパイラルを描いて移動
	m_position = m_basePos + m_positionOffSet;
	// 境界球の中心座標を弾の座標に更新
	m_boundingSphere.Center = m_position;
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)m_pEnemyBullet->SetIsShot(true);// 待機時間を過ぎたら発射
	// 弾の方向を更新
	m_pEnemyBullet->SetDirection(m_direction);
	// 弾の速度を更新
	m_pEnemyBullet->SetVelocity(m_velocity);
	// 弾の座標を更新
	m_pEnemyBullet->SetPosition(m_position);
	// 弾の境界球を更新
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);
}
/*
*	@brief 回転弾を展開する
*	@details 回転弾を展開する関数
*	@param なし
*	@return なし
*/
void SpecialBullet::Expand()
{
	// 展開していない場合は何もしない
	if (!m_pEnemyBullet->GetIsExpand())return;
	// 回転速度を設定
	m_rotationSpeed = 1.0f;
	// 弾の距離を補完
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);
	// 弾の高さを補完
	m_height = 2.0f;
	// SEが再生されていない場合
	if (!m_isPlayChargeSE)
	{
		// 展開音を再生
		m_pCommonResources->GetAudioManager()->PlaySound("ChargeSpecial", m_seVolume);
		// SE再生フラグを立てる
		m_isPlayChargeSE = true;
	}
}
/*
*	@brief 回転弾を発射する
*	@details 回転弾を発射する関数
*	@param なし
*	@return なし
*/
void SpecialBullet::Shot()
{
	// 発射していない場合は何もしない
	if (!m_pEnemyBullet->GetIsShot()) return;
	// 回転速度を設定
	m_rotationSpeed = 3.0f;
	// 弾の距離を補完
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);
	// 基準点を目的地に向かって線形補完
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetCurrentTarget(), m_elapsedTime * 2);
	// SEが再生されていない場合
	if (!m_isPlayShotSE)
	{
		// 発射音を再生
		m_pCommonResources->GetAudioManager()->PlaySound("SpecialAttack", m_seVolume);
		// SE再生フラグを立てる
		m_isPlayShotSE = true;
	}
}
/*
*	@brief 回転弾の展開をやめる
*	@details 回転弾の展開をやめる関数
*	@param なし
*	@return なし
*/
void SpecialBullet::StopExpand()
{
	// 展開していない場合は何もしない
	if (m_pEnemyBullet->GetIsExpand())return;
	// 回転を止める
	m_rotationSpeed = 0.0f;
	// 弾の距離を補完
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);
	// 弾の高さを補正
	m_height = 1.50f;
}
/*
*	@brief 回転弾を自分の周りに戻す
*	@details 回転弾を自分の周りに戻す関数
*	@param なし
*	@return なし
*/
void SpecialBullet::ComeBack()
{
	// 発射していない場合は何もしない
	if (m_pEnemyBullet->GetIsShot()) return;
	// 基準点を親が向いている方向に動かす
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);
	// 基準点を目的地に向かって線形補完
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetEnemyPosition(), m_elapsedTime * 50);
}
