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
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>

// コンストラクタ
Enemy::Enemy()
	: m_enemyBoundingSphere()
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{}
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
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	m_attackCooldown = 3.0f;
	/*
	デバッグドローの表示用オブジェクトを生成する
*/
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
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
	// HPBar生成
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->Initialize(resources);
	// AI生成
	m_enemyAI = std::make_unique<EnemyAI>();
	m_enemyAI->Initialize();
	std::random_device rd;  // シード生成器
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタの乱数生成器
	std::uniform_real_distribution<float> dist(-25.0f, 25.0f); // 一様分布
	m_position.x = dist(gen);
	m_position.y = 6.0f;
	m_position.z = dist(gen);
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);

	m_enemyAI->SetPosition(m_position);
	m_currentHP = hp;
	m_HPBar->SetEnemyHP(m_currentHP);
	// 境界球の初期化
	m_enemyBoundingSphere.Center = m_position;
	m_enemyBoundingSphere.Radius = 1.5f;
}
// 描画
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 敵のサイズを設定
	Matrix enemyWorld = Matrix::CreateScale(m_enemyAI->GetScale());
	// 敵の座標を設定
	enemyWorld *= Matrix::CreateFromQuaternion(m_enemyAI->GetRotation());
	enemyWorld *= Matrix::CreateTranslation(m_position);
	enemyWorld *= Matrix::CreateTranslation(Vector3{ 0,-2,0 });


	// モデルのエフェクト情報を更新する
	m_model->UpdateEffects([](DirectX::IEffect* effect)
						   {
							   // ベーシックエフェクトを設定する
							   BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);

							   if (basicEffect)
							   {
								   // 個別のライトをすべて無効化する
								   basicEffect->SetLightEnabled(0, false);
								   basicEffect->SetLightEnabled(1, false);
								   basicEffect->SetLightEnabled(2, false);

								   // モデルを自発光させる
								   basicEffect->SetAmbientLightColor(Colors::White);
								   basicEffect->SetFogStart(1);
							   }


						   }
	);
	// HPBar描画
	m_HPBar->Render(view, proj, m_position, m_rotate);


	// 敵描画
	m_model->Draw(context, *states, enemyWorld, view, proj);
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	//Vector3 lightDir = Vector3{ 0.5f,1.0f,-0.5f };
	lightDir.Normalize();
	Matrix shadowMatrix = Matrix::CreateShadow(lightDir, Plane(0, 1, 0, -0.01f));
	Matrix mat = enemyWorld * shadowMatrix;
	m_model->Draw(context, *states, mat, view, proj, false, [&]()
				  {
					  context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
					  context->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
					  context->RSSetState(states->CullCounterClockwise());
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

	for (const auto& bullet : m_bullets)bullet->Render(view, proj);

	m_primitiveBatch->Begin();
	if (!m_isHit)
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::Black);
		else					DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::White);
	}
	else
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::Blue);
		else					 DX::Draw(m_primitiveBatch.get(), m_enemyBoundingSphere, Colors::Tomato);

	}
	m_primitiveBatch->End();

}
// 更新
void Enemy::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{

	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit);
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// 攻撃態勢なら
	{
		// 攻撃のクールダウンタイムを管理
		m_attackCooldown -= elapsedTime;
		if (m_attackCooldown <= 0.0f)
		{
			// 弾を発射
			auto bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize(m_commonResources);
			m_rotation = m_enemyAI->GetRotation();
			// クォータニオンから方向ベクトルを計算
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_rotation);
			bullet->MakeBall(GetPosition(), direction, playerPos);
			m_bullets.push_back(std::move(bullet));
			m_attackCooldown = 3.0f; // 次の攻撃までのクールダウンタイムを3秒に設定
		}
	}
	UpdateBullets(elapsedTime);
	m_enemyBoundingSphere.Center = m_position;
	m_enemyBoundingSphere.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// 敵同士が衝突したら押し戻す
void Enemy::CheckHitOtherEnemy(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
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



void Enemy::UpdateBullets(float elapsedTime)
{
	std::vector<std::unique_ptr<EnemyBullet>> newBullets;

	// 弾の更新と有効な弾を新しいリストに移動する
	for (auto& bullet : m_bullets)
	{
		bullet->Update(m_position, elapsedTime); // 弾の更新
		SetBulletBoundingSphere(bullet->GetBoundingSphere());
		m_isBullethit = GetBulletBoundingSphere().Intersects(GetPlayerBoundingSphere());
		if (m_isBullethit)
		{
			SetBulletHitToPlayer(m_isBullethit);
			newBullets.push_back(std::move(bullet));

		}
		else if (!bullet->IsExpired())//寿命を迎えた弾はローカル変数に葬る
		{
			newBullets.push_back(std::move(bullet));

		}
	}

	// m_bullets を新しいリストで置き換える
	m_bullets = std::move(newBullets);
}
