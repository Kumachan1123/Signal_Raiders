/*
	@file	Boss.cpp
	@brief	ボスクラス
*/
#include "pch.h"
#include "Boss.h"
//// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"
//#include "Game/Enemy/EnemyManager/EnemyManager.h"


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
Boss::~Boss() {  }// 弾を削除
/*
*	@brief	初期化処理
*	@return	なし
*/
void Boss::CreateModel()
{
	m_pBossModel = std::make_unique<BossModel>();	// ボスモデル生成 これはタイプによって分岐予定
	m_pBossModel->Initialize(m_commonResources);// ボスモデル初期化 これはタイプによって分岐予定
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
