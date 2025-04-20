/*
	@file	LastBoss.cpp
	@brief	ラスボスクラス
*/
#include <pch.h>
#include "LastBoss.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"


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
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// 攻撃のクールダウンタイム
	, m_specialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)// 特殊攻撃のクールダウンタイム
	, m_initSpecialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)	// 特殊攻撃のクールダウンタイム(初期化用)
	, m_bulletCooldown(EnemyParameters::ATTACK_INTERVAL)// 弾のクールダウンタイム
	, m_time(0.0f)// 時間
	, m_position{}// 位置
	, m_velocity{}// 速度
	, m_rotate{}// 回転
	, m_enemyBSToPlayerArea{}// 敵とPlayerとの一定範囲の当たり判定に使う
	, m_enemyBulletBS{}// 敵の弾の境界球
	, m_playerBS{}// プレイヤーの境界球
	, m_matrix{}// マトリクス
	, m_isDead(false)// 死亡フラグ
	, m_isHitToPlayer(false)// プレイヤーに当たったか
	, m_isHitToOtherEnemy(false)// 他の敵に当たったか
	, m_isEnemyHitByPlayerBullet(false)// 敵がプレイヤーの弾に当たったか
	, m_isPlayerHitByEnemyBullet(false)// プレイヤーが敵の弾に当たったか
	, m_canAttack(true)// 攻撃可能か
	, m_isAttack(false)// 攻撃中か
	, m_bulletType(EnemyBullet::BulletType::NORMAL)// 弾の種類
	, m_audioManager{ }// オーディオマネージャー
{
}
/*
*	@brief	デストラクタ
*	@return	なし
*/
LastBoss::~LastBoss() {  }// 弾を削除
/*
*	@brief	初期化処理
*	@return	なし
*/
void LastBoss::Initialize()
{
	//m_pBossBase->SetBossSheildType(BossSheild::BossShieldType::LASTBOSS);// ボスのシールドの種類を設定
	m_pBossModel = std::make_unique<LastBossModel>();	// ボスモデル生成 これはタイプによって分岐予定
	m_pBossModel->Initialize(m_commonResources);// ボスモデル初期化 これはタイプによって分岐予定
	//m_pBossSheild = std::make_unique<BossSheild>(BossSheild::BossType::LASTBOSS, m_maxHP, this);// シールド生成 これはタイプによって分岐予定
	//m_pBossSheild->Initialize(m_commonResources);// シールド初期化 これはタイプによって分岐予定

}
/*
*	@brief	更新処理
*	@param[in] elapsedTime 経過時間
*	@return	なし
*/
void LastBoss::Update(float elapsedTime)
{
	//m_cameraEye = m_pCamera->GetEyePosition();// カメラの位置を取得
	//m_cameraTarget = m_pCamera->GetTargetPosition();// カメラの注視点を取得
	//m_cameraUp = m_pCamera->GetUpVector();// カメラの上方向を取得
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());// モデルのアニメーション更新
	//m_pBossAI->Update(elapsedTime);// AIの更新
	//m_position = m_pBossAI->GetPosition();// 敵の座標を更新
	//m_audioManager->Update();// オーディオマネージャーの更新
	//m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// 攻撃のクールダウンタイムを取得
	//m_specialAttackCooldown -= elapsedTime;// 特殊攻撃のクールダウンタイムを減らす
	//this->ShootBullet();// 弾発射
	//m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// 境界球の中心座標を更新
	//this->BulletPositioning();// 弾の位置設定
	//m_pHPBar->SetCurrentHP(m_currentHP);// HPバーのHPを更新
	//m_pHPBar->Update(elapsedTime);// HPバーの更新
	//if (m_currentHP <= m_maxHP / 2)m_pBossSheild->SetSheild(true);// HPが半分以下になったらシールドを出す
	//m_pBossSheild->Update(elapsedTime);// シールドの更新
	//m_isDead = m_pHPBar->GetIsDead();// 死んだかどうかを受け取る
}
/*
*	@brief	描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void LastBoss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
*	@brief	当たり判定描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void LastBoss::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// 未使用の警告を出さない
	UNREFERENCED_PARAMETER(proj);// 未使用の警告を出さない
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);// 描画開始
	DirectX::XMVECTOR color = Colors::Black;// 色
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// 当たった
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// 当たっていない
	DrawCollision::DrawBoundingSphere(m_bossBS, color);// 境界球の描画
	DrawCollision::DrawEnd();// 描画終了
#endif
}
