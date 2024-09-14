/*
	@file	PlayerBullet.cpp
	@brief	プレイヤーの弾クラス
	作成者：くまち
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Player/PlayerBullet/PlayerBullet.h"
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
PlayerBullet::PlayerBullet()
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
PlayerBullet::~PlayerBullet()
{
}
void PlayerBullet::MoveStop()
{
}

void PlayerBullet::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
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
	// 弾の軌道ポインター
	m_bulletTrail = std::make_unique<BulletTrail>();
	m_bulletTrail->Initialize(m_commonResources);

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Bullet.cmo", *fx);
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
						   {
							   // ベイシックエフェクトを取得する
							   auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
							   // ディフューズカラーを設定する
							   basicEffect->SetDiffuseColor(DirectX::Colors::Lime);
							   // スペキュラカラーを設定する
							   basicEffect->SetSpecularColor(DirectX::Colors::White);
							   // スペキュラパワーを設定する
							   basicEffect->SetSpecularPower(50.0f);
							   // エミッションカラーを設定する
							   basicEffect->SetEmissiveColor(DirectX::XMVECTOR{ 0.3, 0.3, 0.3, 1 });
						   });


	m_direction = Vector3::Zero;
	m_velocity = Vector3::Zero;
	m_position = Vector3::Zero;
	m_boundingSphere.Center = m_position;
	m_boundingSphere.Radius = .25;

}
// 更新
void PlayerBullet::Update(DirectX::SimpleMath::Vector3& Direction, float elapsedTime)
{
	UNREFERENCED_PARAMETER(Direction);
	m_angle += 6.0f;
	if (m_angle > 360)m_angle = 0;
	// カメラが向いている方向に速度を与える
	m_velocity += m_direction;
	// 移動量を正規化する
	if (m_velocity.LengthSquared() > 0)m_velocity.Normalize();
	// 移動量を補正する
	m_velocity *= 0.75f;
	// 実際に移動する
	m_position += m_velocity;
	// バウンディングスフィアの位置更新
	m_boundingSphere.Center = m_position;

	// 現在の弾の位置を軌跡リストに追加
	m_bulletTrail->SetBulletPosition(m_position);
	// 軌跡の更新

	m_bulletTrail->Update(elapsedTime);
	// 時間計測
	m_time += elapsedTime;

}
// 弾の初期位置を設定
void PlayerBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir)
{
	using namespace DirectX::SimpleMath;
	m_position = pos;
	m_position += Vector3(0.0f, -1.0f, 0.0f);
	m_direction = dir;
	m_direction.y += 0.0375f;
}
void PlayerBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 弾のサイズを設定
	Matrix bulletWorld = Matrix::CreateScale(SIZE);
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// 弾の座標を設定
	bulletWorld *= Matrix::CreateTranslation(m_position);
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);
	// 弾描画
	m_model->Draw(context, *states, bulletWorld, view, proj);
	// 軌跡描画
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	m_bulletTrail->Render(view, proj);
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//　デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);

	// 境界球の変換を同じワールドマトリックスに基づいて行う
	BoundingSphere transformedBoundingSphere = m_boundingSphere;
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);
#ifdef _DEBUG
	// 描画する
	m_primitiveBatch->Begin();
	DX::Draw(m_primitiveBatch.get(), m_boundingSphere, DirectX::Colors::Red);
	m_primitiveBatch->End();
#endif
}
