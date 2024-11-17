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

// コンストラクタ
Boss::Boss(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS()
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_bossModel{}
	, m_pBossAI{}
	, m_HPBar{}
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

{}
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
	// デバイスとコンテキストを取得
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	m_bossModel = std::make_unique<BossModel>();
	m_bossModel->Initialize(m_commonResources);
	// 敵の体力を設定
	m_currentHP = hp;
	// HPBar生成
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// AI生成
	m_pBossAI = std::make_unique<BossAI>();
	m_pBossAI->Initialize();
	//// 弾全体生成
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);


	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// 初期位置を設定
	m_position = Vector3(0.0f, 10.0f, 0.0f);
	// 敵の座標を設定
	m_pBossAI->SetPosition(m_position);


	// 境界球の初期化
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 2.5f;
	// オーディオマネージャー
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");

}
// 描画
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 基準となる座標やら回転やら
	Matrix world = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
	// 敵のサイズを設定
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale() * 2);
	// 敵の座標を設定
	enemyWorld *= world;

	// 敵描画	
	m_bossModel->Render(context, states, enemyWorld, view, proj);
	// HPBarの座標を設定
	Vector3 hpBarPos = Vector3(m_position.x, m_position.y - 5, m_position.z);
	m_HPBar->SetScale(Vector3(3.0f));
	// HPBar描画
	m_HPBar->Render(view, proj, hpBarPos, m_rotate);

	// 敵の弾描画
	m_enemyBullets->Render(view, proj);


#ifdef _DEBUG
	// 境界球を描画する
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
	m_primitiveBatch->Begin();
	if (!m_isHit)
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Black);
		else					DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::White);
	}
	else
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Blue);
		else					 DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Tomato);

	}

	m_primitiveBatch->End();
#endif

}
// 更新
void Boss::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bossModel->Update(elapsedTime, m_pBossAI->GetState());// モデルのアニメーション更新
	m_pBossAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AIの更新
	m_audioManager->Update();// オーディオマネージャーの更新

	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();
	ShootBullet();// 弾発射

	m_enemyBullets->Update(elapsedTime, GetPosition());// 敵の弾の更新
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 1.0f;
	// 弾の位置設定
	BulletPotsitioning();
	// HPBar更新
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// オブジェクト同士が衝突したら押し戻す
void Boss::CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
{
	using namespace DirectX::SimpleMath;
	// 押し戻す処理
	// Ａの中心とＢの中心との差分ベクトル（ＢからＡに向かうベクトル）…①
	Vector3 diffVector = A.Center - B.Center;
	// Ａの中心とＢの中心との距離（①の長さ）…②
	float distance = diffVector.Length();
	// Ａの半径とＢの半径の合計…③
	float sumRadius = A.Radius + B.Radius;
	// （ＡがＢに）めり込んだ距離（③－②）…④
	float penetrationDistance = sumRadius - distance;
	// ①を正規化する…⑤
	diffVector.Normalize();
	// 押し戻すベクトルを計算する（⑤と④で表現する）…⑥
	Vector3 pushBackVec = diffVector * penetrationDistance;
	// ⑥を使用して、Ａの座標とＡのコライダー座標を更新する（実際に押し戻す）
	m_position += pushBackVec;
	A.Center = m_position;
}

// 壁との衝突判定
void Boss::CheckHitWall(DirectX::BoundingSphere& A, DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	// 押し戻しベクトルを計算
	Vector3 pushBackVec = Vector3::Zero;
	// 球体の中心とボックスのクランプされた位置の差分を求める
	Vector3 closestPoint; // ボックスの最も近い点

	// 各軸でクランプして、最も近い位置を取得
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));

	// 球体の中心と最も近い点のベクトル差
	Vector3 diffVector = A.Center - closestPoint;

	// 距離を計算
	float distance = diffVector.Length();

	// 距離が球体の半径より小さい場合は押し戻し処理
	if (distance < A.Radius)
	{
		// 押し戻し量を計算 (正規化して押し戻しベクトルを作成)
		float penetrationDistance = A.Radius - distance;
		diffVector.Normalize();
		pushBackVec = diffVector * penetrationDistance;

		m_position += pushBackVec;
		A.Center = m_position;
		A.Center.y -= 2.0f;
	}
}

// 弾発射
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
		m_pBossAI->GetBossAttack()->SetCoolTime(1.5f);
	}
}

// 弾の位置設定
void Boss::BulletPotsitioning()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	// 弾の発射位置を設定
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// 中央の座標に回転を適用
	m_bulletPosCenter = Vector3::Transform(Vector3(0, 2.5f, 3), transform);
	// 左の座標に回転を適用
	m_bulletPosLeft = Vector3::Transform(Vector3(-2.5f, 0, 3), transform);
	// 右の座標に回転を適用
	m_bulletPosRight = Vector3::Transform(Vector3(2.5f, 0, 3), transform);
}


// 弾を生成
void Boss::CreateBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 角度をずらして左右の弾を発射
	constexpr float angleOffset = XMConvertToRadians(30.0f); // 30度の角度オフセット
	// Enemiesクラスで設定した弾のタイプによって処理を分岐
	switch (GetBulletType())
	{
	case BossBulletType::NORMAL:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// 中央の弾を発射
		break;
	case BossBulletType::TWIN:

		CreateLeftBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// 右の弾を発射
		break;
	case BossBulletType::THREE:
		CreateCenterBullet(EnemyBullet::BulletType::STRAIGHT);// 中央の弾を発射
		CreateLeftBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::STRAIGHT);// 右の弾を発射
		break;
	case BossBulletType::SPIRAL:
		CreateCenterBullet(EnemyBullet::BulletType::SPIRAL);// 中央の弾を発射
		CreateLeftBullet(angleOffset, EnemyBullet::BulletType::SPIRAL);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::SPIRAL);// 右の弾を発射
		CreateVerticalBullet();// 真下に落ちて着弾したら加速する弾を発射
		break;
	}

}

// 中央の弾を発射
void Boss::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_enemyBullets->CreateBullet(m_bulletPosCenter, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
	m_enemyBullets->SetRotateDirection(-1);// 螺旋弾の回転方向を設定（左回り
}

// 左の弾を発射
void Boss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 左方向
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	m_enemyBullets->CreateBullet(m_bulletPosLeft, m_bulletDirection, m_pPlayer->GetPlayerPos(),
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
	m_enemyBullets->CreateBullet(m_bulletPosRight, m_bulletDirection, m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, type);
}

// 真下に落ちる弾を発射
void Boss::CreateVerticalBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 真下に落ちる弾を発射
	m_enemyBullets->CreateBullet(m_bulletPosCenter, Vector3(0, 0, 0), m_pPlayer->GetPlayerPos(),
		BULLET_SIZE, EnemyBullet::BulletType::VERTICAL);
}