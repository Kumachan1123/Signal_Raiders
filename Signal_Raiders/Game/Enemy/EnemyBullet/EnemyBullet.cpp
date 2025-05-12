#include "pch.h"
#include "EnemyBullet.h"

using namespace DirectX::SimpleMath;
//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
EnemyBullet::EnemyBullet(float size)
	:m_position{}
	, m_velocity{}
	, m_commonResources{}
	, m_time{ 0.0f }
	, m_elapsedTime{ 0.0f }
	, m_angle{ 0.0f }
	, m_size{ size }
	, m_spiralAngle{ 0.0f }
	, m_bulletSpeed{ 0.0f }
	, m_distance{ 5.0f }
	, m_rotateDirection{ 1 }
	, m_rotationSpeed{ 0.0f }
	, m_height{ 0.0f }
	, m_isExpand{ false }
	, m_isShot{ false }
	, m_pShooter{ nullptr }
	, m_pSpecialBullet{ nullptr }
	, m_pNormalBullet{ nullptr }
	, m_pSpeedBullet{ nullptr }
	, m_pEnemyBullet{ nullptr }
	, m_bulletType{ BulletType::NORMAL }
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
	m_pEnemyBullet = EnemyBulletFactory::CreateBullet(m_bulletType);// ファクトリで生成
	m_pEnemyBullet->SetEnemyBullet(this);// 敵弾ポインターを設定する
	// 境界球の初期化
	DrawCollision::Initialize(m_commonResources);
	// 影用のピクセルシェーダー
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Bullet.cmo", *fx);
	// モデルのエフェクトを設定する
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			// ベイシックエフェクトを取得する
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, .2, 0, 1));// ディフューズカラーを設定する
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red);// アンビエントライトカラーを設定する
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato);// エミッシブカラーを設定する

		});
	// 弾の軌道生成
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);
	m_bulletTrail->Initialize(resources);
	m_direction = Vector3::Zero;// 方向を初期化
	m_velocity = Vector3::Zero;// 速度を初期化
	m_position = Vector3::Zero;// 位置を初期化
	// 境界球の初期化
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
	m_pEnemyBullet->Initialize();
}

// 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
bool EnemyBullet::IsExpired() const
{
	if (m_bulletType == BulletType::SPECIAL)
		return GetTime() >= BulletParameters::SPIRAL_BULLET_LIFETIME;
	else
		return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;
}

// 更新
void EnemyBullet::Update(float elapsedTime)
{
	// 角度を増加させる
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// 角度を0～360度に制限する

	// 弾の更新
	m_pEnemyBullet->Update(elapsedTime);

	// 現在の弾の位置を軌跡リストに追加
	m_bulletTrail->SetBulletPosition(m_position);

	// 軌跡の更新
	m_bulletTrail->Update(elapsedTime);

	// 経過時間を更新
	m_time += elapsedTime;

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
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
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


// 弾のワールド行列を取得
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

