/*
*	@file PlayerUI.cpp
*	@brief プレイヤーUIクラス
*/
#include <pch.h>
#include "PlayerUI.h"
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details PlayerUIのコンストラクタ
*	@param なし
*	@return なし
*/
PlayerUI::PlayerUI(CommonResources* pCommonResources)
	: m_pDR{ nullptr }// デバイスリソース
	, m_pCommonResources{ pCommonResources }// 共通リソースへのポインタ
	, m_windowWidth{ 0 }// ウィンドウの幅
	, m_windowHeight{ 0 }// ウィンドウの高さ
	, m_textureWidth{ 0 }// テクスチャの幅
	, m_textureHeight{ 0 }// テクスチャの高さ
	, m_frameRows(0)// フレームの行数
	, m_frameCols(0)// フレームの列数
	, m_anim(0)// フレーム数
	, m_pTexture(nullptr)// テクスチャハンドル
	, m_pTextureResource(nullptr)// テクスチャリソースハンドル
	, m_scale{ DirectX::SimpleMath::Vector2::One }// スケール
	, m_baseScale{ DirectX::SimpleMath::Vector2::One }// 基準スケール
	, m_position{ DirectX::SimpleMath::Vector2::Zero }// 位置
	, m_anchor(KumachiLib::ANCHOR::TOP_LEFT)// アンカー
	, m_shaderType(ShaderType::OTHER)// シェーダーの種類
	, m_renderRatio(1.0f)// レンダリング比率
	, m_renderRatioOffset(0.0f)// レンダリング比率のオフセット
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
}
/*
*	@brief	デストラクタ
*	@details 何もしない
*	@param なし
*	@return なし
*/
PlayerUI::~PlayerUI() {/*do nothing*/ }

/*
*	@brief	テクスチャの読み込み
*	@details テクスチャの読み込み
*	@param key テクスチャのキー
*	@return なし
*/
void PlayerUI::LoadTexture(std::string key)
{
	// テクスチャマネージャーからテクスチャを取得
	m_pTexture = m_pCommonResources->GetTextureManager()->GetTexture(key);
	// サイズ取得のための準備
	// 一時保存用リソースハンドル
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	// テクスチャからリソースを取得
	m_pTexture->GetResource(resource.GetAddressOf());
	// 一時保存用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	// リソースをテクスチャに変換
	DX::ThrowIfFailed(resource.As(&texture2D));
	// テクスチャの情報を取得
	// テクスチャの情報を格納する構造体
	D3D11_TEXTURE2D_DESC desc;
	// テクスチャの情報を取得
	texture2D->GetDesc(&desc);
	// テクスチャを配列に追加
	m_pTextures.push_back(m_pTexture.Get());
	// テクスチャの幅を取得
	m_textureWidth = desc.Width;
	// テクスチャの高さを取得
	m_textureHeight = desc.Height;
}

/*
*	@brief	UIの生成
*	@details UIの生成
*	@param pDR デバイスリソース
*	@param key テクスチャのキー
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void PlayerUI::Create(DX::DeviceResources* pDR
	, const std::string& key
	, const DirectX::SimpleMath::Vector2& position
	, const DirectX::SimpleMath::Vector2& scale
	, KumachiLib::ANCHOR anchor)
{
	// デバイスリソース
	m_pDR = pDR;
	// 位置
	m_position = position;
	// スケール
	m_baseScale = m_scale = scale;
	// アンカー
	m_anchor = anchor;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// テクスチャを読み込む
	LoadTexture(key);
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief	シェーダーの作成
*	@details 各種シェーダーを作成
*	@param なし
*	@return なし
*/
void PlayerUI::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/PlayerUI/VS_PlayerUI.cso", m_pVertexShader);
	// ピクセルシェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerUI.cso", m_pPixelShader);
	// ジオメトリシェーダーの作成
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/PlayerUI/GS_PlayerUI.cso", m_pGeometryShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// HP用シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerHP.cso", m_pHPPixelShader);
	// 円形ゲージ用シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_CircleGauge.cso", m_pCirclePixelShader);
	// UVアニメーション用シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_Loading.cso", m_pAnimPixelShader);
	// HP用シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	// HP以外のシェーダー
	// 頂点シェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();
	// ピクセルシェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();
	// ジオメトリシェーダーをセット
	m_shaders.gs = m_pGeometryShader.Get();
	// HP用シェーダー
	// 頂点シェーダーをセット
	m_hpShaders.vs = m_pVertexShader.Get();
	// ピクセルシェーダーをセット
	m_hpShaders.ps = m_pHPPixelShader.Get();
	// ジオメトリシェーダーをセット
	m_hpShaders.gs = m_pGeometryShader.Get();
	// 円形ゲージ用シェーダー
	// 頂点シェーダーをセット
	m_circleShaders.vs = m_pVertexShader.Get();
	// ピクセルシェーダーをセット
	m_circleShaders.ps = m_pCirclePixelShader.Get();
	// ジオメトリシェーダーをセット
	m_circleShaders.gs = m_pGeometryShader.Get();
	// UVアニメーション用シェーダー
	// 頂点シェーダーをセット
	m_animShaders.vs = m_pVertexShader.Get();
	// ピクセルシェーダーをセット
	m_animShaders.ps = m_pAnimPixelShader.Get();
	// ジオメトリシェーダーをセット
	m_animShaders.gs = m_pGeometryShader.Get();
}
/*
*	@brief	描画関数
*	@details 板ポリゴンを描画する
*	@param なし
*	@return なし
*/
void PlayerUI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 頂点情報
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(
			  Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))//	Position.xy	:拡縮用スケール	Position.z	:アンカータイプ(0～8)の整数で指定
			, Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))	//	Color.xy　	:アンカー座標(ピクセル指定:1280 ×720) Color.zw	:画像サイズ
			, Vector2(m_renderRatio - m_renderRatioOffset,0))//	Tex.xy		:x = ゲージ画像の描画範囲(0～1), y = 0
	};
	// シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// ウィンドウサイズ
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	// レンダリング比率
	m_constBuffer.renderRatio = m_renderRatio - m_renderRatioOffset;
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
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
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	//	シェーダをセットする
	if (m_shaderType == ShaderType::HP)// HP用のシェーダーの場合
	{
		// HP用のシェーダーをセット
		m_pDrawPolygon->SetShader(m_hpShaders, nullptr, 0);
	}
	else if (m_shaderType == ShaderType::CIRCLE)// 円形ゲージ用のシェーダーの場合 
	{
		// 円形ゲージ用のシェーダーをセット
		m_pDrawPolygon->SetShader(m_circleShaders, nullptr, 0);
	}
	else if (m_shaderType == ShaderType::ANIM)// UVアニメーション用のシェーダーの場合
	{
		// UVアニメーション用のシェーダーをセット
		m_pDrawPolygon->SetShader(m_animShaders, nullptr, 0);
	}
	else// その他のシェーダーの場合
	{
		// 共通のシェーダーをセット
		m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	}
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief	ウィンドウのサイズを設定
*	@details ウィンドウのサイズを設定
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void PlayerUI::SetWindowSize(const int& width, const int& height)
{
	// ウィンドウの幅
	m_windowWidth = width;
	// ウィンドウの高さ
	m_windowHeight = height;
}