/*
	@file	Enemy.cpp
	@brief	敵クラス
	作成者：くまち
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
#include "Game/Enemy/Enemies/Enemies.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>

// コンストラクタ
Enemy::Enemy(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS{}
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_enemyModel{}
	, m_enemyAI{}
	, m_HPBar{}
	, m_bullets{}
	, m_depthStencilState_Shadow{}
	, m_pixelShader{}
	, m_depthStencilState{}
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
	, m_audioManager{ AudioManager::GetInstance() }


{}
// デストラクタ
Enemy::~Enemy() {}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void Enemy::Initialize(CommonResources* resources, int hp)
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
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/PS_EnemyShadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));
	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Enemy");
	// 影用のモデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
	m_enemyModel = std::make_unique<EnemyModel>();
	m_enemyModel->Initialize(m_commonResources);
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
	std::random_device rd;  // シード生成器
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタの乱数生成器
	std::uniform_real_distribution<float> dist(-50.0f, 50.0f); // 一様分布
	m_position.x = dist(gen);
	m_position.y = 6.0f;
	m_position.z = dist(gen);
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// 敵の座標を設定
	m_enemyAI->SetPosition(m_position);
	// 境界球の初期化
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;
	// オーディオマネージャー
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");

}
// 描画
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 基準となる座標やら回転やら
	Matrix world = Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())
		* Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
	// 敵のサイズを設定
	Matrix enemyWorld = Matrix::CreateScale(m_enemyAI->GetScale());
	// 敵の座標を設定
	enemyWorld *= world;
	// HPBar描画
	m_HPBar->Render(view, proj, m_position, m_rotate);
	// 敵描画	
	m_enemyModel->Render(context, states, enemyWorld, view, proj);
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, -0.01f));
	enemyWorld *= shadowMatrix;
	// 影描画
	m_model->Draw(context, *states, enemyWorld * Matrix::Identity, view, proj, true, [&]()
				  {
					  context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
					  context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 0);
					  context->RSSetState(states->CullClockwise());
					  context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
				  });
	// 描画する
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
	// 敵の弾描画
	m_enemyBullets->Render(view, proj);
#ifdef _DEBUG
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
void Enemy::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_enemyModel->Update(elapsedTime, m_enemyAI->GetState());// モデルのアニメーション更新
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
			m_enemyBullets->CreateBullet(GetPosition(), direction, playerPos);
			// クールダウンタイムをリセット
			m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
		}
	}
	m_enemyBullets->Update(elapsedTime, GetPosition());// 敵の弾の更新
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// オブジェクト同士が衝突したら押し戻す(境界球同士の場合）
void Enemy::CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
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
	A.Center.y -= 2.0f;
}
// オブジェクト同士が衝突したら押し戻す(オブジェクトと壁の場合）
void Enemy::CheckHitWall(DirectX::BoundingSphere& O, DirectX::BoundingBox& W)
{
	using namespace DirectX::SimpleMath;
	Vector3 closestPoint;
	closestPoint.x = std::max(W.Center.x - W.Extents.x, std::min(O.Center.x, W.Center.x + W.Extents.x));
	closestPoint.y = std::max(W.Center.y - W.Extents.y, std::min(O.Center.y, W.Center.y + W.Extents.y));
	closestPoint.z = std::max(W.Center.z - W.Extents.z, std::min(O.Center.z, W.Center.z + W.Extents.z));
	// 球の中心と最近接点のベクトルを計算
	Vector3 pushDirection = O.Center - closestPoint;
	float distance = pushDirection.Length();
	// 衝突が発生している場合に押し戻し処理を行う
	if (distance < O.Radius)
	{
		pushDirection.Normalize();
		float pushDistance = O.Radius - distance;

		// 押し戻しを適用
		O.Center = O.Center + (pushDirection * pushDistance);
	}
}




