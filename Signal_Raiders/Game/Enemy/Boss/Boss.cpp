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
#include "Game/Enemy/EnemyManager/EnemyManager.h"

#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
Boss::Boss(Player* pPlayer, CommonResources* resources, int hp)
	: IEnemy(pPlayer, resources, hp)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_bossBS{}
	, m_commonResources{ resources }
	, m_currentHP{ hp }
	, m_maxHP{}
	, m_attackCooldown{ EnemyParameters::ATTACK_COOLDOWN }
	, m_bulletCooldown{ EnemyParameters::ATTACK_INTERVAL }
	, m_SEVolume{ 0.0f }
	, m_SEVolumeCorrection{ 0.0f }
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
	, m_isDead{ false }
	, m_isHitToPlayer{ false }
	, m_isHitToOtherEnemy{ false }
	, m_isEnemyHitByPlayerBullet{ false }
	, m_isPlayerHitByEnemyBullet{ false }
	, m_canAttack{ true }
	, m_bossBulletType{ BossBulletType::NORMAL }
	, m_bulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_audioManager{ }

{
}
// デストラクタ
Boss::~Boss() {}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void Boss::Initialize()
{
	// 当たり判定描画用クラスの初期化
	DrawCollision::Initialize(m_commonResources);
	// ボスモデル生成
	m_pBossModel = std::make_unique<BossModel>();
	m_pBossModel->Initialize(m_commonResources);
	m_maxHP = m_currentHP;
	// HPBar生成
	m_pHPBar = std::make_unique<EnemyHPBar>();
	m_pHPBar->SetEnemyHP(m_currentHP);
	m_pHPBar->Initialize(m_commonResources);
	// AI生成
	m_pBossAI = std::make_unique<BossAI>(this);
	m_pBossAI->Initialize();
	// 弾全体生成
	m_pEnemyBullets = std::make_unique<EnemyBullets>(this);
	m_pEnemyBullets->Initialize(m_commonResources);
	// シールド生成
	m_pBossSheild = std::make_unique<BossSheild>(m_maxHP, this);
	m_pBossSheild->Initialize(m_commonResources);
	// 初期位置を設定
	m_position = EnemyParameters::INITIAL_BOSS_POSITION;
	// 敵の座標を設定
	m_pBossAI->SetPosition(m_position);
	// 境界球の初期化
	m_bossBS.Center = m_position;
	m_bossBS.Radius = EnemyParameters::NORMAL_BOSS_RADIUS;
	// 音量の設定
	m_SEVolume = m_pPlayer->GetVolume();
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();
}
// 描画
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 敵のワールド行列を設定
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale())
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
	Vector3 hpBarPos = m_position - EnemyParameters::BOSS_HPBAR_OFFSET;
	m_pHPBar->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));
	// HPBar描画
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);
}
void Boss::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG
	// 描画開始
	DrawCollision::DrawStart(view, proj);
	// 色設定
	DirectX::XMVECTOR color = Colors::Black;
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// 当たった
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// 当たっていない
	// 境界球描画
	DrawCollision::DrawBoundingSphere(m_bossBS, color);
	// 描画終了
	DrawCollision::DrawEnd();
#endif
}
// 更新
void Boss::Update(float elapsedTime)
{
	// カメラの情報を取得
	m_cameraEye = m_pCamera->GetEyePosition();
	m_cameraTarget = m_pCamera->GetTargetPosition();
	m_cameraUp = m_pCamera->GetUpVector();
	// ボスのモデルの状態を更新
	m_pBossModel->SetState(m_pBossAI->GetState());// モデルのアニメーション更新
	m_pBossAI->Update(elapsedTime);// AIの更新
	m_position = m_pBossAI->GetPosition();// 敵の座標を更新
	m_audioManager->Update();// オーディオマネージャーの更新
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// 攻撃のクールダウンタイムを取得
	this->ShootBullet();// 弾発射
	m_pEnemyBullets->Update(elapsedTime);// 敵の弾の更新
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// 境界球の中心座標を更新
	// 弾の位置設定
	this->BulletPositioning();
	// HPBar更新
	m_pHPBar->SetCurrentHP(m_currentHP);
	m_pHPBar->Update(elapsedTime);
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
	m_audioManager->PlaySound("Barrier", this->GetSheildSEVolume());// サウンド再生 
}
//---------------------------------------------------------
// 弾発射
//---------------------------------------------------------
void Boss::ShootBullet()
{
	// 攻撃のクールダウンタイムを管理
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		// クォータニオンから方向ベクトルを計算
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());
		// 弾を発射
		this->CreateBullet();
		// クールダウンタイムをリセット
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);
	}

}

// 弾の位置設定
void Boss::BulletPositioning()
{

	// 弾の発射位置を設定
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// 中央の座標に回転を適用
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);
	// 左の座標に回転を適用
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);
	// 右の座標に回転を適用
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);
}


// 弾を生成
void Boss::CreateBullet()
{
	// 角度をずらして左右の弾を発射
	float angleOffset = XMConvertToRadians(EnemyParameters::BOSS_BULLET_ANGLE); // 30度の角度オフセット
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
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosCenter);
	m_pEnemyBullets->SetDirection(m_bulletDirection);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, type);
}

// 左の弾を発射
void Boss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	// 左方向
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	// 発射位置を設定
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosLeft);
	m_pEnemyBullets->SetDirection(leftDirection);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, type);
}

// 右の弾を発射
void Boss::CreateRightBullet(float angleOffset, EnemyBullet::BulletType type)
{
	// 右方向
	Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);
	Vector3 rightDirection = Vector3::Transform(m_bulletDirection, rightRotation);
	// 発射位置を設定
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosRight);
	m_pEnemyBullets->SetDirection(rightDirection);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, type);
}

// 真下に落ちる弾を発射
void Boss::CreateVerticalBullet()
{
	// 真下に落ちる弾を発射
	m_pEnemyBullets->SetEnemyPosition(m_bulletPosCenter);
	m_pEnemyBullets->CreateBullet(EnemyParameters::BOSS_BULLET_SIZE, EnemyBullet::BulletType::VERTICAL);
}

// 敵のHPに関する処理
void Boss::SetEnemyHP(int hp)
{
	// シールドがある場合はシールドのHPを減らす
	if (m_pBossSheild->GetSheildHP() > 0 && m_pBossSheild->GetSheild() == true)
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);
		if (m_pBossSheild->GetSheildHP() <= 0)
		{
			m_audioManager->PlaySound("BarrierBreak", m_pPlayer->GetVolume());// サウンド再生 
		}
	}
	else
	{
		// シールドがない場合は敵のHPを減らす
		m_currentHP -= hp;
	}

}
