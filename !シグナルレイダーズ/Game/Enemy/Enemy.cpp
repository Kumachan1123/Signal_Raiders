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
	: IEnemy(pPlayer)  // BaseEnemyのコンストラクタを呼び出す
	, m_enemyAI{}
	, m_enemyBullets{}

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
	IEnemy::Initialize(resources, hp);  // BaseEnemyの初期化処理

	// AI生成
	m_enemyAI = std::make_unique<EnemyAI>();
	m_enemyAI->Initialize();

	// 弾全体生成
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);

	// 乱数生成（位置の初期化）
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-50.0f, 50.0f);
	m_position.x = dist(gen);
	m_position.y = 6.0f;
	m_position.z = dist(gen);

	// AIに座標を設定
	m_enemyAI->SetPosition(m_position);

	// 境界球の初期化
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;
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
	// BaseEnemyの更新処理を呼び出し
	IEnemy::Update(elapsedTime, playerPos);

	// AIの更新
	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);

	// 攻撃処理
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())
	{
		m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
		if (m_attackCooldown <= 0.1f)
		{
			m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
			m_enemyBullets->CreateBullet(GetPosition(), direction, playerPos);
			m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
		}
	}

	// 弾の更新
	m_enemyBullets->Update(elapsedTime, GetPosition());

	// 境界球の位置を更新
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 2.0f;

	// HPバーの更新
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// オブジェクト同士が衝突したら押し戻す
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




