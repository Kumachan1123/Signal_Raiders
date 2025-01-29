/*
	@file	Enemy.cpp
	@brief	敵クラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/KumachiLib.h"
#include <Game/KumachiLib/DrawCollision/DrawCollision.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;


// コンストラクタ
Enemy::Enemy(Player* pPlayer, CommonResources* resources, int hp)
	: IEnemy(pPlayer, resources, hp)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS{}
	, m_commonResources{ resources }
	, m_currentHP{ hp }
	, m_attackCooldown{ 3.0f }
	, m_enemyModel{}
	, m_enemyAI{}
	, m_pHPBar{}
	, m_bullets{}
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_enemyBSToPlayerArea{}
	, m_enemyBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{ false }
	, m_isHitToPlayer{ false }
	, m_isHitToOtherEnemy{ false }
	, m_isEnemyHitByPlayerBullet{ false }
	, m_isPlayerHitByEnemyBullet{ false }
	, m_canAttack{ true }
	, m_audioManager{ AudioManager::GetInstance() }


{}
// デストラクタ
Enemy::~Enemy() {}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void Enemy::Initialize()
{

	// デバッグ用の初期化
	DrawCollision::Initialize(m_commonResources);

	// 敵のモデルを読み込む
	m_enemyModel = std::make_unique<EnemyModel>();
	m_enemyModel->Initialize(m_commonResources);
	// HPBar生成
	m_pHPBar = std::make_unique<EnemyHPBar>();
	m_pHPBar->SetEnemyHP(m_currentHP);
	m_pHPBar->Initialize(m_commonResources);
	// AI生成
	m_enemyAI = std::make_unique<EnemyAI>(this);
	m_enemyAI->Initialize();
	// 弾全体生成
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(m_commonResources);
	// 乱数生成
	Vector3 position = Vector3(GenerateRandomMultiplier(-50.0f, 50.0f)); // 一様分布
	// 敵の初期位置を設定
	m_position = Vector3{ position.x, 0.0f,position.z };// 敵の初期位置を設定
	// 敵の座標を設定
	m_enemyAI->SetPosition(m_position);
	// 境界球の初期化
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;

}
// 更新
void Enemy::Update(float elapsedTime)
{

	m_enemyModel->SetState(m_enemyAI->GetState());// モデルの更新
	m_pHPBar->SetCurrentHP(m_currentHP);
	m_pHPBar->Update(elapsedTime);
	m_enemyAI->Update(elapsedTime);// AIの更新
	m_audioManager->Update();// オーディオマネージャーの更新
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// 攻撃態勢なら
	{
		ShootBullet();// 弾を発射
	}
	m_enemyBullets->Update(elapsedTime);// 敵の弾の更新
	// 敵の当たり判定の座標を更新
	m_enemyBS.Center = m_position;
	m_isDead = m_pHPBar->GetIsDead();// 敵のHPが0になったら死亡
}
// 描画
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 基準となる座標やら回転やら
	Matrix world = Matrix::CreateScale(m_enemyAI->GetScale())
		* Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// HPBarの座標を設定
	Vector3 hpBarPos = Vector3(m_position.x, m_position.y - 1, m_position.z);
	// HPBar描画
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
	// 敵描画	
	m_enemyModel->Render(context, states, world, view, proj);

	// 敵の弾描画
	m_enemyBullets->Render(view, proj);

}
void Enemy::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG
	// 描画開始
	DrawCollision::DrawStart(view, proj);
	// 色設定
	DirectX::XMVECTOR color = Colors::Black;
	// 当たった
	if (m_isHitToPlayer) color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;
	// 当たっていない
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;
	// 境界球描画
	DrawCollision::DrawBoundingSphere(m_enemyBS, color);
	// 描画終了
	DrawCollision::DrawEnd();
#endif
}

// 敵の弾を発射
void Enemy::ShootBullet()
{
	m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
	// 攻撃のクールダウンタイムを管理
	if (m_attackCooldown <= ATTACK_INTERVAL)
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		// クォータニオンから方向ベクトルを計算
		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
		// 弾が飛ぶ方向を設定
		m_enemyBullets->SetDirection(direction);
		// 発射位置を設定
		m_enemyBullets->SetEnemyPosition(m_position);
		// 弾を発射
		m_enemyBullets->CreateBullet(0.15f, EnemyBullet::BulletType::STRAIGHT);
		// クールダウンタイムをリセット
		m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
	}
}
