/*
	@file	Boss.cpp
	@brief	ボスクラス
*/
#include "pch.h"
#include "Boss.h"
//// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief	コンストラクタ
*	@param[in] pBoss		ボスベースのポインタ
*	@param[in] resources	共通リソース
*	@return	なし
*/
Boss::Boss(BossBase* pBoss, CommonResources* commonResources)
	: m_commonResources(commonResources)// 共通リソース
	, m_pBossBase(pBoss)// ボスのポインタ
	, m_bossBS{}// 境界球の初期化
	, m_pBossModel{}// ボスモデル
	, m_pHPBar{}// HPバー
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
Boss::~Boss() {  }
/*
*	@brief	初期化処理
*	@return	なし
*/
void Boss::Initialize()
{
	m_pBossModel = std::make_unique<BossModel>();	// ボスモデル生成
	m_pBossModel->Initialize(m_commonResources);// ボスモデル初期化
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_BOSS_RADIUS);// 通常時ボスの当たり判定を設定
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::BOSS_SHIELD_RADIUS);// シールド展開時のボスの当たり判定を設定
	m_pBossBase->SetBulletSize(EnemyParameters::BOSS_BULLET_SIZE);// 弾のサイズを設定
	m_pBossBase->SetToPlayerDamage(EnemyParameters::BOSS_DAMAGE);// ボスがプレイヤーに与えるダメージを設定
}
/*
*	@brief	更新処理
*	@return	なし
*/
void Boss::ChangeState()
{
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());// モデルのアニメーション更新
}
/*
*	@brief	描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void Boss::Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキスト
	auto states = m_commonResources->GetCommonStates();// ステート
	// ワールド行列を設定
	Matrix enemyWorld = Matrix::CreateScale(m_pBossBase->GetScale())// スケール
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())// 回転
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());// 位置
	// シールドのワールド行列を設定
	Matrix sheildWorld = Matrix::CreateScale(m_pBossBase->GetScale() * 3)// スケール(シールドの大きさ)
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
void Boss::BulletPositioning()
{
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation())// 弾の発射位置を設定
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);// 中央の座標に回転を適用
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);// 左の座標に回転を適用
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);// 右の座標に回転を適用
}
/*
*	@brief	弾の生成
*	@return	なし
*/
void Boss::CreateBullet()
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE);// 弾のサイズを設定
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);// 弾を発射したオブジェクトを設定
	switch (m_pBossBase->GetBulletType())	// Enemiesクラスで設定した弾のタイプによって処理を分岐
	{
	case BossBase::BossBulletType::STAGE_1:// 通常弾
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// 中央の弾を発射
		break;
	case BossBase::BossBulletType::STAGE_2:// 二発

		CreateLeftBullet(EnemyBullet::BulletType::NORMAL);// 左の弾を発射
		CreateRightBullet(EnemyBullet::BulletType::NORMAL);// 右の弾を発射
		break;
	case BossBase::BossBulletType::STAGE_3:// 三発
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// 中央の弾を発射
		CreateLeftBullet(EnemyBullet::BulletType::NORMAL);// 左の弾を発射
		CreateRightBullet(EnemyBullet::BulletType::NORMAL);// 右の弾を発射
		break;

	}
}
/*
*	@brief	中央から弾を発射
*	@param[in] type 弾の種類
*/
void Boss::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// 弾を生成
}
/*
*	@brief	左の弾を発射
*	@param[in] type 弾の種類
*/
void Boss::CreateLeftBullet(EnemyBullet::BulletType type)
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeft, m_bulletDirection);// 弾を生成
}
/*
*	@brief 右の弾を発射
*	@param[in] type 弾の種類
*/
void Boss::CreateRightBullet(EnemyBullet::BulletType type)
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRight, m_bulletDirection);// 弾を生成
}