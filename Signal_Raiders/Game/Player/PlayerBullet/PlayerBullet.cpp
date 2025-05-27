/*
*	@file	PlayerBullet.cpp
*	@brief	プレイヤーの弾クラス
*/
#include "pch.h"
#include "PlayerBullet.h"
/*
*	@brief	コンストラクタ
*	@details プレイヤーの弾クラスのコンストラクタ
*	@param [in]	なし
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
*	@param [in]	なし
*	@return	なし
*/
PlayerBullet::~PlayerBullet()
{
	m_model.reset();// モデルの解放
	m_pPixelShader.Reset();// ピクセルシェーダーの解放
	m_bulletTrail.reset();// 軌跡ポインターの解放
	m_pCommonResources = nullptr;// 共通リソースの解放
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
	m_pCommonResources = resources;// 共通リソースの設定
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスの取得
	DrawCollision::Initialize(m_pCommonResources);// 当たり判定可視化用クラスの初期化
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::PLAYERTRAIL, BulletParameters::PLAYER_BULLET_SIZE);// 弾の軌道生成
	m_bulletTrail->Initialize(m_pCommonResources);// 弾の軌道生成の初期化
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// 影用のピクセルシェーダー読み込み
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// ピクセルシェーダーの作成
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("PlayerBullet");// マネージャーからモデルを取得
	m_direction = Vector3::Zero;// 弾の移動方向
	m_velocity = Vector3::Zero;// 弾の移動量
	m_position = Vector3::Zero;// 弾の位置
	m_boundingSphere.Center = m_position;// 弾の中心座標
	m_boundingSphere.Radius = BulletParameters::COLLISION_RADIUS;// 弾の半径
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
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;// 弾の回転速度を加算
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// 角度を0～360度に制限する
	m_velocity += m_direction;// カメラが向いている方向に速度を与える
	if (m_velocity.LengthSquared() > 0)m_velocity.Normalize();// 速度を正規化
	m_velocity *= BulletParameters::ADJUST_MOVE * elapsedTime;// 移動量を補正する
	m_position += m_velocity;// 実際に移動する
	m_boundingSphere.Center = m_position;// バウンディングスフィアの位置更新
	m_bulletTrail->SetBulletPosition(m_position);// 現在の弾の位置を軌跡リストに追加
	m_bulletTrail->Update(elapsedTime);// 軌跡の更新
	m_time += elapsedTime;// 時間計測
	m_worldMatrix = Matrix::CreateScale(BulletParameters::PLAYER_BULLET_SIZE);// 弾のサイズを設定
	m_worldMatrix *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));// 弾の回転を設定
	m_worldMatrix *= Matrix::CreateTranslation(m_position);// 弾の座標を設定
}
/*
*	@brief	弾を生成
*	@details プレイヤーの弾クラスの弾を生成
*	@param DirectX::SimpleMath::Vector3& pos 弾の位置
*	@param DirectX::SimpleMath::Vector3& dir 弾の移動方向
*	@return	なし
*/
void PlayerBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir)
{
	using namespace DirectX::SimpleMath;
	m_position = pos + BulletParameters::INITIAL_POSITION;	// 初期位置を設定
	m_direction = dir;										// 方向を設定
	m_direction.y += BulletParameters::ADJUST_DIRECTION;	// 上方向に補正
}
/*
*	@brief	描画
*	@details プレイヤーの弾クラスの描画
*	@param DirectX::SimpleMath::Matrix view ビュー行列
*	@param DirectX::SimpleMath::Matrix proj プロジェクション行列
* 	@return	なし
*/
void PlayerBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストの取得
	auto states = m_pCommonResources->GetCommonStates();// 共通ステートの取得
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);// 軌跡のビルボード行列を作成
	m_bulletTrail->Render(view, proj);// 軌跡描画
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj);// 弾描画
}
/*
*	@brief	影の描画
*	@details プレイヤーの弾クラスの影の描画
*	@param DirectX::SimpleMath::Matrix view ビュー行列
* 	@param DirectX::SimpleMath::Matrix proj プロジェクション行列
*	@return	なし
*/
void PlayerBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストの取得
	auto states = m_pCommonResources->GetCommonStates();// 共通ステートの取得
	Vector3 lightDir = Vector3::UnitY;// ライトの方向
	lightDir.Normalize();// 正規化
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);// 影行列の元を作る
	shadowMatrix = m_worldMatrix * shadowMatrix;// ワールド行列と影行列を掛け算
	m_pModel->Draw(context, *states, shadowMatrix, view, proj, true, [&]()// 影描画
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// ブレンドステートの設定
			context->OMSetDepthStencilState(states->DepthNone(), 0);// 深度ステンシルステートの設定
			context->RSSetState(states->CullNone());	// ラスタライザーステートの設定
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// ピクセルシェーダーの設定
		});
}
/*
*	@brief	当たり判定の描画
*	@details プレイヤーの弾クラスの当たり判定の描画
*	@param DirectX::SimpleMath::Matrix view ビュー行列
*	@param DirectX::SimpleMath::Matrix proj プロジェクション行列
*	@return	なし
*/
void PlayerBullet::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// 未使用警告非表示
	UNREFERENCED_PARAMETER(proj);// 未使用警告非表示
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG// デバッグビルド時のみ実行
	DrawCollision::DrawStart(view, proj);// 当たり判定の描画開始
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Blue);// 当たり判定の描画
	DrawCollision::DrawEnd();// 当たり判定の描画終了
#endif
}
