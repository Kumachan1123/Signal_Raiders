/*
*	@file	Fade.cpp
*	@brief	フェードクラス
*/
#include <pch.h>
#include "Fade.h"
/*
*	@brief フェード最小値
*/
const float Fade::FADE_MIN = -1.01f;
/*
*	@brief フェード最大値
*/
const float Fade::FADE_MAX = 1.01f;
/*
*	@brief フェード速度
*/
const float Fade::FADE_SPEED = 1.5f;
/*
*	@brief フェード初期値
*/
const float Fade::FADE_INIT = -0.75f;
/*
*	@brief フェードの滑らかさ
*/
const float Fade::FADE_SMOOTHNESS = 0.05f;

/*
*	@brief インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief コンストラクタ
*	@param commonResources	共通リソース
*	@return	なし
*/
Fade::Fade(CommonResources* commonResources)
	: m_pDR{ }// デバイスリソース
	, m_pCBuffer{}// 定数バッファ
	, m_pInputLayout{}// 入力レイアウト
	, m_pCommonResources{ commonResources }// 共通リソース
	, m_pTexture{}// テクスチャ
	, m_pVertexShader{}// 頂点シェーダー
	, m_pPixelShader{}// ピクセルシェーダー
	, m_world{}// ワールド行列
	, m_view{}// ビュー行列
	, m_proj{}// プロジェクション行列
	, m_fadeTime{ FADE_INIT }// フェード時間
	, m_fadeState{ FadeState::None }// フェード状態
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
}
/*
*	@brief デストラクタ
*	@param なし
*	@return なし
*/
Fade::~Fade() {/*do nothing*/ }
/*
*	@brief 生成
*	@param なし
*	@return なし
*/
void Fade::Initialize()
{
	// デバイスの取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(device, &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// デバイスリソースの取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// シェーダーの作成
	CreateShaders();
	// テクスチャの取得
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Fade"));
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief シェーダーの作成
*	@param なし
*	@return なし
*/
void Fade::CreateShaders()
{
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Fade/VS_Fade.cso", m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Fade/PS_Fade.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーを渡す
	m_shaders.ps = m_pPixelShader.Get();
	// シェーダーの構造体にジオメトリシェーダーをnullptrにする(使わないので)
	m_shaders.gs = nullptr;
}
/*
*	@brief 更新
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::Update(float elapsedTime)
{
	// フェードイン
	FadeIn(elapsedTime);
	// フェードアウト
	FadeOut(elapsedTime);
}
/*
*	@brief フェードイン
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::FadeIn(float elapsedTime)
{
	// フェードイン中なら
	if (m_fadeState == FadeState::FadeIn)
	{
		// 時間を計算
		m_fadeTime += elapsedTime * FADE_SPEED;
		// フェードを制限
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// フェードが最大値になったらフェードイン終了
		if (m_fadeTime >= FADE_MAX)m_fadeState = FadeState::FadeInEnd;
	}
}
/*
*	@brief フェードアウト
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::FadeOut(float elapsedTime)
{
	// フェードアウト中なら
	if (m_fadeState == FadeState::FadeOut)
	{
		// 時間を計算
		m_fadeTime -= elapsedTime * FADE_SPEED;
		// フェードを制限
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// フェードが最小値になったらフェードアウト終了
		if (m_fadeTime <= FADE_MIN)m_fadeState = FadeState::FadeOutEnd;
	}
}
/*
*	@brief 描画
*	@param なし
*/
void Fade::Render()
{
	using namespace DirectX;
	//	頂点情報
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),	// 左上
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),		// 右上
		VertexPositionTexture(SimpleMath::Vector3(1.0, -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),		// 右下
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),		// 左下
	};
	// シェーダーに渡すバッファを作成
	ConstBuffer cbuff;
	// フェードの滑らかさを設定
	cbuff.smoothness = FADE_SMOOTHNESS;
	// フェードの進行度を設定
	cbuff.fadeAmount = m_fadeTime;
	// ConstBufferからID3D11Bufferへの変換
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &cbuff);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// 深度ステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}