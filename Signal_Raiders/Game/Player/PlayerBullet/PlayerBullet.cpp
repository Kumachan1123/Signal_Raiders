/*
*	@file	PlayerBullet.cpp
*	@brief	プレイヤーの弾クラス
*/
#include <pch.h>
#include "PlayerBullet.h"
/*
*	@brief	コンストラクタ
*	@details プレイヤーの弾クラスのコンストラクタ
*	@param	なし
*	@return	なし
*/
PlayerBullet::PlayerBullet()
	: m_position{ DirectX::SimpleMath::Vector3::Zero }// 弾の位置
	, m_velocity{ DirectX::SimpleMath::Vector3::Zero }// 弾の移動量
	, m_direction{ DirectX::SimpleMath::Vector3::Zero }// 弾の移動方向
	, m_pCommonResources{}// 共通リソース
	, m_time{ 0.0f }// 経過時間
	, m_additionalDamage{ 0 }// 追加ダメージ
	, m_angle{ 0.0f }// 弾の角度
	, m_pModel{ nullptr }// モデルポインター
{
}
/*
*	@brief	デストラクタ
*	@details プレイヤーの弾クラスのデストラクタ
*	@param	なし
*	@return	なし
*/
PlayerBullet::~PlayerBullet()
{
	// ピクセルシェーダーの解放
	m_pPixelShader.Reset();
	// 軌跡ポインターの解放
	m_pBulletTrail.reset();
	// 共通リソースの解放
	m_pCommonResources = nullptr;
	// モデルポインターの解放
	m_pModel = nullptr;
}
/*
*	@brief	初期化
*	@details プレイヤーの弾クラスの初期化
*	@param CommonResources* resources 共通リソース
*	@return	なし
*/
void PlayerBullet::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースの設定
	m_pCommonResources = resources;
	// デバイスの取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// 当たり判定可視化用クラスの初期化
	DrawCollision::Initialize(m_pCommonResources);
	// 弾の軌道生成
	m_pBulletTrail = std::make_unique<Particle>(ParticleUtility::Type::PLAYERTRAIL, BulletParameters::PLAYER_BULLET_SIZE);
	// 弾の軌道生成の初期化
	m_pBulletTrail->Initialize(m_pCommonResources);
	// 影用のピクセルシェーダー読み込み
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");
	// ピクセルシェーダーの作成
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));
	// マネージャーからモデルを取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("PlayerBullet");
	// 弾の移動方向
	m_direction = Vector3::Zero;
	// 弾の移動量
	m_velocity = Vector3::Zero;
	// 弾の位置
	m_position = Vector3::Zero;
	// 弾の中心座標
	m_boundingSphere.Center = m_position;
	// 弾の半径
	m_boundingSphere.Radius = BulletParameters::COLLISION_RADIUS;
}
/*
*	@brief	更新
*	@details プレイヤーの弾クラスの更新
*	@param float elapsedTime 経過時間
*	@return	なし
*/
void PlayerBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 弾の回転速度を加算
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	// 角度を0～360度に制限する
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);
	// カメラが向いている方向に速度を与える
	m_velocity += m_direction;
	// 速度を正規化
	if (m_velocity.LengthSquared() > 0)m_velocity.Normalize();
	// 移動量を補正する
	m_velocity *= BulletParameters::ADJUST_MOVE * elapsedTime;
	// 実際に移動する
	m_position += m_velocity;
	// バウンディングスフィアの位置更新
	m_boundingSphere.Center = m_position;
	// 現在の弾の位置を軌跡リストに追加
	m_pBulletTrail->SetBulletPosition(m_position);
	// 軌跡の更新
	m_pBulletTrail->Update(elapsedTime);
	// 時間計測
	m_time += elapsedTime;
	// 弾のサイズを設定
	m_worldMatrix = Matrix::CreateScale(BulletParameters::PLAYER_BULLET_SIZE);
	// 弾の回転を設定
	m_worldMatrix *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// 弾の座標を設定
	m_worldMatrix *= Matrix::CreateTranslation(m_position);
}
/*
*	@brief	弾を生成
*	@details プレイヤーの弾クラスの弾を生成
*	@param DirectX::SimpleMath::Vector3& pos 弾の位置
*	@param DirectX::SimpleMath::Vector3& dir 弾の移動方向
*	@return	なし
*/
void PlayerBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir)
{
	using namespace DirectX::SimpleMath;
	// 初期位置を設定
	m_position = pos + BulletParameters::INITIAL_POSITION;
	// 方向を設定
	m_direction = dir;
	// 上方向に補正
	m_direction.y += BulletParameters::ADJUST_DIRECTION;
}
/*
*	@brief	描画
*	@details プレイヤーの弾クラスの描画
*	@param DirectX::SimpleMath::Matrix view ビュー行列
*	@param DirectX::SimpleMath::Matrix proj プロジェクション行列
* 	@return	なし
*/
void PlayerBullet::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// デバイスコンテキストの取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通ステートの取得
	auto states = m_pCommonResources->GetCommonStates();
	// 軌跡のビルボード行列を作成
	m_pBulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	// 軌跡描画
	m_pBulletTrail->Render(view, proj);
	// 弾描画
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj);
}
/*
*	@brief	影の描画
*	@details プレイヤーの弾クラスの影の描画
*	@param DirectX::SimpleMath::Matrix view ビュー行列
* 	@param DirectX::SimpleMath::Matrix proj プロジェクション行列
*	@return	なし
*/
void PlayerBullet::RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストの取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通ステートの取得
	auto states = m_pCommonResources->GetCommonStates();
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	// ライトの方向を設定
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
	// ワールド行列と影行列を掛ける
	shadowMatrix = m_worldMatrix * shadowMatrix;
	// 影を描画
	m_pModel->Draw(context, *states, shadowMatrix, view, proj, true, [&]()
		{
			// ブレンドステートの設定
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// 深度ステンシルステートの設定
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// ラスタライザーステートの設定
			context->RSSetState(states->CullNone());
			// ピクセルシェーダーの設定
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
}
/*
*	@brief	当たり判定の描画
*	@details プレイヤーの弾クラスの当たり判定の描画
*	@param DirectX::SimpleMath::Matrix view ビュー行列
*	@param DirectX::SimpleMath::Matrix proj プロジェクション行列
*	@return	なし
*/
void PlayerBullet::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG// デバッグビルド時のみ実行
	// 当たり判定の描画開始
	DrawCollision::DrawStart(view, proj);
	// 当たり判定の描画
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Blue);
	// 当たり判定の描画終了
	DrawCollision::DrawEnd();
#endif
}