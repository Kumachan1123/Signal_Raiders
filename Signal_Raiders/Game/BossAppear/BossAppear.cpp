/*
*	@file BossAppear.cpp
*	@brief ボス出現演出のクラスのソースファイル
*/
#include <pch.h>
#include "BossAppear.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC> BossAppear::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*  @brief コンストラクタ
*  @param なし
*  @return なし
*/
BossAppear::BossAppear()
	: m_world(Matrix::Identity)// ワールド行列
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
	, m_timer(0.0f)// タイマー
	, m_pDR{}// デバイスリソース
	, m_constantBuffer{}// コンスタントバッファ
	, m_pInputLayout{}// インプットレイアウト
	, m_pTextures{}// テクスチャ
	, m_pCommonResources{}// コモンリソース
	, m_seVolume(0.0f)// SE音量
	, m_isPlaying(false)// 再生フラグ
{
}

/*
*  @brief デストラクタ
*  @param なし
*  @return なし
*/
BossAppear::~BossAppear()
{
	Finalize();// 終了処理
}

/*
*  @brief 初期化関数
*  @param resources コモンリソース
*  @return なし
*/
void BossAppear::Initialize(CommonResources* resources)
{
	m_pCommonResources = resources;	// コモンリソースの取得
	m_pDR = m_pCommonResources->GetDeviceResources();	// デバイスリソースの取得
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// シェーダー作成クラスの初期化
	this->SettingShader();	// シェーダーの作成
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("BossAppear"));// テクスチャの取得
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// 板ポリゴン描画用

}

/*
*  @brief テクスチャの読み込み
*　@param path テクスチャのパス
*  @return なし
*/
void BossAppear::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャのポインタ（一時保存用）
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());// テクスチャの読み込み
	m_pTextures.push_back(texture);// テクスチャのポインタを格納
}

/*
*  @brief シェーダーの設定
*  @param なし
*  @return なし
*/
void BossAppear::SettingShader()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/BossAppear/VS_BossAppear.cso", m_pVertexShader);// 頂点シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppear.cso", m_pPixelShader);// ピクセルシェーダー作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトを取得
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppearBack.cso", m_pPixelShaderBack);// ピクセルシェーダー作成（背景用）
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));	// 定数バッファ作成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダーは使用しない
	// 背景用シェーダーの構造体にシェーダーを渡す
	m_shadersBack.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shadersBack.ps = m_pPixelShaderBack.Get();	// ピクセルシェーダー
	m_shadersBack.gs = nullptr;// ジオメトリシェーダーは使用しない
}

/*
*  @brief 更新関数
*  @param elapsedTime フレーム時間
*  @return なし
*/
void BossAppear::Update(float elapsedTime)
{
	m_timer += elapsedTime;// 時間加算
	m_pCommonResources->GetAudioManager()->Update();// オーディオマネージャーの更新
	if (!m_isPlaying)
	{
		m_isPlaying = !m_isPlaying;// 再生フラグを立てる
		m_pCommonResources->GetAudioManager()->PlaySound("BossAppear", m_seVolume);// ボス登場演出音を再生
	}

}

/*
*  @brief 描画関数
*  @param view ビュー行列
*  @param proj プロジェクション行列
*  @return なし
*/
void BossAppear::Render()
{
	DrawBack();// 背景描画
	DrawMain();// メインテクスチャ描画
}
/*
*  @brief 終了関数
*  @param なし
*  @return なし
*/
void BossAppear::Finalize()
{
	//　do nothing
}

/*
*  @brief 描画関数(メインテクスチャ)
*  @param なし
*  @return なし
*/
void BossAppear::DrawMain()
{
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  0.5f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  0.7f, 0.0f) , SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0, -0.5f, 0.0f)  , SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -0.7f, 0.0f) , SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// 2秒経過後に拡大を開始
	float expandStartTime = 2.0f;  // 拡大開始時間
	float expandDuration = 0.5f;    // 拡大にかける時間（0.5秒で完了）
	// 拡大率を計算
	float scaleFactor = 1.0f; // 初期値（拡大なし）
	// 拡大開始時間を過ぎたら拡大を開始
	if (m_timer > expandStartTime)
	{
		float t = (m_timer - expandStartTime) / expandDuration;// 拡大時間を計算
		t = std::min(t, 1.0f); // 1.0を超えないようにクランプ
		scaleFactor = Lerp(1.0f, 5.0f, t * 10); // 1.0 から 2.0 へスムーズに拡大
	}
	// 頂点の拡大
	vertex[0].position = SimpleMath::Vector3(-1.0f, 0.5f, 0.0f) * scaleFactor;
	vertex[1].position = SimpleMath::Vector3(1.0f, 0.7f, 0.0f) * scaleFactor;
	vertex[2].position = SimpleMath::Vector3(1.0f, -0.5f, 0.0f) * scaleFactor;
	vertex[3].position = SimpleMath::Vector3(-1.0f, -0.7f, 0.0f) * scaleFactor;
	// コンスタントバッファの設定
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);// 色
	m_constantBuffer.time = Vector4(m_timer); // 時間
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);// 深度ステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// 描画終了
	m_pDrawPolygon->ReleaseShader();
}

/*
*  @brief 描画関数(背景)
*  @param なし
*  @return なし
*/
void BossAppear::DrawBack()
{
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0f, 1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// コンスタントバッファの設定
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);// 色
	m_constantBuffer.time = Vector4(m_timer);// 時間
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);// 深度ステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shadersBack, nullptr, 0);
	//	板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// 描画終了
	m_pDrawPolygon->ReleaseShader();
}