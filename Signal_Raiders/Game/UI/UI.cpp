/*
*	@file	UI.cpp
*	@brief	UIクラス(タイトル・リザルト・セッティング)
*/
#include <pch.h>
#include "UI.h"

// UIクラスのインプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> UI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details UIのコンストラクタ
*	@param なし
*	@return なし
*/
UI::UI(CommonResources* pCommonResources)
	: m_pDR{ nullptr }// デバイスリソース
	, m_pCommonResources{ pCommonResources }// 共通リソースへのポインタ
	, m_time{ 0 }// 時間
	, m_windowWidth{ 0 }// ウィンドウの幅
	, m_windowHeight{ 0 }// ウィンドウの高さ
	, m_textureWidth{ 0 }// テクスチャの幅
	, m_textureHeight{ 0 }// テクスチャの高さ
	, m_scale{ DirectX::SimpleMath::Vector2::One }// スケール
	, m_baseScale{ DirectX::SimpleMath::Vector2::One }// 基準スケール
	, m_position{ DirectX::SimpleMath::Vector2::Zero }// 位置
	, m_anchor{ KumachiLib::ANCHOR::TOP_LEFT }// アンカー
	, m_shaderType{ ShaderType::NORMAL }// シェーダーの種類
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
UI::~UI() {/*do nothing*/ }
/*
*	@brief	テクスチャの読み込み
*	@details テクスチャの読み込み
*	@param key テクスチャのキー
*	@return なし
*/
void UI::LoadTexture(std::string key)
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
*	@brief	初期化
*	@details UIの初期化
*	@param pDR デバイスリソース
*	@param key テクスチャのキー
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void UI::Create(DX::DeviceResources* pDR, std::string key
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
*	@brief	当たり判定
*	@details マウスの座標がUIの範囲内にあるかを判定する
*	@param pos マウスの座標
*	@return true:当たり判定あり, false:当たり判定なし
*/
bool UI::IsHit(const DirectX::SimpleMath::Vector2& pos) const
{

	DirectX::SimpleMath::Vector2 leftTop = // 画像の左上の座標を取得
		m_position - DirectX::SimpleMath::Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.x / 2;
	DirectX::SimpleMath::Vector2 rightBottom =// 画像の右下の座標を取得
		m_position + DirectX::SimpleMath::Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.y / 2;

	if (leftTop.x <= pos.x && pos.x <= rightBottom.x	// 画像の左上の座標と右下の座標を比較して、
		&& leftTop.y <= pos.y && pos.y <= rightBottom.y)// 	マウスの座標が範囲内にあるかを判定する
		return true;// 当たり判定あり
	return false;// 当たり判定なし
}
/*
*	@brief	シェーダーの作成
*	@details 各種シェーダーを作成
*	@param なし
*	@return なし
*/
void UI::CreateShaders()
{
	// シェーダーを作成する
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Menu/VS_Menu.cso", m_pVertexShader);// 頂点シェーダー作成
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Menu/GS_Menu.cso", m_pGeometryShader);// ジオメトリシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_Menu.cso", m_pPixelShader);// ピクセルシェーダ作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_StageMenu.cso", m_pPixelShaderStageSelect);// ピクセルシェーダ作成(ステージセレクト用)
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.gs = m_pGeometryShader.Get();// ジオメトリシェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	// ステージセレクト用のシェーダーの構造体にシェーダーを渡す
	m_StageSelectShaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_StageSelectShaders.gs = m_pGeometryShader.Get();// ジオメトリシェーダー
	m_StageSelectShaders.ps = m_pPixelShaderStageSelect.Get();// ピクセルシェーダー
}
/*
*	@brief	更新
*	@details UIの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void UI::Update(float elapsedTime) { m_time += elapsedTime; }
/*
*	@brief	描画
*	@details UIの描画
*	@param なし
*	@return なし
*/
void UI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionColorTexture vertex[1] = {// 頂点情報
		VertexPositionColorTexture(Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))// 大きさとアンカー
		, Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))// 位置と幅と高さ
		, Vector2(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)))// ウィンドウの幅と高さ
	};
	//	シェーダーに渡す追加のバッファを作成する
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);// ウィンドウサイズ
	m_constBuffer.time = m_time;// 時間
	m_constBuffer.color = Vector3(0.5, 0.5, 0.5);// 色
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// 受け渡し用バッファの内容更新
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// ConstBufferからID3D11Bufferへの変換
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// シェーダーにバッファを渡す
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);// 描画開始
	if (m_shaderType == ShaderType::NORMAL)	// 普通のメニューだったら
		m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);			//	普通のシェーダをセットする
	else									// ステージセレクトだったら
		m_pDrawPolygon->SetShader(m_StageSelectShaders, nullptr, 0);// 	ステージセレクト用のシェーダをセットする
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);// 板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダの登録を解除しておく

}
/*
*	@brief	ウィンドウのサイズを設定
*	@details ウィンドウのサイズを設定
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void UI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;// ウィンドウの幅
	m_windowHeight = height;// ウィンドウの高さ
}
