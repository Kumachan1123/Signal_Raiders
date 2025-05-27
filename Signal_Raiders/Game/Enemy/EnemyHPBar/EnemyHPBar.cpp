/*
*	@file	EnemyHPBar.cpp
*	@brief	敵HPBarクラス
*/
#include <pch.h>
#include "EnemyHPBar.h"


/*
*	@brief	インプットレイアウト
*	@return 	なし
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyHPBar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details 敵HPBarクラスのコンストラクタ
*	@param	なし
*	@return なし
*/
EnemyHPBar::EnemyHPBar()
	: m_pCommonResources{}// 共通リソース
	, m_maxHP(100)// 敵の最大HP
	, m_currentHP(100)// 敵の現在HP
	, m_displayedHP(100)// 敵の表示HP
	, m_lerpSpeed(5.0f)// 補間速度
	, m_scale(1.5f)// スケール
	, m_time{ 0.0f }// 経過時間
	, m_isDead(false)// 死亡フラグ
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	using namespace DirectX::SimpleMath;
	// HPバーの頂点情報を設定する
	m_hpbarVert[0] = { Vector3(HPBAR_X_MIN,HPBAR_Y_MAX,0.0f),Vector2(0.0f,0.0f) };	// 左上
	m_hpbarVert[1] = { Vector3(HPBAR_X_MAX,HPBAR_Y_MAX,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarVert[2] = { Vector3(HPBAR_X_MAX,HPBAR_Y_MIN,0.0f),Vector2(1.0f,1.0f) };	// 右下
	m_hpbarVert[3] = { Vector3(HPBAR_X_MIN,HPBAR_Y_MIN,0.0f),Vector2(0.0f,1.0f) };	// 左下
	// HPバーの背景の頂点情報を設定する
	m_hpbarBackVert[0] = { Vector3(HPBARBACK_X_MIN,HPBARBACK_Y_MAX,0.0f),Vector2(0.0f,0.0f) };// 左上
	m_hpbarBackVert[1] = { Vector3(HPBARBACK_X_MAX,HPBARBACK_Y_MAX,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarBackVert[2] = { Vector3(HPBARBACK_X_MAX,HPBARBACK_Y_MIN,0.0f),Vector2(1.0f,1.0f) };	// 右下
	m_hpbarBackVert[3] = { Vector3(HPBARBACK_X_MIN,HPBARBACK_Y_MIN,0.0f),Vector2(0.0f,1.0f) };// 左下

}

/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return なし
*/
EnemyHPBar::~EnemyHPBar()
{
	m_pCommonResources = nullptr;	// 共通リソースをnullptrに設定
}

/*
*	@brief	画像を読み込む
*	@details	敵HPバーのテクスチャを読み込む
*	@param	const wchar_t*　path	画像のパス
*	@return なし
*/
void EnemyHPBar::LoadTexture(const wchar_t* path)
{
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャを格納するポインタ
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャを読み込む
	m_pGaugeTexture.push_back(texture);// テクスチャを格納する
}
/*
*	@brief	シェーダーを作成する
*	@details	敵HPバーのシェーダーを作成する
*	@param	なし
*	@return なし
*/
void EnemyHPBar::CreateShaders()
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(),
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);// シェーダー作成クラスの初期化
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/EnemyHP/VS_EnemyHP.cso", m_pVertexShader);// 頂点シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/EnemyHP/PS_EnemyHP.cso", m_pPixelShader);// ピクセルシェーダー作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダー(使わないのでnullptr)
}
/*
*	@brief	初期化
*	@details	敵HPバーの初期化
*	@param	CommonResources* resources	共通リソース
*	@return なし
*/
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// 共通リソースを取得
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());	// 板ポリゴン描画用クラスの初期化
	CreateShaders();	// シェーダーの作成
	LoadTexture(L"Resources/Textures/EnemyHPBar.png");// テクスチャ読み込み
	m_displayedHP = (float)(m_maxHP);// 初期HP
}
/*
*	@brief	更新
*	@details	敵HPバーの更新
*	@param	float elapsedTime	経過時間
*	@return なし
*/
void EnemyHPBar::Update(float elapsedTime)
{
	m_time += elapsedTime;// 経過時間を加算
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);	// HPを滑らかに更新するための線形補間
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);	// 表示HPに基づいてHP割合を計算
	float maxBarWidth = 3.0f;	// HPバーの最大時の幅を設定 
	float currentBarWidth = maxBarWidth * hpPercentage;	// HPバーの幅を計算
	// HPバーの頂点を設定 
	m_hpbarVert[0].position.x = BAR_LEFT;                    // 左上
	m_hpbarVert[1].position.x = BAR_LEFT + currentBarWidth;  // 右上
	m_hpbarVert[2].position.x = BAR_LEFT + currentBarWidth;  // 右下
	m_hpbarVert[3].position.x = BAR_LEFT;                    // 左下
	if (m_hpbarVert[1].position.x <= BAR_LEFT)m_isDead = true;// HPバーの幅が0以下になったら死亡

}
/*
*	@brief	描画
*	@details	敵HPバーの描画
*	@param	DirectX::SimpleMath::Matrix view	ビュー行列
*	@param	DirectX::SimpleMath::Matrix proj	プロジェクション行列
*	@param	DirectX::SimpleMath::Vector3 pos	座標
*	@param	DirectX::SimpleMath::Vector3 rot	回転
*	@return なし
*/
void EnemyHPBar::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	Matrix billboardMatrix = view.Invert();	// ビルボード行列を作成
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	Matrix worldBillboard{};	// ビルボードをアフィン変換
	worldBillboard = billboardMatrix;// ビルボード行列を代入
	worldBillboard *= Matrix::CreateScale(m_scale);	// スケールをかける
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));	// 回転をかける
	worldBillboard *= Matrix::CreateTranslation(pos);	// ビルボードの位置を設定
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));	// 回転をかける
	m_constBuffer.matView = view.Transpose();	// ビュー設定
	m_constBuffer.matProj = proj.Transpose();	// プロジェクション設定
	m_constBuffer.matWorld = worldBillboard.Transpose();// ワールド設定
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// 深度ステンシルステート
	// HPbar(背景)描画///////////////////////////////////////////////////////////////////////////////
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);	// 色設定 
	CreateBuffer();// シェーダーに渡すバッファを作成
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pGaugeTexture);	// 描画準備
	m_pDrawPolygon->DrawTexture(m_hpbarBackVert);	// ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();	// シェーダの登録を解除しておく
	// HPbar(緑)描画///////////////////////////////////////////////////////////////////////////////
	m_constBuffer.colors = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);// 色設定
	CreateBuffer();// シェーダーに渡すバッファを作成
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pGaugeTexture);	// 描画準備
	m_pDrawPolygon->DrawTexture(m_hpbarVert);	// ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();	// シェーダの登録を解除しておく
}

/*
*	@brief	シェーダーに渡すバッファを作成
*	@details	敵HPバーのシェーダーに渡すバッファを作成する
*	@param	なし
*	@return なし
*/
void EnemyHPBar::CreateBuffer()
{
	using namespace DirectX::SimpleMath;
	m_constBuffer.hp = Vector4(float(m_currentHP), float(m_maxHP), 0, 0);	// 時間設定
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);	// 受け渡し用バッファの内容更新
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);	// シェーダをセットする
}