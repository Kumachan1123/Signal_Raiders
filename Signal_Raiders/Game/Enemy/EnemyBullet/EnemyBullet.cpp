/*
*	@file	EnemyBullet.cpp
*	@brief	敵の弾クラス
*/
#include <pch.h>
#include "EnemyBullet.h"
/*
*	@brief コンストラクタ
*	@details 敵の弾クラスのコンストラクタ
*	@param size 弾のサイズ
*	@return なし
*/
EnemyBullet::EnemyBullet(float size)
	: m_position{}// 弾の座標
	, m_velocity{}// 弾の速さ
	, m_pCommonResources{}// 共通リソース
	, m_time{ 0.0f }// 経過時間
	, m_elapsedTime{ 0.0f }// 更新時間
	, m_angle{ 0.0f }// 弾の自転
	, m_size{ size }// 弾のサイズ
	, m_spiralAngle{ 0.0f }// 特殊弾の回転角度
	, m_bulletSpeed{ 0.0f }// 弾の速度
	, m_distance{ 5.0f }// 弾の敵との距離
	, m_rotateDirection{ 1 }// 回転方向（螺旋弾用:+なら右回り、-なら左回り）
	, m_rotationSpeed{ 0.0f }// 回転速度
	, m_height{ 0.0f }// 高さ
	, m_isExpand{ false }// 展開完了フラグ
	, m_isShot{ false }// 発射フラグ
	, m_pShooter{ nullptr }// 発射した敵のポインター
	, m_pModel{ nullptr }// 弾のモデル
	, m_pSpecialBullet{ nullptr }// 特殊弾
	, m_pNormalBullet{ nullptr }// 通常弾
	, m_pSpeedBullet{ nullptr }// 速い弾
	, m_pEnemyBullet{ nullptr }// 弾の種類ごとに処理を変えるためのポインター
	, m_bulletType{ BulletType::NORMAL }// 弾の種類
	, m_seVolume{ 0.0f }// SE音量
{
}
/*
*	@brief デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param なし
*	@return なし
*/
EnemyBullet::~EnemyBullet()
{
	m_pCommonResources = nullptr; // 共通リソースのポインターをnullptrに設定
	m_pEnemyBullet = nullptr; // 弾の種類ごとに処理を変えるためのポインターをnullptrに設定
	m_pNormalBullet = nullptr; // 通常弾のポインターをnullptrに設定
	m_pSpeedBullet = nullptr; // 速い弾のポインターをnullptrに設定
	m_pSpecialBullet = nullptr; // 特殊弾のポインターをnullptrに設定
	m_pModel = nullptr; // 弾のモデルをnullptrに設定
	m_pPixelShader.Reset(); // ピクセルシェーダーをリセット
	m_bulletTrail.reset(); // 弾の軌道をリセット
	m_pShooter = nullptr; // 発射した敵のポインターをnullptrに設定
}
/*
*	@brief 初期化
*	@details 敵の弾クラスの初期化
*	@param pCmmonResources 共通リソース
*	@return なし
*/
void EnemyBullet::Initialize(CommonResources* pCmmonResources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = pCmmonResources;// 共通リソースを設定
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	m_pEnemyBullet = EnemyBulletFactory::CreateBullet(m_bulletType);// ファクトリで生成
	m_pEnemyBullet->SetEnemyBullet(this);// 敵弾ポインターを設定する
	m_pEnemyBullet->SetCommonResources(m_pCommonResources);// 弾に共通リソースを設定
	m_pEnemyBullet->SetSEVolume(m_seVolume);// SE音量を設定
	DrawCollision::Initialize(m_pCommonResources);// 境界球の初期化
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// 影用のピクセルシェーダー読み込み
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// ピクセルシェーダーの作成
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("EnemyBullet");// マネージャーからモデルを取得
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);	// 弾の軌道生成
	m_bulletTrail->Initialize(pCmmonResources);// 弾の軌道初期化
	m_direction = Vector3::Zero;// 方向を初期化
	m_velocity = Vector3::Zero;// 速度を初期化
	m_position = Vector3::Zero;// 位置を初期化
	m_boundingSphere.Center = m_position;	// 境界球の位置設定
	m_boundingSphere.Radius = m_size * 2;	// 境界球の半径設定

}
/*
*	@brief 弾の発射
*	@details 弾の発射処理を行う
*	@param pos 弾の座標
*	@param dir 弾の方向
*	@param target 弾発射時のターゲットの位置
*	@return なし
*/
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	m_position = pos;// 弾の座標
	m_direction = dir;// 弾の方向
	m_target = target;// 弾発射時のターゲットの位置
	m_pEnemyBullet->Initialize();// 弾の初期化
}

