#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
EnemyBullet::EnemyBullet(float size)
	:m_position{}
	, m_velocity{}
	, m_commonResources{}
	, m_time{ 0.0f }
	, m_angle{ 0.0f }
	, m_size{ size }
	, m_spiralAngle{ 0.0f }
	, m_bulletSpeed{ 0.5f }
	, m_rotateDirection{ 1 }
	, m_bulletType{ BulletType::STRAIGHT }
{
}
//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
EnemyBullet::~EnemyBullet()
{
}
void EnemyBullet::Initialize(CommonResources* resources, BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_bulletType = type;// 弾の種類を設定する
	DrawCollision::Initialize(m_commonResources);
	// 影用のピクセルシェーダー
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_EnemyShadow.cso");
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
			basicEffect->SetAlpha(.5f);// アルファ値を設定する
			basicEffect->SetDiffuseColor(DirectX::Colors::Pink);// ディフューズカラーを設定する
			basicEffect->SetAmbientLightColor(DirectX::Colors::Pink);// アンビエントライトカラーを設定する
			basicEffect->SetEmissiveColor(DirectX::Colors::Magenta);// エミッシブカラーを設定する

		});
	// 弾の軌道生成
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);
	m_bulletTrail->Initialize(resources);
	m_direction = Vector3::Zero;// 方向を初期化
	m_velocity = Vector3::Zero;// 速度を初期化
	m_position = Vector3::Zero;// 位置を初期化

	m_boundingSphere.Center = m_position;
	m_boundingSphere.Radius = m_size * 2;
}
// 弾の初期位置を設定
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	m_position = pos;
	m_direction = dir;
	m_target = target;
	m_bulletSpeed = 0.5f;
}

// 更新
void EnemyBullet::Update(DirectX::SimpleMath::Vector3& pos, float elapsedTime)
{
	// 角度を増加させる
	m_angle += 6.0f;
	if (m_angle > 360) m_angle = 0;

	if (m_bulletType == BulletType::SPIRAL)   SpiralBullet();
	else if (m_bulletType == BulletType::STRAIGHT) StraightBullet(pos);
	else if (m_bulletType == BulletType::VERTICAL) VerticalBullet(pos);

	// 現在の弾の位置を軌跡リストに追加
	m_bulletTrail->SetBulletPosition(m_position);

	// 軌跡の更新
	m_bulletTrail->Update(elapsedTime);
	m_time += elapsedTime; // 経過時間を更新

}

void EnemyBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 軌跡描画
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	m_bulletTrail->Render(view, proj);
	// 弾描画
	m_model->Draw(context, *states, BulletWorldMatrix(), view, proj);

}

void EnemyBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	shadowMatrix = BulletWorldMatrix() * shadowMatrix;
	// 影描画
	m_model->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			context->RSSetState(states->CullClockwise());
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});
}

void EnemyBullet::RenderBoundingSphere(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);
	// 境界球の変換を同じワールドマトリックスに基づいて行う
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Red);
	DrawCollision::DrawEnd();
#endif

}

// 直線弾
void EnemyBullet::StraightBullet(DirectX::SimpleMath::Vector3& pos)
{	// プレイヤーの方向ベクトルを計算
	DirectX::SimpleMath::Vector3 toPlayer = m_target - pos;
	// ベクトルを正規化
	if (toPlayer.LengthSquared() > 0)
	{
		toPlayer.Normalize();
	}
	// 弾の方向をプレイヤーの方向に向ける
	m_direction = toPlayer;
	m_direction.y -= 0.1f;// 下に若干ずらす
	// 弾の速度を遅くする
	float bulletSpeed = .2f; // 適当な速度を設定する（任意の値、調整可能）
	m_velocity = m_direction * bulletSpeed;
	// プレイヤーの方向に向かって弾を飛ばす
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;//境界球に座標を渡す
}

DirectX::SimpleMath::Matrix EnemyBullet::BulletWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	// 弾のサイズを設定
	Matrix bulletWorld = Matrix::CreateScale(m_size);
	// 弾の自転
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// 弾の座標を設定
	bulletWorld *= Matrix::CreateTranslation(m_position);
	return bulletWorld;
}

// 垂直直進弾
void EnemyBullet::VerticalBullet(DirectX::SimpleMath::Vector3& pos)
{
	using namespace DirectX::SimpleMath;
	if (m_position.y >= 0.50f)
	{
		// 真下に落とす
		m_velocity = Vector3(0.0f, -0.15f, 0.0f);
	}
	else//着弾してから
	{
		// プレイヤーの方向ベクトルを計算
		Vector3 toPlayer = m_target - pos;
		// ベクトルを正規化
		if (toPlayer.LengthSquared() > 0)
		{
			toPlayer.Normalize();
		}
		// 弾の方向をプレイヤーの方向に向ける
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z);
		// 弾の速度を遅くする
		float bulletSpeed = 1.f; // 適当な速度を設定する（任意の値、調整可能）
		m_velocity = m_direction * bulletSpeed;
	}
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;
}

// 螺旋弾
void EnemyBullet::SpiralBullet()
{
	// プレイヤー方向ベクトルを計算
	DirectX::SimpleMath::Vector3 toPlayer = m_target - m_position;

	// プレイヤーとの距離を取得してスパイラルの半径に使う
	float distanceToPlayer = toPlayer.Length();
	if (distanceToPlayer > 0)
	{
		toPlayer.Normalize();
	}

	// プレイヤー中心に円を描くようにオフセット計算
	float spiralRadius = 1.0f; // プレイヤー中心からのスパイラルの半径
	float spiralSpeed = 5.0f;  // スパイラルの回転速度

	// 時間によってプレイヤー周りを回転する位置を設定
	DirectX::SimpleMath::Vector3 spiralOffset = {
		0.0f,
		spiralRadius * cosf(spiralSpeed * m_time * 2.5f) * distanceToPlayer * 0.1f,

		spiralRadius * sinf(spiralSpeed * m_time * 2.5f) * distanceToPlayer * 0.1f * m_rotateDirection
	};

	// プレイヤーに向かう方向とスパイラル効果をミックス
	m_direction = toPlayer + spiralOffset;
	m_direction.Normalize();

	// 弾の速度を設定
	m_velocity = m_direction * m_bulletSpeed;

	// プレイヤーに向かいつつスパイラルを描いて移動
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;
	// 弾の速度を少し遅くする
	m_bulletSpeed -= 0.001f;
}