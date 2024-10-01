/*
	@file	Boss.cpp
	@brief	ボスクラス
	作成者：くまち
*/
#include "pch.h"
#include "Boss.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/ReadData.h"
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
Boss::Boss(Player* pPlayer)
	: m_commonResources{}
	, m_currentHP{ 500 }
	, m_attackCooldown{ 3.0f }
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_bossBullets{}
	, m_bossBSToPlayerArea{}
	, m_bossBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{}
	, m_isHit{}
	, m_isHitToOtherBoss{}
	, m_isHitToPlayerBullet{}
	, m_isBullethit{}
	, m_audioManager{ AudioManager::GetInstance() }
	, m_pPlayer{ pPlayer }
{
}

// デストラクタ
Boss::~Boss() {}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void Boss::Initialize(CommonResources* resources, int hp)
{
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
	// 敵の体力を設定
	m_currentHP = hp;
	// HPBar生成
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// 弾全体生成
	/*m_bossBullets = std::make_unique<EnemyBullets>(this);
	m_bossBullets->Initialize(resources);*/
	// モデル生成
	m_enemyModel = std::make_unique<EnemyModel>();
	m_enemyModel->Initialize(resources);
	// 座標を設定
	m_position = Vector3(0.0f, 8.0f, 0.0f);
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// オーディオマネージャー
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");
}
// 更新
void Boss::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	//m_enemyModel->Update(elapsedTime, m_enemyAI->GetState());// モデルのアニメーション更新
	//m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AIの更新
	m_audioManager->Update();// オーディオマネージャーの更新
	//if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// 攻撃態勢なら
	//{
	//	m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
	//	// 攻撃のクールダウンタイムを管理
	//	if (m_attackCooldown <= 0.1f)
	//	{
	//		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
	//		// クォータニオンから方向ベクトルを計算
	//		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
	//		// 弾を発射
	//		m_enemyBullets->CreateBullet(GetPosition(), direction, playerPos);
	//		// クールダウンタイムをリセット
	//		m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
	//	}
	//}
	//m_bossBullets->Update(elapsedTime, GetPosition());// 敵の弾の更新
	m_bossBS.Center = m_position;
	m_bossBS.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}
// 描画
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 基準となる座標やら回転やら
	Matrix world =/* Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())
		* */Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
	// 敵のサイズを設定
	//Matrix enemyWorld = Matrix::CreateScale(m_enemyAI->GetScale());
	// 敵の座標を設定
	//enemyWorld *= world;
	// HPBar描画
	m_HPBar->Render(view, proj, m_position, m_rotate);
	// 敵描画	
	m_enemyModel->Render(context, states, world, view, proj);
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	//// 影行列の元を作る
	//Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, -0.01f));
	//enemyWorld *= shadowMatrix;
	//// 影描画
	//m_model->Draw(context, *states, enemyWorld * Matrix::Identity, view, proj, true, [&]()
	//			  {
	//				  context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
	//				  context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 0);
	//				  context->RSSetState(states->CullClockwise());
	//				  context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	//			  });
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
	//m_bossBullets->Render(view, proj);
#ifdef _DEBUG
	m_primitiveBatch->Begin();
	if (!m_isHit)
	{
		if (!m_isHitToOtherBoss)DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::Black);
		else					DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::White);
	}
	else
	{
		if (!m_isHitToOtherBoss)DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::Blue);
		else					 DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::Tomato);

	}
	m_primitiveBatch->End();
#endif

}
