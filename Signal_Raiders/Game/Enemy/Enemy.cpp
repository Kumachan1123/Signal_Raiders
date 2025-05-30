/*
	@file	Enemy.cpp
	@brief	敵クラス
*/
#include <pch.h>
#include "Enemy.h"
// 自作ヘッダーファイル
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

/*
*	@brief	コンストラクタ
*	@param	pPlayer	プレイヤーのポインター
*	@param	pCommonResources	共通リソース
*	@param	hp	敵のHP
*	@return	なし
*/
Enemy::Enemy(Player* pPlayer, CommonResources* pCommonResources, int hp)
	: IEnemy(pPlayer, pCommonResources, hp)// 親クラスのコンストラクタを呼ぶ
	, m_pPlayer{ pPlayer }// プレイヤーのポインター
	, m_pCamera{ pPlayer->GetCamera() }// カメラのポインター
	, m_enemyBS{}// 敵の境界球
	, m_pCommonResources{ pCommonResources }// 共通リソース
	, m_currentHP{ hp }// 敵のHP
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
*	@param	なし
*/
Enemy::~Enemy()
{
	m_pBulletManager->RemoveBulletsByShooter(this);// 弾を削除する
}
/*
*	@brief	初期化する
*	@param	なし
*	@return	なし
*/
void Enemy::Initialize()
{
	using namespace DirectX::SimpleMath;
	DrawCollision::Initialize(m_pCommonResources);// 当たり判定描画クラスの初期化
	m_pEnemyModel = std::make_unique<EnemyModel>();	// 敵のモデルを読み込む
	m_pEnemyModel->Initialize(m_pCommonResources);// モデルの初期化
	m_pHPBar = std::make_unique<EnemyHPBar>();	// HPBar生成
	m_pHPBar->SetEnemyMaxHP(m_currentHP);// 最大HP設定
	m_pHPBar->Initialize(m_pCommonResources);// 初期化
	m_pEnemyAI = std::make_unique<EnemyAI>(this);	// AI生成
	m_pEnemyAI->Initialize();// AI初期化
	Vector3 position = 	// 乱数生成
		Vector3(GenerateRandomMultiplier(-EnemyParameters::ENEMY_SPAWN_RADIUS, EnemyParameters::ENEMY_SPAWN_RADIUS));
	m_position = Vector3{ position.x, 0.0f,position.z };// 敵の初期位置を設定
	m_pEnemyAI->SetPosition(m_position);	// 敵の座標を設定
	m_enemyBS.Center = m_position;	// 境界球の座標設定
	m_enemyBS.Radius = EnemyParameters::NORMAL_ENEMY_RADIUS;// 境界球の半径設定

}
/*
*	@brief	更新
*	@param	elapsedTime	経過時間
*	@return	なし
*/
void Enemy::Update(float elapsedTime)
{
	m_pEnemyModel->SetState(m_pEnemyAI->GetState());// モデルの更新
	m_pHPBar->SetCurrentHP(m_currentHP);// HPの更新
	m_pHPBar->Update(elapsedTime);// HPバーの更新
	m_pEnemyAI->Update(elapsedTime);// AIの更新
	m_pCommonResources->GetAudioManager()->Update();// オーディオマネージャーの更新
	if (m_pEnemyAI->GetNowState() == m_pEnemyAI->GetEnemyAttack())// 攻撃態勢なら
		ShootBullet();// 弾を発射
	m_enemyBS.Center = m_position;	// 敵の当たり判定の座標を更新
	m_isDead = m_pHPBar->GetIsDead();// 敵のHPが0になったら死亡
}
/*
*	@brief	描画
*	@param	view	ビュー行列
*	@param	proj	射影行列
*	@return	なし
*/
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキスト取得
	auto states = m_pCommonResources->GetCommonStates();// 共通ステート取得
	Matrix world = Matrix::CreateScale(m_pEnemyAI->GetScale())// スケール行列
		* Matrix::CreateFromQuaternion(m_pEnemyAI->GetRotation())// 回転行列
		* Matrix::CreateTranslation(m_position);	// 平行移動行列
	Vector3 hpBarPos = m_position + EnemyParameters::ENEMY_HPBAR_OFFSET;	// HPBarの座標を設定
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);	// HPBar描画
	m_pEnemyModel->Render(context, states, world, view, proj);	// 敵描画	
}
/*
*	@brief	敵の当たり判定を描画
*	@param	view	ビュー行列
*	@param	proj	射影行列
*	@return	なし
*/
void Enemy::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(view);// 未使用変数警告を無視
	UNREFERENCED_PARAMETER(proj);// 未使用変数警告を無視
#ifdef _DEBUG// デバッグビルド時のみ実行
	DrawCollision::DrawStart(view, proj);	// 描画開始
	DirectX::XMVECTOR color = Colors::Black;	// 色設定
	if (m_isHitToPlayer) color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;	// 当たった
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;	// 当たっていない
	DrawCollision::DrawBoundingSphere(m_enemyBS, color);	// 境界球描画
	DrawCollision::DrawEnd();	// 描画終了
#endif
}
/*
*	@brief	敵の弾を発射
*	@param	なし
*	@return	なし
*/
void Enemy::ShootBullet()
{
	m_attackCooldown = m_pEnemyAI->GetEnemyAttack()->GetCoolTime();// クールダウンタイムを取得
	if (m_attackCooldown <= EnemyParameters::ATTACK_INTERVAL)	// 攻撃のクールダウンタイムを管理
	{
		m_pCommonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		DirectX::SimpleMath::Vector3 direction =
			DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_pEnemyAI->GetRotation());// クォータニオンから方向ベクトルを計算
		m_pBulletManager->SetEnemyBulletType(BulletType::NORMAL);// 弾の種類を設定
		m_pBulletManager->SetEnemyBulletSize(EnemyParameters::ENEMY_BULLET_SIZE);// 弾のサイズを設定
		m_pBulletManager->SetShooter(this);// 弾の発射者を設定
		m_pBulletManager->CreateEnemyBullet(m_position, direction);// 弾を生成
		m_pEnemyAI->GetEnemyAttack()->SetCoolTime(EnemyParameters::ATTACK_COOLDOWN);	// クールダウンタイムをリセット
	}
}
