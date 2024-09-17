#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
EnemyBullet::EnemyBullet()
	:m_position{}
	, m_velocity{}
	, m_commonResources{}
	, m_time(0.0f)
	, m_angle{ 0.0f }
{
}
//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
EnemyBullet::~EnemyBullet()
{
}
void EnemyBullet::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
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
	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Bullet.cmo", *fx);
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
						   {
							   // ベイシックエフェクトを取得する
							   auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
							   basicEffect->SetAlpha(.5f);// アルファ値を設定する
							   basicEffect->SetDiffuseColor(DirectX::Colors::Purple);// ディフューズカラーを設定する
							   basicEffect->SetAmbientLightColor(DirectX::Colors::Purple);// アンビエントライトカラーを設定する
							   basicEffect->SetEmissiveColor(DirectX::Colors::Purple);// エミッシブカラーを設定する

						   });
	// 弾の軌道生成
	m_bulletTrail = std::make_unique<BulletTrail>(ParticleUtility::Type::ENEMYTRAIL);
	m_bulletTrail->Initialize(resources);
	m_direction = Vector3::Zero;
	m_velocity = Vector3{ 0.0f,0.0f,0.0f };
	m_position = Vector3::Zero;
	m_boundingSphere.Center = m_position;
	m_boundingSphere.Radius = .3;
}
// 弾の初期位置を設定
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	m_position = pos;
	m_position.y -= 2.5f;
	m_direction = dir;
	m_target = target;
}

// 更新
void EnemyBullet::Update(DirectX::SimpleMath::Vector3& pos, float elapsedTime)
{
	m_angle += 6.0f;
	if (m_angle > 360)m_angle = 0;
	// プレイヤーの方向ベクトルを計算
	DirectX::SimpleMath::Vector3 toPlayer = m_target - pos;
	// ベクトルを正規化
	if (toPlayer.LengthSquared() > 0)
	{
		toPlayer.Normalize();
	}
	// 弾の速度を遅くする
	float bulletSpeed = .05f; // 適当な速度を設定する（任意の値、調整可能）
	m_velocity = toPlayer * bulletSpeed;
	// プレイヤーの方向に向かって弾を飛ばす
	m_position += m_velocity;
	//m_position.y += 0.05f;// プレイヤーの高さに補正
	m_boundingSphere.Center = m_position;//境界球に座標を渡す
	// 現在の弾の位置を軌跡リストに追加
	m_bulletTrail->SetBulletPosition(m_position);
	// 軌跡の更新
	m_bulletTrail->Update(elapsedTime);
	m_time += elapsedTime;// 経過時間

}
void EnemyBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 弾のサイズを設定
	Matrix bulletWorld = Matrix::CreateScale(SIZE);
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);
	// 弾の自転
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// 弾の座標を設定
	bulletWorld *= Matrix::CreateTranslation(m_position);
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);

	// 軌跡描画
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	m_bulletTrail->Render(view, proj);
	// 弾描画
	m_model->Draw(context, *states, bulletWorld, view, proj);
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
	// 境界球の変換を同じワールドマトリックスに基づいて行う
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);
#ifdef _DEBUG
	// 描画する
	m_primitiveBatch->Begin();
	DX::Draw(m_primitiveBatch.get(), m_boundingSphere, DirectX::Colors::Red);
	m_primitiveBatch->End();
#endif
}
