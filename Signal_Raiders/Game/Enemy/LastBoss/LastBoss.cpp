/*
	@file	LastBoss.cpp
	@brief	ラスボスクラス
*/
#include <pch.h>
#include "LastBoss.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief	コンストラクタ
*	@param[in] pBoss		ボスベースのポインタ
*	@param[in] resources	共通リソース
*	@return	なし
*/
LastBoss::LastBoss(BossBase* pBoss, CommonResources* commonResources)
	: m_commonResources(commonResources)// 共通リソース
	, m_pBossBase(pBoss)// ボスのポインタ
	, m_bossBS{}// 境界球の初期化
	, m_time(0.0f)// 時間
	, m_position{}// 位置
	, m_velocity{}// 速度
	, m_rotate{}// 回転
	, m_matrix{}// マトリクス
	, m_bulletType(EnemyBullet::BulletType::NORMAL)// 弾の種類
{
}
/*
*	@brief	デストラクタ
*	@return	なし
*/
LastBoss::~LastBoss() {  }
/*
*	@brief	初期化処理
*	@return	なし
*/
void LastBoss::Initialize()
{
	m_pBossModel = std::make_unique<LastBossModel>();	// ラスボスモデル生成
	m_pBossModel->Initialize(m_commonResources);// ラスボスモデル初期化 
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_LASTBOSS_RADIUS);// 通常時のラスボスの当たり判定を設定
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::LASTBOSS_SHIELD_RADIUS);// シールド展開時のラスボスの当たり判定を設定
	m_pBossBase->SetBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);// 弾のサイズを設定

}
/*
*	@brief	更新処理
*	@return	なし
*/
void LastBoss::ChangeState()
{
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());// モデルのアニメーション更新

}
/*
*	@brief	描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void LastBoss::Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキスト
	auto states = m_commonResources->GetCommonStates();// ステート
	// ワールド行列を設定
	Matrix enemyWorld = Matrix::CreateScale(m_pBossBase->GetScale())// スケール
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())// 回転
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());// 位置
	// シールドのワールド行列を設定
	Matrix sheildWorld = Matrix::CreateScale(m_pBossBase->GetScale() * 10)// スケール(シールドの大きさ)
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())// 回転
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());// 位置
	m_pBossBase->GetBossSheild()->SetPosition(m_bossBS.Center);// シールドの座標を設定
	m_pBossBase->GetBossSheild()->SetRotation(m_pBossBase->GetQuaternion());// シールドの回転を設定
	m_pBossModel->Render(context, states, enemyWorld, view, proj);// モデル描画
	m_pBossBase->GetBossSheild()->Render(context, states, sheildWorld, view, proj);// シールド描画
	Vector3 hpBarPos = m_pBossBase->GetPosition() - EnemyParameters::BOSS_HPBAR_OFFSET;// HPバーの位置を設定
	m_pBossBase->GetHPBar()->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));// HPバーのスケールを設定
	m_pBossBase->GetHPBar()->Render(view, proj, hpBarPos, m_rotate);// HPバー描画

}
/*
*	@brief	弾の位置設定
*	@return	なし
*/
void LastBoss::BulletPositioning()
{
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation())// 弾の発射位置を設定
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	m_bulletPosLeftDown = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTDOWN_GUN_OFFSET, transform);// 左下の座標に回転を適用
	m_bulletPosLeftUp = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTUP_GUN_OFFSET, transform);// 左上の座標に回転を適用
	m_bulletPosRightDown = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTDOWN_GUN_OFFSET, transform);// 右下の座標に回転を適用
	m_bulletPosRightUp = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTUP_GUN_OFFSET, transform);// 右上の座標に回転を適用

}

void LastBoss::CreateBullet()
{
	float angleOffset = XMConvertToRadians(EnemyParameters::BOSS_BULLET_ANGLE); // 30度の角度オフセット
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);// 弾のサイズを設定
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);// 弾を発射したオブジェクトを設定
	switch (m_pBossBase->GetBulletType())	// Enemiesクラスで設定した弾のタイプによって処理を分岐
	{

	case BossBase::BossBulletType::STAGE_4:// 二発

		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::NORMAL);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::NORMAL);// 右の弾を発射
		break;
	case BossBase::BossBulletType::STAGE_5:// 三発
		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::NORMAL);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::NORMAL);// 右の弾を発射
		break;
	}
}

/*
*	@brief	左の弾を発射
*	@param[in] angleOffset 角度オフセット
*	@param[in] type 弾の種類
*/
void LastBoss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);// 左方向
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftUp, leftDirection);// 弾を生成
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftDown, leftDirection);// 弾を生成
}
/*
*	@brief 右の弾を発射
*	@param[in] angleOffset 角度オフセット
*	@param[in] type 弾の種類
*/
void LastBoss::CreateRightBullet(float angleOffset, EnemyBullet::BulletType type)
{
	Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);// 右方向
	Vector3 rightDirection = Vector3::Transform(m_bulletDirection, rightRotation);
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightUp, rightDirection);// 弾を生成
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightDown, rightDirection);// 弾を生成
}