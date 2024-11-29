/*
	@file	Boss.cpp
	@brief	ボスクラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Boss/Boss.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/Boss/BossModel/BossModel.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"

// コンストラクタ
Boss::Boss(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_bossBS{}
	, m_commonResources{}
	, m_currentHP{}
	, m_maxHP{}
	, m_attackCooldown{ 3.0f }
	, m_bulletCooldown{ 1.0f }
	, m_pBossModel{}
	, m_pBossAI{}
	, m_pHPBar{}
	, m_time{ 0.0f }
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_enemyBSToPlayerArea{}
	, m_enemyBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{}
	, m_isHit{}
	, m_isHitToOtherEnemy{}
	, m_isHitToPlayerBullet{}
	, m_isBullethit{}
	, m_bossBulletType{ BossBulletType::NORMAL }
	, m_bulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_audioManager{ AudioManager::GetInstance() }
{
}
// デストラクタ
Boss::~Boss() {}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void Boss::Initialize(CommonResources* resources, int hp)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを設定
	m_commonResources = resources;
	// 当たり判定描画用クラスの初期化
	DrawCollision::Initialize(m_commonResources);
	// ボスモデル生成
	m_pBossModel = std::make_unique<BossModel>();
	m_pBossModel->Initialize(m_commonResources);
	// 敵の体力を設定
	m_currentHP = hp;
	m_maxHP = hp;
	// HPBar生成
	m_pHPBar = std::make_unique<EnemyHPBar>();
	m_pHPBar->SetEnemyHP(m_currentHP);
	m_pHPBar->Initialize(m_commonResources);
	// AI生成
	m_pBossAI = std::make_unique<BossAI>();
	m_pBossAI->Initialize();
	// 弾全体生成
	m_pEnemyBullets = std::make_unique<EnemyBullets>(this);
	m_pEnemyBullets->Initialize(m_commonResources);
	// シールド生成
	m_pBossSheild = std::make_unique<BossSheild>(m_maxHP, this);
	m_pBossSheild->Initialize(m_commonResources);
	// 初期位置を設定
	m_position = INITIAL_POSITION;
	// 敵の座標を設定
	m_pBossAI->SetPosition(m_position);
	// 境界球の初期化
	m_bossBS.Center = m_position;
	m_bossBS.Radius = SPHERE_RADIUS;
	// オーディオマネージャー
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");// 弾発射音
	m_audioManager->LoadSound("Resources/Sounds/Barrier.mp3", "Barrier");// シールド音
	m_audioManager->LoadSound("Resources/Sounds/BarrierBreak.mp3", "BarrierBreak");// シールド破壊音
}
// 描画
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 敵のワールド行列を設定
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale() * 2)
		* Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// シールドの座標を設定
	m_pBossSheild->SetPosition(m_bossBS.Center);
	m_pBossSheild->SetRotation(m_pBossAI->GetRotation());
	// 敵描画	
	m_pBossModel->Render(context, states, enemyWorld, view, proj);
	// シールド描画
	m_pBossSheild->Render(context, states, enemyWorld, view, proj);
	// 敵の弾描画
	m_pEnemyBullets->Render(view, proj);
	// HPBarの座標を設定
	Vector3 hpBarPos = m_position - HPBAR_OFFSET;
	m_pHPBar->SetScale(Vector3(HPBAR_SCALE));
	// HPBar描画
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
}
void Boss::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
#ifdef _DEBUG
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 描画開始
	DrawCollision::DrawStart(view, proj);
	// 色設定
	DirectX::XMVECTOR color = Colors::Black;
	if (m_isHit)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// 当たった
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// 当たっていない
	// 境界球描画
	DrawCollision::DrawBoundingSphere(m_bossBS, color);
	// 描画終了
	DrawCollision::DrawEnd();
#endif
}
// 更新
void Boss::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pBossModel->Update(elapsedTime, m_pBossAI->GetState());// モデルのアニメーション更新
	m_pBossAI->Update(elapsedTime, playerPos, m_isHit, m_isHitToPlayerBullet);// AIの更新
	m_position = m_pBossAI->GetPosition();// 敵の座標を更新
	m_audioManager->Update();// オーディオマネージャーの更新
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();
	ShootBullet();// 弾発射
	m_pEnemyBullets->Update(elapsedTime, m_position);// 敵の弾の更新
	m_bossBS.Center = m_position + SPHERE_OFFSET;// 境界球の中心座標を更新
	// 弾の位置設定
	BulletPositioning();
	// HPBar更新
	m_pHPBar->Update(elapsedTime, m_currentHP);
	// 最大HPの半分になったらシールドを展開
	if (m_currentHP <= m_maxHP / 2)m_pBossSheild->SetSheild(true);
	// シールド更新
	m_pBossSheild->Update(elapsedTime);
	// 死んだかどうかを受け取る
	m_isDead = m_pHPBar->GetIsDead();
}
//---------------------------------------------------------
// バリアSE再生
//---------------------------------------------------------
void Boss::PlayBarrierSE()
{
	m_audioManager->PlaySound("Barrier", m_pPlayer->GetVolume() / 2);// サウンド再生 
}
//---------------------------------------------------------
// 弾発射
//---------------------------------------------------------
void Boss::ShootBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 攻撃のクールダウンタイムを管理
	if (m_attackCooldown <= 0.1f)
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		// クォータニオンから方向ベクトルを計算
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());
		// 弾を発射
		CreateBullet();
		// クールダウンタイムをリセット
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);
	}

}

// 弾の位置設定
void Boss::BulletPositioning()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	// 弾の発射位置を設定
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// 中央の座標に回転を適用
	m_bulletPosCenter = Vector3::Transform(TOP_OFFSET, transform);
	// 左の座標に回転を適用
	m_bulletPosLeft = Vector3::Transform(LEFT_OFFSET, transform);
	// 右の座標に回転を適用
	m_bulletPosRight = Vector3::Transform(RIGHT_OFFSET, transform);
}


// 弾を生成
void Boss::CreateBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 角度をずらして左右の弾を発射
	float angleOffset = XMConvertToRadians(ANGLE_OFFSET); // 30度の角度オフセット
	// Enemiesクラスで設定した弾のタイプによって処理を分岐
	switch (GetBulletType())
	{
	case BossBulletType::NORMAL:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// 中央の弾を発射
		break;
	case BossBulletType::TWIN:

		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::STRAIGHT);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// 右の弾を発射
		break;
	case BossBulletType::THREE:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// 中央の弾を発射
		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::STRAIGHT);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// 右の弾を発射
		break;
	case BossBulletType::SPIRAL:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// 中央の弾を発射
		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::SPIRAL);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::SPIRAL);// 右の弾を発射
		CreateVerticalBullet();// 真下に落ちて着弾したら加速する弾を発射
		break;
	}

}

// 中央の弾を発射
void Boss::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_pEnemyBullets->CreateBullet(m_bulletPosCenter, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// 左の弾を発射
void Boss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 左方向
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	m_pEnemyBullets->CreateBullet(m_bulletPosLeft, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// 右の弾を発射
void Boss::CreateRightBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 右方向
	Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);
	Vector3 rightDirection = Vector3::Transform(m_bulletDirection, rightRotation);
	m_pEnemyBullets->CreateBullet(m_bulletPosRight, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// 真下に落ちる弾を発射
void Boss::CreateVerticalBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 真下に落ちる弾を発射
	m_pEnemyBullets->CreateBullet(m_bulletPosCenter, Vector3::Zero, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, EnemyBullet::BulletType::VERTICAL);
}
void Boss::SetEnemyHP(int hp)
{
	// シールドがある場合はシールドのHPを減らす
	if (m_pBossSheild->GetSheildHP() > 0 && m_pBossSheild->GetSheild() == true)
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);
		if (m_pBossSheild->GetSheildHP() <= 0)
		{
			m_audioManager->PlaySound("BarrierBreak", m_pPlayer->GetVolume() * 3);// サウンド再生 
		}
	}
	else //if (m_pBossSheild->GetSheildHP() <= 0)
	{
		// シールドがない場合は敵のHPを減らす
		m_currentHP -= hp;
	}

}
