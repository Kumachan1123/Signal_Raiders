/*
*	@file	TitleLogo.cpp
*	@brief	タイトルロゴクラス
*/
#include "pch.h"
#include "TitleLogo.h"
/*
*	@brief インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  TitleLogo::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief コンストラクタ
*	@details タイトルロゴクラスのコンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
TitleLogo::TitleLogo(CommonResources* resources)
	: m_pCommonResources{ resources }// 共通リソース
	, m_pVertexShader{}// 頂点シェーダー
	, m_pPixelShader{}// ピクセルシェーダー
	, m_pInputLayout{}// インプットレイアウト
	, m_pDR{}// デバイスリソース
	, m_pCBuffer{}// コンスタントバッファ
	, m_titleTexture{}// テクスチャ
	, m_time{ 0.0f }// 時間
	, m_world{}// ワールド行列
	, m_view{}// ビュー行列
	, m_proj{}// プロジェクション行列
	, m_ConstBuffer{}// コンスタントバッファ（シェーダーに渡す方）
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // シェーダー作成クラスの初期化
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief デストラクタ
*	@details ここでは何もしない
*	@param なし
*	@return なし
*/
TitleLogo::~TitleLogo() {/*do nothing*/ }
/*
*	@brief テクスチャ読み込み
*	@details テクスチャを読み込む
*	@param path テクスチャのパス
*	@return なし
*/
void TitleLogo::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// 一時保存用テクスチャ
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャの読み込み
	m_titleTexture.push_back(texture);// 配列に登録
}
/*
*	@brief タイトルロゴの初期化
*	@details タイトルロゴの初期化を行う
*	@param pDR デバイスリソース
*	@return なし
*/
void TitleLogo::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;// デバイスリソースをセット
	CreateShaders();// シェーダーの作成
	LoadTexture(L"Resources/Textures/Title.png");// 画像の読み込み　
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// 板ポリゴンの描画用
}

void TitleLogo::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);// 頂点シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);// ピクセルシェーダーの作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダー(使わないのでnullptr)
}
/*
*	@brief 更新
*	@details タイトルロゴの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TitleLogo::Update(float elapsedTime)
{
	m_time += elapsedTime; // 時間をカウント
}
/*
*	@brief 描画
*	@details タイトルロゴの描画を行う
*	@param なし
*	@return なし
*/
void TitleLogo::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] = // 頂点情報とUV情報
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(-0.85f,  0.75f, 0.0f), Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(0.85f,  0.75f, 0.0f),  Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(0.85f, -0.75f, 0.0f),  Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(-0.85f, -0.75f, 0.0f), Vector2(0.0f, 1.0f)),
	};
	// シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	m_ConstBuffer.matView = m_view.Transpose();// ビュー設定
	m_ConstBuffer.matProj = m_proj.Transpose();// プロジェクション設定
	m_ConstBuffer.matWorld = m_world.Transpose();// ワールド設定
	m_ConstBuffer.Colors = Vector4(0.25f, 0.75f, 0.75f, 0);// グラデーションエフェクトの色設定 
	m_ConstBuffer.time = Vector4(m_time);// 時間設定
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);// 受け渡し用バッファの内容更新
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// ConstBufferからID3D11Bufferへの変換
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// シェーダーにバッファを渡す
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_titleTexture);// 描画
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->DrawTexture(vertex);// 板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダの登録を解除しておく
}