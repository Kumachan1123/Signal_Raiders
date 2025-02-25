#include "pch.h"
#include "EnemyBullet.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/KumachiLib/KumachiLib.h"
#include <SimpleMath.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
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
void EnemyBullet::Initialize(CommonResources* resources, BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_bulletType = type;// 弾の種類を設定する

	switch (m_bulletType)
	{
	case EnemyBullet::BulletType::NORMAL:// 直線弾
		m_pNormalBullet = std::make_unique<NormalBullet>();
		m_pEnemyBullet = m_pNormalBullet.get();
		break;
	case EnemyBullet::BulletType::SPECIAL:// 特殊攻撃の弾
		m_pSpecialBullet = std::make_unique<SpecialBullet>();
		m_pEnemyBullet = m_pSpecialBullet.get();
		break;
	case EnemyBullet::BulletType::SPEED:// 垂直直進弾
		m_pSpeedBullet = std::make_unique<SpeedBullet>();
		m_pEnemyBullet = m_pSpeedBullet.get();
		break;
	}
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
			basicEffect->SetDiffuseColor(DirectX::Colors::Pink);// ディフューズカラーを設定する
			basicEffect->SetAmbientLightColor(DirectX::Colors::Magenta);// アンビエントライトカラーを設定する
			basicEffect->SetEmissiveColor(DirectX::Colors::Magenta);// エミッシブカラーを設定する

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

	return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;
}

// 更新
void EnemyBullet::Update(float elapsedTime)
{
	// 角度を増加させる
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// 角度を0～360度に制限する

	//// 弾の種類によって処理を分岐
	//switch (m_bulletType)
	//{
	//case BulletType::SPECIAL:// 特殊攻撃の弾
	//	SpiralBullet(elapsedTime);
	//	break;
	//case BulletType::NORMAL:// 直線弾
	//	StraightBullet(elapsedTime);
	//	break;
	//case BulletType::SPEED:// 垂直直進弾
	//	VerticalBullet(elapsedTime);
	//	break;
	//default:
	//	break;
	//}

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

// 直線弾
void EnemyBullet::StraightBullet(float elapsedTime)
{
	// プレイヤーの方向ベクトルを計算
	DirectX::SimpleMath::Vector3 toPlayer = m_target - m_enemyPosition;
	// ベクトルを正規化
	if (toPlayer.LengthSquared() > 0)
	{
		toPlayer.Normalize();
	}
	// 弾の方向をプレイヤーの方向に向ける
	m_direction = toPlayer;
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;// 下に若干ずらす
	// 弾の速度を遅くする
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;
	// プレイヤーの方向に向かって弾を飛ばす
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;//境界球に座標を渡す
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

// 垂直直進弾
void EnemyBullet::VerticalBullet(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	if (m_position.y >= BulletParameters::VERTICAL_BULLET_LANDING_POSITION)// 着弾位置に到達していない
	{
		// 真下に落とす
		m_velocity = BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
	}
	else//着弾してから
	{
		// プレイヤーの方向ベクトルを計算
		Vector3 toPlayer = m_target - m_enemyPosition;
		// ベクトルを正規化
		if (toPlayer.LengthSquared() > 0)
		{
			toPlayer.Normalize();
		}
		// 弾の方向をプレイヤーの方向に向ける
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z) * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
		m_velocity = m_direction;
	}
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;
}


// 特殊攻撃の弾
void EnemyBullet::SpiralBullet(float elapsedTime)
{
	// 経過時間を更新
	m_elapsedTime = elapsedTime;
	// 時計回りに回転するための角度
	m_spiralAngle += m_rotationSpeed * elapsedTime;
	// XY平面上で円運動 (時計回り)
	float xOffset = cosf(m_spiralAngle) * m_distance;
	float zOffset = sinf(m_spiralAngle) * m_distance;
	// もともとのY座標の動きは変更しない
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);
	Expand();// 子オブジェクトを展開
	Shot();// 子オブジェクトを発射
	StopExpand();// 子オブジェクトを収納
	ComeBack();// 子オブジェクトを戻す
	// プレイヤーに向かいつつスパイラルを描いて移動
	m_position = m_basePos + m_positionOffSet;
	m_boundingSphere.Center = m_position;
	// 弾の寿命に応じてフラグを切り替える
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)
	{
		SetIsShot(true);
	}

}

// 子オブジェクトを展開
void EnemyBullet::Expand()
{
	if (!GetIsExpand())return;
	m_rotationSpeed = 1.0f; // 速度調整用（値を大きくすると速く回転する）
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);
	m_height = 2.0f;
}

// 子オブジェクトを発射
void EnemyBullet::Shot()
{

	if (!GetIsShot()) return;
	m_rotationSpeed = 3.0f;
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);
	m_basePos = Lerp(m_basePos, m_currentTarget, m_elapsedTime * 2);

}

// 子オブジェクトを収納
void EnemyBullet::StopExpand()
{
	if (GetIsExpand())return;
	m_rotationSpeed = 0.0f;
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);
	m_height = 1.50f;
}

// 子オブジェクトを戻す
void EnemyBullet::ComeBack()
{
	if (GetIsShot()) return;
	//m_look.Normalize();// プレイヤーが向いている方向を正規化
	//// 基準点を親が向いている方向に動かす
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);
	// 基準点を目的地に向かって線形補完
	m_basePos = Lerp(m_basePos, m_enemyPosition, m_elapsedTime * 50);

}
