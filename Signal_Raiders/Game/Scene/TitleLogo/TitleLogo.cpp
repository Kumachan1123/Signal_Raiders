/*
*	@file	TitleLogo.cpp
*	@brief	タイトルロゴクラス
*/
#include <pch.h>
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
	, m_pTitleTexture{}// テクスチャ
	, m_time{ 0.0f }// 時間
	, m_world{}// ワールド行列
	, m_view{}// ビュー行列
	, m_proj{}// プロジェクション行列
	, m_ConstBuffer{}// コンスタントバッファ（シェーダーに渡す方）
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
}
/*
*	@brief デストラクタ
*	@details ここでは何もしない
*	@param なし
*	@return なし
*/
TitleLogo::~TitleLogo() {/*do nothing*/ }
/*
*	@brief タイトルロゴの初期化
*	@details タイトルロゴの初期化を行う
*	@param pDR デバイスリソース
*	@return なし
*/
void TitleLogo::Create(DX::DeviceResources* pDR)
{
	// デバイスリソースをセット
	m_pDR = pDR;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// タイトルロゴのテクスチャを取得
	m_pTitleTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Title"));
	// 板ポリゴンの描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}

void TitleLogo::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);
	// ピクセルシェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーを渡す
	m_shaders.ps = m_pPixelShader.Get();
	// ジオメトリシェーダーは使わないのでnullptrをセット
	m_shaders.gs = nullptr;
}
/*
*	@brief 更新
*	@details タイトルロゴの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TitleLogo::Update(float elapsedTime)
{
	// 時間をカウント
	m_time += elapsedTime;
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
	// 頂点情報とUV情報
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(-0.85f,  0.75f, 0.0f), Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(0.85f,  0.75f, 0.0f),  Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(0.85f, -0.75f, 0.0f),  Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(-0.85f, -0.75f, 0.0f), Vector2(0.0f, 1.0f)),// 左下
	};
	// シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// ビュー行列を転置してセット
	m_ConstBuffer.matView = m_view.Transpose();
	// プロジェクション行列を転置してセット
	m_ConstBuffer.matProj = m_proj.Transpose();
	// ワールド行列を転置してセット
	m_ConstBuffer.matWorld = m_world.Transpose();
	// 色の設定
	m_ConstBuffer.Colors = Vector4(0.25f, 0.75f, 0.75f, 0);
	// 時間の設定
	m_ConstBuffer.time = Vector4(m_time);
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);
	// ConstBufferからID3D11Bufferへの変換
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	// 描画開始
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTitleTexture);
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}