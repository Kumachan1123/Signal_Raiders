/*
*	@file	Boss.cpp
*	@brief	ボスクラス
*/
#include <pch.h>
#include "Boss.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"
/*
*	@brief	コンストラクタ
*	@details ボスクラスのコンストラクタ
*	@param pBoss		ボスベースのポインタ
*	@param resources	共通リソース
*	@return	なし
*/
Boss::Boss(BossBase* pBoss, CommonResources* commonResources)
	: m_pCommonResources(commonResources)// 共通リソース
	, m_pBossBase(pBoss)// ボスのポインタ
	, m_bossBS{}// 境界球の初期化
	, m_pBossModel{}// ボスモデル
	, m_pHPBar{}// HPバー
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
*	@details ボスクラスのデストラクタ
*	@param	なし
*	@return	なし
*/
Boss::~Boss()
{
	// ボスモデルの解放
	m_pBossModel.reset();
	// HPバーの解放
	m_pHPBar.reset();
	// 共通リソースのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief	初期化処理
*	@details ボスクラスの初期化
*	@param	なし
*	@return	なし
*/
void Boss::Initialize()
{
	// ボスモデル生成
	m_pBossModel = std::make_unique<BossModel>();
	// ボスモデル初期化
	m_pBossModel->Initialize(m_pCommonResources);
	// ラスボスの初期位置を設定
	m_position = EnemyParameters::INITIAL_BOSS_POSITION;
	// ベースクラスに初期位置を設定
	m_pBossBase->SetPosition(m_position);
	// 通常時ボスの当たり判定を設定
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_BOSS_RADIUS);
	// シールド展開時のボスの当たり判定を設定
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::BOSS_SHIELD_RADIUS);
	// 弾のサイズを設定
	m_pBossBase->SetBulletSize(EnemyParameters::BOSS_BULLET_SIZE);
	// ボスがプレイヤーに与えるダメージを設定
	m_pBossBase->SetToPlayerDamage(EnemyParameters::BOSS_DAMAGE);
	// バリア破壊パーティクルのサイズ設定
	m_pBossBase->SetBarrierBreakSize(EnemyParameters::BOSS_BARRIERBREAK_SIZE);
	// 死亡エフェクトのサイズ
	m_pBossBase->SetDeadEffectSize(EnemyParameters::BOSS_DEADEFFECT_SCALE);
}
/*
*	@brief	更新処理
*	@details ボスクラスの更新処理
*	@param elapsedTime	経過時間
*	@return	なし
*/
void Boss::ChangeState()
{
	// モデルの状態を更新
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());
}
/*
*	@brief	描画処理
*	@details ボスクラスの描画処理
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return	なし
*/
void Boss::Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// ワールド行列を設定（スケール → 回転 → 位置）
	Matrix enemyWorld = Matrix::CreateScale(m_pBossBase->GetScale())
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	// シールドのワールド行列を設定（スケール → 回転 → 位置）
	Matrix sheildWorld = Matrix::CreateScale(m_pBossBase->GetScale() * EnemyParameters::BOSS_SHIELD_SIZE)
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	// シールドの座標を設定
	m_pBossBase->GetBossSheild()->SetPosition(m_bossBS.Center);
	// シールドの回転を設定
	m_pBossBase->GetBossSheild()->SetRotation(m_pBossBase->GetQuaternion());
	// モデル描画
	m_pBossModel->Render(context, states, enemyWorld, view, proj);
	// シールド描画
	m_pBossBase->GetBossSheild()->Render(context, states, sheildWorld, view, proj);
	// HPバーの位置を設定
	Vector3 hpBarPos = m_pBossBase->GetPosition() - EnemyParameters::BOSS_HPBAR_OFFSET;
	// HPバーのスケールを設定
	m_pBossBase->GetHPBar()->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));
	// HPバー描画
	m_pBossBase->GetHPBar()->Render(view, proj, hpBarPos, m_rotate);
}
/*
*	@brief	弾の位置設定
*	@details ボスクラスの弾の発射位置を設定
*	@param	なし
*	@return	なし
*/
void Boss::BulletPositioning()
{
	using namespace DirectX::SimpleMath;
	// 弾の発射位置を設定
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation()) * Matrix::CreateTranslation(m_pBossBase->GetPosition());
	// 中央の座標に回転を適用
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);
	// 左の座標に回転を適用
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);
	// 右の座標に回転を適用
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);
}
/*
*	@brief	弾の生成
*	@details ボスの弾を生成
*	@param	なし
*	@return	なし
*/
void Boss::CreateBullet()
{
	// 弾のサイズを設定
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE);
	// 弾を発射したオブジェクトを設定
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);
	// Enemiesクラスで設定した弾のタイプによって処理を分岐
	switch (m_pBossBase->GetBulletType())
	{
	case BossBase::BossBulletType::STAGE_1:// 通常弾
		// 中央の弾を発射
		CreateCenterBullet(BulletType::NORMAL);
		break;
	case BossBase::BossBulletType::STAGE_2:// 二発

		// 左の弾を発射
		CreateLeftBullet(BulletType::NORMAL);
		// 右の弾を発射
		CreateRightBullet(BulletType::NORMAL);
		break;
	case BossBase::BossBulletType::STAGE_3:// 三発
		// 中央の弾を発射
		CreateCenterBullet(BulletType::NORMAL);
		// 左の弾を発射
		CreateLeftBullet(BulletType::NORMAL);
		// 右の弾を発射
		CreateRightBullet(BulletType::NORMAL);
		break;
	}
}
/*
*	@brief	中央から弾を発射
*	@details ボスの中央から弾を発射する
*	@param type 弾の種類
*	@return	なし
*/
void Boss::CreateCenterBullet(BulletType type)
{
	// 弾の種類を設定
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);
	// 弾を生成
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);
}
/*
*	@brief	左の弾を発射
*	@details ボスの左から弾を発射する
*	@param type 弾の種類
*	@return	なし
*/
void Boss::CreateLeftBullet(BulletType type)
{
	// 弾の種類を設定
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);
	// 弾を生成
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeft, m_bulletDirection);// 弾を生成
}
/*
*	@brief 右の弾を発射
*	@details ボスの右から弾を発射する
*	@param type 弾の種類
*	@return なし
*/
void Boss::CreateRightBullet(BulletType type)
{
	// 弾の種類を設定
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);
	// 弾を生成
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRight, m_bulletDirection);// 弾を生成
}