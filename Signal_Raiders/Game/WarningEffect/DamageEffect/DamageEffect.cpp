/*
*	@file DamageEffect.cpp
*	@brief 攻撃を受けた時の演出クラス
*/
#include "pch.h"
#include "DamageEffect.h"

// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC>  DamageEffect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief コンストラクタ
*	@details ダメージエフェクトのコンストラクタ
*	@param resources CommonResourcesクラスのポインタ
*	@return なし
*/
DamageEffect::DamageEffect(CommonResources* resources)
	: m_commonResources(resources)// リソースクラスのポインタ
	, m_time(0.0f)// 時間
	, m_constBuffer{}// コンスタントバッファ
	, m_pDR(resources->GetDeviceResources())// デバイスリソース
	, m_pPlayer(nullptr)// プレイヤー
	, m_pEnemy(nullptr)// 敵
	, m_enemyDirection{}// 攻撃してきた敵の向き
	, m_playEffect(false)// エフェクト再生フラグ
	, m_effectType(EffectType::DAMAGE)// エフェクトタイプ
	, m_pDrawPolygon(DrawPolygon::GetInstance())// 描画ポリゴンクラスのインスタンス
	, m_pCreateShader(CreateShader::GetInstance())// シェーダー作成クラスのインスタンス
{
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);// 色の初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(),// シェーダー作成クラスの初期化
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief デストラクタ
*	@details ダメージエフェクトのデストラクタ
*	@param なし
*	@return なし
*/
DamageEffect::~DamageEffect() {/* do nothing */ }
/*
*	@brief 初期化
*	@details ダメージエフェクトの初期化
*	@param なし
*	@return なし
*/
void DamageEffect::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_playEffect = true;// エフェクト再生フラグをtrueにする
	MakeShader();	// シェーダーの作成
	LoadTexture(L"Resources/Textures/WARNING.png");// テクスチャ読み込み
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// 頂点情報の初期化
	if (m_effectType == EffectType::DAMAGE)// ダメージエフェクトなら
	{
		m_constBuffer.colors = Vector4(2.0f, 0.0f, 0.0f, 1.0f);// 赤色
		m_enemyDirection = m_pPlayer->GetEnemyBulletDirection();// 攻撃してきた敵の向きを取得
	}
	else if (m_effectType == EffectType::INCOMINGENEMY)// 警戒エフェクトなら
	{
		m_constBuffer.colors = Vector4(1.0f, 0.5f, 0.0f, 1.0f);// 黄色
		m_enemyPosition = m_pEnemy->GetPosition();// 敵の位置を取得
		m_enemyDirection = m_pPlayer->GetPlayerPos() - m_enemyPosition;// プレイヤーと敵の位置の差を取得
	}
}

/*
*	@brief テクスチャリソース読み込み関数
*	@param path テクスチャのパス
*	@return なし
*/
void  DamageEffect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャリソースビュー
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャの読み込み
	m_textures.push_back(texture);// テクスチャリソースビューを格納
}
/*
*	@brief シェーダー作成
*	@details シェーダーの作成
*	@param なし
*	@return なし
*/
void  DamageEffect::MakeShader()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/DamageEffect/VS_Damage.cso", m_vertexShader);// 頂点シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/DamageEffect/PS_Damage.cso", m_pixelShader);	// ピクセルシェーダー作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();// 入力レイアウト取得
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));// コンスタントバッファ作成
	m_shaders.vs = m_vertexShader.Get();// 頂点シェーダーをセット
	m_shaders.ps = m_pixelShader.Get();// ピクセルシェーダーをセット
	m_shaders.gs = nullptr;// ジオメトリシェーダーは使わないのでnullptr
}
/*
*	@brief 更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void  DamageEffect::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	if (!m_playEffect)return;// エフェクト再生フラグがfalseなら更新しない
	m_time += elapsedTime;// 時間を加算
	if (m_time >= PLAY_TIME)// 再生時間を過ぎたら
	{
		m_time = 0.0f;// 時間を初期化
		m_constBuffer.colors.w = 0.0f;// アルファ値を初期化
		m_playEffect = false;// エフェクト再生フラグをfalseにする
		m_pPlayer->SetisPlayEffect(m_playEffect);// プレイヤーにエフェクト再生フラグを送る
		return;// 更新終了
	}
	m_constBuffer.time = Vector4(m_time);// 時間をコンスタントバッファに格納
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);// アルファ値を計算
	float angle = CalculateAngle();// 角度計算
	m_constBuffer.uv = GetUVFromAngle(angle);// 角度からUV座標を求める
}
/*
*	@brief 角度計算
*	@details 敵の向きとプレイヤーの向きの差を計算
*	@param なし
*	@return 角度
*/
float DamageEffect::CalculateAngle() const
{
	using namespace DirectX::SimpleMath;
	Vector3 playerDir = m_pPlayer->GetPlayerDir();// プレイヤーの向き
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(playerDir.x, playerDir.z);// プレイヤーと敵の向きの差
	angle = DirectX::XMConvertToDegrees(angle);// ラジアンから度に変換
	return (angle < 0) ? angle + 360 : angle;// 0から360の間に収める
}
/*
*	@brief 角度からUV座標を求める
*	@details 角度からUV座標を求める
*	@param angle 角度
*	@return UV座標
*/
DirectX::SimpleMath::Vector4 DamageEffect::GetUVFromAngle(float angle) const
{
	using namespace DirectX::SimpleMath;
	float rad = DirectX::XMConvertToRadians(angle);// 度からラジアンに変換
	float radius = UV_W - UV_C;	// 半径は左右または上下の中心からの距離
	float u = UV_C - radius * -sin(rad);// 座標を計算U
	float v = UV_C + radius * cos(rad);// 座標を計算V
	return  Vector4(u, v, 0, 0);// UV座標を返す
}
/*
*	@brief 描画
*	@details ダメージエフェクトの描画
*	@param なし
*	@return なし
*/
void  DamageEffect::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =	// 頂点情報(板ポリゴンの４頂点の座標情報）
	{
		// 頂点情報													UV情報
		VertexPositionTexture(Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),  Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),   Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),   Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),  Vector2(0.0f, 1.0f)),// 左下
	};
	m_constBuffer.matView = m_view.Transpose();// ビュー行列を転置
	m_constBuffer.matProj = m_proj.Transpose();// プロジェクション行列を転置
	m_constBuffer.matWorld = m_world.Transpose();// ワールド行列を転置
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);// コンスタントバッファの更新
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };// コンスタントバッファのセット
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->DrawSetting(// 	// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);// 描画開始
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// シェーダーの登録
	m_pDrawPolygon->DrawTexture(vertex);// 板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダー解放
}