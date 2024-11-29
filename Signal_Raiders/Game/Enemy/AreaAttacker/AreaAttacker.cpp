/*
	@file	AreaAttacker.cpp
	@brief	範囲攻撃敵クラス
*/
#include "pch.h"
#include "AreaAttacker.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/AreaAttackerModel/AreaAttackerModel.h"
#include "Game/Enemy/Enemies/Enemies.h"
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

// コンストラクタ
AreaAttacker::AreaAttacker(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS{}
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_pAreaAttackerModel{}
	, m_enemyAI{}
	, m_HPBar{}
	, m_bullets{}
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_enemyBSToPlayerArea{}
	, m_enemyBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{ false }
	, m_isHit{ false }
	, m_isHitToOtherEnemy{ false }
	, m_isHitToPlayerBullet{ false }
	, m_isBullethit{ false }
	, m_audioManager{ AudioManager::GetInstance() }
{}

// デストラクタ
AreaAttacker::~AreaAttacker() {}

// 初期化する
void AreaAttacker::Initialize(CommonResources* resources, int hp)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを設定
	m_commonResources = resources;

	DrawCollision::Initialize(resources);
	// モデルを読み込む
	m_pAreaAttackerModel = std::make_unique<AreaAttackerModel>();
	m_pAreaAttackerModel->Initialize(m_commonResources);
	// 敵の体力を設定
	m_currentHP = hp;
	// HPBar生成
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// AI生成
	m_enemyAI = std::make_unique<EnemyAI>();
	m_enemyAI->Initialize();
	// 弾全体生成
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);
	// 乱数生成
	Vector3 position = Vector3(GenerateRandomMultiplier(-50.0f, 50.0f)); // 一様分布
	// 敵の初期位置を設定
	m_position = Vector3{ position.x, 0.0f,position.z };// 敵の初期位置を設定

	// 敵の座標を設定
	m_enemyAI->SetPosition(m_position);
	// 境界球の初期化
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;
	// オーディオマネージャー
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");

}

// 更新する
void AreaAttacker::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_pAreaAttackerModel->Update(elapsedTime, m_enemyAI->GetState());// モデルのアニメーション更新
	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AIの更新
	m_audioManager->Update();// オーディオマネージャーの更新
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// 攻撃態勢なら
	{
		m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
		// 攻撃のクールダウンタイムを管理
		if (m_attackCooldown <= 0.1f)
		{
			m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
			// クォータニオンから方向ベクトルを計算
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
			// 弾を発射
			m_enemyBullets->CreateBullet(m_enemyBS.Center, direction, playerPos, 0.15f, EnemyBullet::BulletType::VERTICAL);
			// クールダウンタイムをリセット
			m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
		}
	}
	m_enemyBullets->Update(elapsedTime, GetPosition());// 敵の弾の更新
	// 敵の当たり判定の座標を更新
	m_enemyBS.Center = m_position;
	//m_enemyBS.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);// HPBarの更新
	m_isDead = m_HPBar->GetIsDead();// 敵のHPが0になったら死亡
}

// 描画
void AreaAttacker::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
	m_HPBar->Render(view, proj, hpBarPos, m_rotate);
	// 敵描画	
	m_pAreaAttackerModel->Render(context, states, world, view, proj);
	// 敵の弾描画
	m_enemyBullets->Render(view, proj);
}

void AreaAttacker::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
#ifdef _DEBUG
	// 描画する
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 描画開始
	DrawCollision::DrawStart(view, proj);
	// 色設定
	DirectX::XMVECTOR color = Colors::Black;
	// 当たった
	if (m_isHit) color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;
	// 当たっていない
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;
	// 境界球描画
	DrawCollision::DrawBoundingSphere(m_enemyBS, color);
	// 描画終了
	DrawCollision::DrawEnd();
#endif
}
