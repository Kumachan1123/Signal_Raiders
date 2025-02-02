/*
	@file	PlayerBullet.cpp
	@brief	プレイヤーの弾クラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/Microsoft/ReadData.h"
#include "Game/KumachiLib//DrawCollision/DrawCollision.h"
//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
PlayerBullet::PlayerBullet()
	: m_position{ DirectX::SimpleMath::Vector3::Zero }
	, m_velocity{ DirectX::SimpleMath::Vector3::Zero }
	, m_direction{ DirectX::SimpleMath::Vector3::Zero }
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
	// 当たり判定可視化用クラスの初期化
	DrawCollision::Initialize(m_commonResources);
	// 弾の軌道ポインター
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::PLAYERTRAIL, BulletParameters::PLAYER_BULLET_SIZE);
	m_bulletTrail->Initialize(m_commonResources);

	// 影用のピクセルシェーダー
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

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
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);

			// エミッションカラーを設定する
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});


	m_direction = Vector3::Zero;
	m_velocity = Vector3::Zero;
	m_position = Vector3::Zero;
	m_boundingSphere.Center = m_position;
	m_boundingSphere.Radius = BulletParameters::COLLISION_RADIUS;

}
// 更新
void PlayerBullet::Update(float elapsedTime)
{
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	Clamp(m_angle, 0.0f, 360.0f);// 角度を0～360度に制限する
	// カメラが向いている方向に速度を与える
	m_velocity += m_direction;
	// 移動量を正規化する
	if (m_velocity.LengthSquared() > 0)m_velocity.Normalize();
	// 移動量を補正する
	m_velocity *= BulletParameters::ADJUST_MOVE;
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
	m_position = pos + BulletParameters::INITIAL_POSITION;// 初期位置を設定
	m_direction = dir;// 方向を設定
	m_direction.y += BulletParameters::ADJUST_DIRECTION;// 上方向に補正
}
void PlayerBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 弾のサイズを設定
	m_worldMatrix = Matrix::CreateScale(BulletParameters::PLAYER_BULLET_SIZE);
	m_worldMatrix *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// 弾の座標を設定
	m_worldMatrix *= Matrix::CreateTranslation(m_position);
	// 軌跡描画
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	m_bulletTrail->Render(view, proj);
	// 弾描画
	m_model->Draw(context, *states, m_worldMatrix, view, proj);

}

void PlayerBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
	shadowMatrix = m_worldMatrix * shadowMatrix;
	// 影描画
	m_model->Draw(context, *states, shadowMatrix, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			context->RSSetState(states->CullNone());
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});
}

void PlayerBullet::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Blue);
	DrawCollision::DrawEnd();
#endif
}
