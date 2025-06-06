/*
	@file	CowardlyEnemy.cpp
	@brief	すばしっこくて卑怯な敵クラス
*/
#include <pch.h>
#include "CowardlyEnemy.h"
// 自作ヘッダーファイル
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

/*
*	@brief	コンストラクタ
*	@details　初期化を行う
*	@param	pPlayer	プレイヤーのポインター
*	@param	pCommonResources	共通リソース
*	@param	hp	敵のHP
*	@return	なし
*/
CowardlyEnemy::CowardlyEnemy(Player* pPlayer, CommonResources* pCommonResources, int hp)
	: m_pPlayer{ pPlayer }// プレイヤーのポインター
	, m_pCamera{ pPlayer->GetCamera() }// カメラのポインター
	, m_enemyBS{}// 敵の境界球
	, m_pCommonResources{ pCommonResources }// 共通リソース
	, m_currentHP{ hp }// 敵のHP
	, m_pBulletManager{ nullptr }// 弾管理クラスのポインター
	, m_attackCooldown{ EnemyParameters::ATTACK_COOLDOWN }// 攻撃のクールダウンタイム
	, m_pEnemyModel{}// 敵のモデル
	, m_pEnemyAI{}// 敵のAI
	, m_pHPBar{}// 敵のHPバー
	, m_position{}// 敵の座標
	, m_velocity{}// 敵の速度
	, m_rotate{}// 敵の回転
	, m_enemyBSToPlayerArea{}// 敵とPlayerとの一定範囲の当たり判定に使う
	, m_enemyBulletBS{}// 敵の弾の境界球
	, m_playerBS{}// プレイヤーの境界球
	, m_matrix{}// マトリクス
	, m_isDead{ false }// 敵のHPが0になったらTrue
	, m_isHitToPlayer{ false }// プレイヤーとの判定
	, m_isHitToOtherEnemy{ false }// その他の敵との判定
	, m_isEnemyHitByPlayerBullet{ false }// 敵がプレイヤーの弾に当たったか
	, m_isPlayerHitByEnemyBullet{ false }// 敵の弾がプレイヤーに当たったか
	, m_canAttack{ true }// 攻撃可能か
	, m_isAttack{ false }// 攻撃中か
{}
/*
*	@brief	デストラクタ
*	@details	死亡した敵の弾を削除する
*	@param	なし
*/
CowardlyEnemy::~CowardlyEnemy()
{
	// 弾を削除する
	m_pBulletManager->RemoveBulletsByShooter(this);
}
/*
*	@brief	初期化する
*	@details	敵の初期化を行う
*	@param	なし
*	@return	なし
*/
void CowardlyEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;
	// 当たり判定描画クラスの初期化
	DrawCollision::Initialize(m_pCommonResources);
	// 敵のモデルを読み込む
	m_pEnemyModel = std::make_unique<CowardlyEnemyModel>();
	// モデルの初期化
	m_pEnemyModel->Initialize(m_pCommonResources);
	// HPバー生成
	m_pHPBar = std::make_unique<EnemyHPBar>();
	// 最大HPを設定
	m_pHPBar->SetEnemyMaxHP(m_currentHP);
	// HPバー初期化
	m_pHPBar->Initialize(m_pCommonResources);
	// AI生成
	m_pEnemyAI = std::make_unique<EnemyAI>(this);
	// 状態を追跡状態にする
	m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyChase());
	// AI初期化
	m_pEnemyAI->Initialize();
	// 乱数生成
	Vector3 position = Vector3(GenerateRandomMultiplier(-EnemyParameters::ENEMY_SPAWN_RADIUS, EnemyParameters::ENEMY_SPAWN_RADIUS));
	// 敵の初期位置を設定
	m_position = Vector3{ position.x, 0.0f,position.z };
	// 敵の座標を設定
	m_pEnemyAI->SetPosition(m_position);
	// 境界球の座標設定
	m_enemyBS.Center = m_position;
	// 境界球の半径設定
	m_enemyBS.Radius = EnemyParameters::NORMAL_ENEMY_RADIUS;
}
/*
*	@brief	更新
*	@param	elapsedTime	経過時間
*	@return	なし
*/
void CowardlyEnemy::Update(float elapsedTime)
{
	// モデルの更新
	m_pEnemyModel->SetState(m_pEnemyAI->GetState());
	// HPの更新
	m_pHPBar->SetCurrentHP(m_currentHP);
	// HPバーの更新
	m_pHPBar->Update(elapsedTime);
	// AIの更新
	m_pEnemyAI->Update(elapsedTime);
	// オーディオマネージャーの更新
	m_pCommonResources->GetAudioManager()->Update();
	// 攻撃態勢なら弾を発射
	/*if (m_pEnemyAI->GetNowState() == m_pEnemyAI->GetEnemyAttack())*/ShootBullet(elapsedTime);
	// 敵の当たり判定の座標を更新
	m_enemyBS.Center = m_position;
	// 敵のHPが0になったら死亡
	m_isDead = m_pHPBar->GetIsDead();
}
/*
*	@brief	描画
*	@details	敵の描画を行う
*	@param	view	ビュー行列
*	@param	proj	射影行列
*	@return	なし
*/
void CowardlyEnemy::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// デバイスコンテキスト取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステート取得
	auto states = m_pCommonResources->GetCommonStates();
	// ワールド行列を設定（スケール → 回転 → 位置）
	Matrix world = Matrix::CreateScale(m_pEnemyAI->GetScale())
		* Matrix::CreateFromQuaternion(m_pEnemyAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// HPBarの座標を設定
	Vector3 hpBarPos = m_position + EnemyParameters::ENEMY_HPBAR_OFFSET;
	// HPBar描画
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
	// 敵描画	
	m_pEnemyModel->Render(context, states, world, view, proj);
}
/*
*	@brief	敵の当たり判定を描画
*	@details	敵の当たり判定を描画する
*	@param	view	ビュー行列
*	@param	proj	射影行列
*	@return	なし
*/
void CowardlyEnemy::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	// 未使用変数警告を無視
	UNREFERENCED_PARAMETER(view);
	// 未使用変数警告を無視
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG// デバッグビルド時のみ実行
	// 描画開始
	DrawCollision::DrawStart(view, proj);
	// 色設定
	DirectX::XMVECTOR color = Colors::Black;
	// プレイヤーに当たっている場合：他の敵にも当たっていればTomato、当たっていなければBlue
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;
	// プレイヤーに当たっていない場合：他の敵に当たっていればWhite、当たっていなければBlack
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;
	// 境界球描画
	DrawCollision::DrawBoundingSphere(m_enemyBS, color);
	// 描画終了
	DrawCollision::DrawEnd();
#endif
}
/*
*	@brief	敵の弾を発射
*	@details	敵の弾を発射する
*	@param	elapsedTime 経過時間
*	@return	なし
*/
void CowardlyEnemy::ShootBullet(float elapsedTime)
{
	// クールダウンタイムを取得
	m_attackCooldown = m_pEnemyAI->GetAttackCooldown();
	// 攻撃のクールダウンタイムを管理
	if (m_attackCooldown <= elapsedTime)
	{
		// 弾の発射音再生 
		m_pCommonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
		// クォータニオンから方向ベクトルを計算
		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_pEnemyAI->GetRotation());
		// 弾の種類を設定
		m_pBulletManager->SetEnemyBulletType(BulletType::NORMAL);
		// 弾のサイズを設定
		m_pBulletManager->SetEnemyBulletSize(EnemyParameters::ENEMY_BULLET_SIZE);
		// 弾の発射者を設定
		m_pBulletManager->SetShooter(this);
		// 弾を生成
		m_pBulletManager->CreateEnemyBullet(m_position, direction);
		// クールダウンタイムをリセット
		m_pEnemyAI->SetAttackCooldown(EnemyParameters::ATTACK_COOLDOWN);
	}
}