/*
*	@brief 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
*	@details 弾の種類によって寿命を判定する
*	@param なし
*	@return true:寿命を超えた false:寿命を超えていない
*/
bool EnemyBullet::IsExpired() const
{
	if (m_bulletType == BulletType::SPECIAL)// 弾の種類が特殊弾の場合
		return GetTime() >= BulletParameters::SPIRAL_BULLET_LIFETIME;// 特殊弾の寿命を超えたかどうか
	else
		return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;// 特殊弾以外の寿命を超えたかどうか
}

/*
*	@brief 弾の更新
*	@details 弾の更新処理を行う
*	@param elapsedTime 更新時間
*	@return なし
*/
void EnemyBullet::Update(float elapsedTime)
{
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;	// 角度を増加させる
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// 角度を0～360度に制限する
	m_pEnemyBullet->Update(elapsedTime);	// 弾の更新
	m_bulletTrail->SetBulletPosition(m_position);	// 現在の弾の位置を軌跡リストに追加
	m_bulletTrail->Update(elapsedTime);	// 軌跡の更新
	m_time += elapsedTime;	// 経過時間を更新
}
/*
*	@brief 弾の描画
*	@details 弾の描画処理を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void EnemyBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストを取得
	auto states = m_pCommonResources->GetCommonStates();// コモンステートを取得
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);// ビルボードを作成
	m_bulletTrail->Render(view, proj);	// 軌跡描画
	m_pModel->Draw(context, *states, BulletWorldMatrix(), view, proj);	// 弾描画
}
/*
*	@brief 弾の影描画
*	@details 弾の影描画処理を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void EnemyBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストを取得
	auto states = m_pCommonResources->GetCommonStates();// コモンステートを取得
	Vector3 lightDir = Vector3::UnitY;	// ライトの方向
	lightDir.Normalize();// 正規化
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);	// 影行列の元を作る
	shadowMatrix = BulletWorldMatrix() * shadowMatrix;	// 影行列をワールド行列に適用する
	m_pModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// 影描画
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// ブレンドステートを設定
			context->OMSetDepthStencilState(states->DepthNone(), 0);// 深度ステンシルステートを設定
			context->RSSetState(states->CullClockwise());	// カリングを設定
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);	// ピクセルシェーダーを設定
		});
}
/*
*	@brief 弾の境界球を描画(デバッグ用)
*	@details 弾の境界球を描画する
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void EnemyBullet::RenderBoundingSphere(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// 未使用変数の警告を抑制
	UNREFERENCED_PARAMETER(proj);// 未使用変数の警告を抑制
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG// デバッグビルド時のみ実行
	DrawCollision::DrawStart(view, proj);// 境界球描画開始
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);// 境界球のワールド行列にスケールを設定
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);// 弾の座標を設定
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;// 境界球のワールド行列を設定
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);// 境界球の移動
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Red);// 境界球描画
	DrawCollision::DrawEnd();// 境界球描画終了
#endif
}


/*
*	@brief 弾のワールド行列を作成
*	@details 弾のワールド行列を作成する
*	@param なし
*	@return DirectX::SimpleMath::Matrix 弾のワールド行列
*/
DirectX::SimpleMath::Matrix EnemyBullet::BulletWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	Matrix bulletWorld = Matrix::CreateScale(m_size);	// 弾のサイズを設定
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));	// 弾の自転
	bulletWorld *= Matrix::CreateTranslation(m_position);	// 弾の座標を設定
	return bulletWorld;// 弾のワールド行列を返す
}

