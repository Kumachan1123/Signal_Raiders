/*
*	@file	LastBoss.cpp
*	@brief	ラスボスクラス
*/
#include <pch.h>
#include "LastBoss.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"
/*
*	@brief	コンストラクタ
*	@details ラスボスクラスのコンストラクタ
*	@param pBoss		ボスベースのポインタ
*	@param resources	共通リソース
*	@return	なし
*/
LastBoss::LastBoss(BossBase* pBoss, CommonResources* commonResources)
	: m_pCommonResources(commonResources)// 共通リソース
	, m_pBossBase(pBoss)// ボスのポインタ
	, m_bossBS{}// 境界球の初期化
	, m_time(0.0f)// 時間
	, m_position{}// 位置
	, m_velocity{}// 速度
	, m_rotate{}// 回転
	, m_matrix{}// マトリクス
	, m_bulletType(BulletType::NORMAL)// 弾の種類
{
}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return	なし
*/
LastBoss::~LastBoss()
{
	m_pCommonResources = nullptr;	// 共通リソースのポインタをnullptrに設定
	m_pBossBase = nullptr;	// ボスのポインタをnullptrに設定
	m_pBossModel = nullptr;	// ラスボスモデルのポインタをnullptrに設定
}
/*
*	@brief	初期化処理
*	@details ラスボスのモデルを生成し、初期化を行う
*	@param	なし
*	@return	なし
*/
void LastBoss::Initialize()
{
	m_pBossModel = std::make_unique<LastBossModel>();	// ラスボスモデル生成
	m_pBossModel->Initialize(m_pCommonResources);// ラスボスモデル初期化 
	m_position = EnemyParameters::INITIAL_LASTBOSS_POSITION;// ラスボスの初期位置を設定
	m_pBossBase->SetPosition(m_position);// ベースクラスに初期位置を設定
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_LASTBOSS_RADIUS);// 通常時のラスボスの当たり判定を設定
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::LASTBOSS_SHIELD_RADIUS);// シールド展開時のラスボスの当たり判定を設定
	m_pBossBase->SetBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);// 弾のサイズを設定
	switch (m_pBossBase->GetBulletType())	// BulletType()とあるが、ここではステージごとのラスボスの攻撃力を分岐
	{
	case BossBase::BossBulletType::STAGE_4:
		m_pBossBase->SetToPlayerDamage(EnemyParameters::BOSS_DAMAGE);// ボスがプレイヤーに与えるダメージを設定
		break;
	case BossBase::BossBulletType::STAGE_5:
		m_pBossBase->SetToPlayerDamage(EnemyParameters::LASTBOSS_DAMAGE);// ボスがプレイヤーに与えるダメージを設定
		break;
	}
	m_pBossBase->SetBarrierBreakSize(EnemyParameters::LASTBOSS_BARRIERBREAK_SIZE);// バリア破壊パーティクルのサイズ設定
	m_pBossBase->SetDeadEffectSize(EnemyParameters::LASTBOSS_DEADEFFECT_SCALE);// 死亡エフェクトのサイズ


}
/*
*	@brief	更新処理
*	@details ラスボスの状態を更新する
*	@param	なし
*	@return	なし
*/
void LastBoss::ChangeState()
{
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());// モデルのアニメーション更新
}
/*
*	@brief	描画処理
*	@details ラスボスのモデルとシールド、HPバーを描画する
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return	なし
*/
void LastBoss::Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキスト
	auto states = m_pCommonResources->GetCommonStates();// ステート
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
*	@details ラスボスの弾の発射位置を決定する
*	@param	なし
*	@return	なし
*/
void LastBoss::BulletPositioning()
{
	using namespace DirectX::SimpleMath;
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation())// 弾の発射位置を設定
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	m_bulletPosLeftDown = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTDOWN_GUN_OFFSET, transform);// 左下の座標に回転を適用
	m_bulletPosLeftUp = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTUP_GUN_OFFSET, transform);// 左上の座標に回転を適用
	m_bulletPosRightDown = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTDOWN_GUN_OFFSET, transform);// 右下の座標に回転を適用
	m_bulletPosRightUp = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTUP_GUN_OFFSET, transform);// 右上の座標に回転を適用

}
/*
*	@brief	弾を生成する
*	@details ラスボスの弾を生成する
*	@param	なし
*	@return	なし
*/
void LastBoss::CreateBullet()
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);// 弾のサイズを設定
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);// 弾を発射したオブジェクトを設定
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(BulletType::NORMAL);// 弾の種類を設定
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftUp, m_bulletDirection);// 弾を生成（左上）
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftDown, m_bulletDirection);// 弾を生成（左下）
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightUp, m_bulletDirection);// 弾を生成（右上）
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightDown, m_bulletDirection);// 弾を生成（右下）

}

