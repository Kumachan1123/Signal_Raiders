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
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラスのポインター
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
	// 共通リソースのポインターをnullptrに設定
	m_pCommonResources = nullptr;
	// 弾のモデルをnullptrに設定
	m_pModel = nullptr;
	// ピクセルシェーダーをリセット
	m_pPixelShader.Reset();
	// 弾の軌道をリセット
	m_bulletTrail.reset();
	// 発射した敵のポインターをnullptrに設定
	m_pShooter = nullptr;
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
	// 共通リソースを設定
	m_pCommonResources = pCmmonResources;
	// デバイスを取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスを初期化
	m_pCreateShader->Initialize(device);
	// ファクトリで生成
	m_pEnemyBullet = EnemyBulletFactory::CreateBullet(m_bulletType);
	// 敵弾ポインターを設定する
	m_pEnemyBullet->SetEnemyBullet(this);
	// 弾に共通リソースを設定
	m_pEnemyBullet->SetCommonResources(m_pCommonResources);
	// SE音量を設定
	m_pEnemyBullet->SetSEVolume(m_seVolume);
	// 境界球の初期化
	DrawCollision::Initialize(m_pCommonResources);
	// 影用シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// マネージャーからモデルを取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("EnemyBullet");
	// 弾の軌道生成
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);
	// 弾の軌道初期化
	m_bulletTrail->Initialize(pCmmonResources);
	// 方向を初期化
	m_direction = Vector3::Zero;
	// 速度を初期化
	m_velocity = Vector3::Zero;
	// 位置を初期化
	m_position = Vector3::Zero;
	// 境界球の位置設定
	m_boundingSphere.Center = m_position;
	// 境界球の半径設定
	m_boundingSphere.Radius = m_size * 2;

}
/*
*	@brief 弾の発射
*	@details 弾の発射処理を行う
*	@param pos 弾の座標
*	@param dir 弾の方向
*	@param target 弾発射時のターゲットの位置
*	@return なし
*/
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir, const DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	// 弾の座標
	m_position = pos;
	// 弾の方向
	m_direction = dir;
	// 弾発射時のターゲットの位置
	m_target = target;
	// 弾の初期化
	m_pEnemyBullet->Initialize();
}

/*
*	@brief 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
*	@details 弾の種類によって寿命を判定する
*	@param なし
*	@return true:寿命を超えた false:寿命を超えていない
*/
bool EnemyBullet::IsExpired() const
{
	// 弾の種類が特殊弾の場合
	if (m_bulletType == BulletType::SPECIAL)
	{
		// 特殊弾の寿命を超えたかどうか
		return GetTime() >= BulletParameters::SPIRAL_BULLET_LIFETIME;
	}
	else// 特殊弾以外の場合
	{
		// 特殊弾以外の寿命を超えたかどうか
		return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;
	}
}

/*
*	@brief 弾の更新
*	@details 弾の更新処理を行う
*	@param elapsedTime 更新時間
*	@return なし
*/
void EnemyBullet::Update(float elapsedTime)
{
	// 角度を増加させる
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	// 角度を0～360度に制限する
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);
	// 弾の更新
	m_pEnemyBullet->Update(elapsedTime);
	// 現在の弾の位置を軌跡リストに追加
	m_bulletTrail->SetBulletPosition(m_position);
	// 軌跡の更新
	m_bulletTrail->Update(elapsedTime);
	// 経過時間を更新
	m_time += elapsedTime;
}
/*
*	@brief 弾の描画
*	@details 弾の描画処理を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void EnemyBullet::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// ビルボードを作成
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	// 軌跡描画
	m_bulletTrail->Render(view, proj);
	// 弾描画
	m_pModel->Draw(context, *states, BulletWorldMatrix(), view, proj);
}
/*
*	@brief 弾の影描画
*	@details 弾の影描画処理を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void EnemyBullet::RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	// 正規化
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
	// 影行列をワールド行列に適用する
	shadowMatrix = BulletWorldMatrix() * shadowMatrix;
	// 影描画
	m_pModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// ブレンドステートを設定
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// 深度ステンシルステートを設定
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// カリングを設定
			context->RSSetState(states->CullClockwise());
			// ピクセルシェーダーを設定
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
}
/*
*	@brief 弾の境界球を描画(デバッグ用)
*	@details 弾の境界球を描画する
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void EnemyBullet::RenderBoundingSphere(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 未使用変数の警告を抑制
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG// デバッグビルド時のみ実行
	// 境界球描画開始
	DrawCollision::DrawStart(view, proj);
	// 境界球のワールド行列にスケールを設定
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);
	// 弾の座標を設定
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);
	// 境界球のワールド行列を設定
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;
	// 境界球の移動
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);
	// 境界球描画
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Red);
	// 境界球描画終了
	DrawCollision::DrawEnd();
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
	// 弾のサイズを設定
	Matrix bulletWorld = Matrix::CreateScale(m_size);
	// 弾の自転
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// 弾の座標を設定
	bulletWorld *= Matrix::CreateTranslation(m_position);
	// 弾のワールド行列を返す
	return bulletWorld;
}

