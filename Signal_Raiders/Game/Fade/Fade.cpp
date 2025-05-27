/*
*	@file	Fade.cpp
*	@brief	フェードクラス
*/
#include "pch.h"
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
	, m_time{ FADE_INIT }// 時間
	, m_fadeState{ FadeState::None }// フェード状態
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(),
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// シェーダー作成クラスの初期化
}
/*
*	@brief デストラクタ
*	@param なし
*	@return なし
*/
Fade::~Fade() {/*do nothing*/ }
/*
*	@brief テクスチャの読み込み
*	@param path	テクスチャのパス
*	@return なし
*/
void Fade::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// 一時保存用変数
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャの読み込み
	m_pTexture.push_back(texture);// 配列に追加
}
/*
*	@brief 生成
*	@param なし
*	@return なし
*/
void Fade::Initialize()
{
	m_pDR = m_pCommonResources->GetDeviceResources();	// デバイスリソースの取得
	CreateShaders();// シェーダーの作成
	LoadTexture(L"Resources/Textures/fade.png");	// 画像の読み込み
	m_pDrawPolygon->InitializePositionTexture(m_pDR);	// 板ポリゴン描画用
}
/*
*	@brief シェーダーの作成
*	@param なし
*	@return なし
*/
void Fade::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Fade/VS_Fade.cso", m_pVertexShader);// 頂点シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Fade/PS_Fade.cso", m_pPixelShader);// ピクセルシェーダー作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));	// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダー(使わないのでnullptr)
}

/*
*	@brief 更新
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::Update(float elapsedTime)
{
	FadeIn(elapsedTime);// フェードイン

	FadeOut(elapsedTime);// フェードアウト

}
/*
*	@brief フェードイン
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::FadeIn(float elapsedTime)
{
	if (m_fadeState == FadeState::FadeIn)// フェードイン中なら
	{
		m_time += elapsedTime * FADE_SPEED;// 時間を減算
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);// フェードを制限
		if (m_time >= FADE_MAX)// フェードが最大値になったら
		{
			m_fadeState = FadeState::FadeInEnd;// フェードイン終了
		}
	}
}
/*
*	@brief フェードアウト
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::FadeOut(float elapsedTime)
{
	if (m_fadeState == FadeState::FadeOut)// フェードアウト中なら
	{
		m_time -= elapsedTime * FADE_SPEED;	// 時間を加算
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);	// フェードを制限
		if (m_time <= FADE_MIN)// フェードが最小値になったら
		{
			m_fadeState = FadeState::FadeOutEnd;// フェードアウト終了
		}
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
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0, -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	ConstBuffer cbuff;	// シェーダーに渡すバッファを作成
	cbuff.smoothness = FADE_SMOOTHNESS;// フェードの滑らかさ
	cbuff.fadeAmount = m_time;	// フェードの進行度
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &cbuff);	// ConstBufferからID3D11Bufferへの変換
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };	//	シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->DrawSetting(	// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);	//	描画準備
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);	//	シェーダをセットする
	m_pDrawPolygon->DrawTexture(vertex);	//	板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();	//	シェーダの登録を解除しておく

}