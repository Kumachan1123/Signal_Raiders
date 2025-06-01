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
	m_pTexture = m_pCommonResources->GetTextureManager()->GetTexture(key);// テクスチャマネージャーからテクスチャを取得
	// サイズ取得のための準備
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;// 一時保存用リソースハンドル
	m_pTexture->GetResource(resource.GetAddressOf());// テクスチャからリソースを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;// 一時保存用テクスチャハンドル
	DX::ThrowIfFailed(resource.As(&texture2D));// リソースをテクスチャに変換
	// テクスチャの情報を取得
	D3D11_TEXTURE2D_DESC desc;// テクスチャの情報を格納する構造体
	texture2D->GetDesc(&desc);// テクスチャの情報を取得
	m_pTextures.push_back(m_pTexture.Get()); // テクスチャを配列に追加
	m_textureWidth = desc.Width; // テクスチャの幅を取得
	m_textureHeight = desc.Height; // テクスチャの高さを取得
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
	, std::string key
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor)
{
	m_pDR = pDR;// デバイスリソース
	m_position = position;// 位置
	m_baseScale = m_scale = scale;// スケール
	m_anchor = anchor;// アンカー
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], // シェーダー作成クラスの初期化
		static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	CreateShaders();// シェーダーの作成
	LoadTexture(key);// テクスチャを読み込む
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);	// 板ポリゴン描画用
}
/*
*	@brief	シェーダーの作成
*	@details 各種シェーダーを作成
*	@param なし
*	@return なし
*/
void PlayerUI::CreateShaders()
{
	// シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/PlayerUI/VS_PlayerUI.cso", m_pVertexShader);// 頂点シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerUI.cso", m_pPixelShader);// ピクセルシェーダーの作成
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/PlayerUI/GS_PlayerUI.cso", m_pGeometryShader);// ジオメトリシェーダーの作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトを受け取る
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerHP.cso", m_pHPPixelShader);// HP用シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_CircleGauge.cso", m_pCirclePixelShader);// 円形ゲージ用シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_Loading.cso", m_pAnimPixelShader);// UVアニメーション用シェーダーの作成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// HP用シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	// HP以外のシェーダー
	m_shaders.vs = m_pVertexShader.Get();			// 頂点シェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();			// ピクセルシェーダーをセット
	m_shaders.gs = m_pGeometryShader.Get();			// ジオメトリシェーダーをセット
	// HP用シェーダー
	m_hpShaders.vs = m_pVertexShader.Get();			// 頂点シェーダーをセット
	m_hpShaders.ps = m_pHPPixelShader.Get();			// ピクセルシェーダーをセット
	m_hpShaders.gs = m_pGeometryShader.Get();		// ジオメトリシェーダーをセット
	// 円形ゲージ用シェーダー
	m_circleShaders.vs = m_pVertexShader.Get();		// 頂点シェーダーをセット
	m_circleShaders.ps = m_pCirclePixelShader.Get();	// ピクセルシェーダーをセット
	m_circleShaders.gs = m_pGeometryShader.Get();	// ジオメトリシェーダーをセット
	// UVアニメーション用シェーダー
	m_animShaders.vs = m_pVertexShader.Get();		// 頂点シェーダーをセット
	m_animShaders.ps = m_pAnimPixelShader.Get();		// ピクセルシェーダーをセット
	m_animShaders.gs = m_pGeometryShader.Get();		// ジオメトリシェーダーをセット
}


/// <summary>
/// 描画関数
/// </summary>
void PlayerUI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionColorTexture vertex[1] = {// 頂点情報
		VertexPositionColorTexture(
			  Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))//	Position.xy	:拡縮用スケール	Position.z	:アンカータイプ(0～8)の整数で指定
			, Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))	//	Color.xy　	:アンカー座標(ピクセル指定:1280 ×720) Color.zw	:画像サイズ
			, Vector2(m_renderRatio - m_renderRatioOffset,0))//	Tex.xy		:x = ゲージ画像の描画範囲(0～1), y = 0
	};
	// シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);// ウィンドウサイズ
	m_constBuffer.renderRatio = m_renderRatio - m_renderRatioOffset;// レンダリング比率
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// 受け渡し用バッファの内容更新
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// ConstBufferからID3D11Bufferへの変換
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// シェーダーにバッファを渡す
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);// 描画開始
	//	シェーダをセットする
	if (m_shaderType == ShaderType::HP)m_pDrawPolygon->SetShader(m_hpShaders, nullptr, 0);// HP用のシェーダー
	else if (m_shaderType == ShaderType::CIRCLE)m_pDrawPolygon->SetShader(m_circleShaders, nullptr, 0);// 円形ゲージ用のシェーダー
	else if (m_shaderType == ShaderType::ANIM)m_pDrawPolygon->SetShader(m_animShaders, nullptr, 0);// UVアニメーション用のシェーダー
	else m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// 共通のシェーダー
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);//	板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();//	シェーダの登録を解除しておく
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
	m_windowWidth = width;// ウィンドウの幅
	m_windowHeight = height;// ウィンドウの高さ
}