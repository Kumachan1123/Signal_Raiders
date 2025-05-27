/*
*	@file	Result.cpp
*	@brief	結果クラス
*/
#include "pch.h"
#include "Result.h"
/*
*	@brief インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Result::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief コンストラクタ
*	@details 結果クラスのコンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
Result::Result(CommonResources* resources)
	: m_pCommonResources{ resources }// 共通リソース
	, m_pVertexShader{}// 頂点シェーダー
	, m_pPixelShader{}// ピクセルシェーダー
	, m_pInputLayout{}// インプットレイアウト
	, m_pDR{}// デバイスリソース
	, m_pCBuffer{}// コンスタントバッファ
	, m_pTexture{}// テクスチャ
	, m_time{ 0.0f }// 時間
	, m_world{}// ワールド行列
	, m_view{}// ビュー行列
	, m_proj{}// プロジェクション行列
	, m_vertex{}// 頂点情報
	, m_ConstBuffer{}// コンスタントバッファ(シェーダーに渡す方)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // シェーダー作成クラスの初期化
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief デストラクタ
*	@details 結果クラスのデストラクタ
*	@param なし
*	@return なし
*/
Result::~Result() {/*do nothing */ }
/*
*	@brief テクスチャリソース読み込み
*	@details テクスチャリソースを読み込む
*	@param path 読み込むテクスチャのパス
*	@return なし
*/
void Result::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャハンドル
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャの読み込み
	m_pTexture.push_back(texture);// テクスチャを読み込む
}
/*
*	@brief 生成
*	@details 結果クラスで使う画像やシェーダーの生成・作成
*	@param pDR デバイスリソース
*	@return なし
*/
void Result::Create(DX::DeviceResources* pDR, const wchar_t* path)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pDR = pDR;// デバイスリソースを取得
	CreateShaders();// シェーダーの作成
	LoadTexture(path);// 画像の読み込み
	if (wcscmp(path, L"Resources/Textures/GameOver.png") == 0)// ゲームオーバー画像
	{
		m_ConstBuffer.Colors = Vector4(0.15f, 0, 0.5f, 0);// グラデーションエフェクトの色設定
		VertexPositionTexture vertex[4] =// 頂点情報とUV情報
		{
			//	頂点情報													UV情報
			VertexPositionTexture(Vector3(-0.85f,  0.4f, 0.0f), Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(0.85f,  0.4f, 0.0f),  Vector2(1.0f, 0.0f)),
			VertexPositionTexture(Vector3(0.85f, -0.4f, 0.0f),  Vector2(1.0f, 1.0f)),
			VertexPositionTexture(Vector3(-0.85f, -0.4f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];// 頂点情報を格納
	}
	else // クリア画像
	{
		m_ConstBuffer.Colors = Vector4(0.5f, 0.5f, 0, 0);// グラデーションエフェクトの色設定
		VertexPositionTexture vertex[4] =// 頂点情報とUV情報
		{
			//	頂点情報													UV情報
			VertexPositionTexture(Vector3(-0.8f,  0.5f, 0.0f), Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(0.8f,  0.5f, 0.0f),  Vector2(1.0f, 0.0f)),
			VertexPositionTexture(Vector3(0.8f, -0.5f, 0.0f),  Vector2(1.0f, 1.0f)),
			VertexPositionTexture(Vector3(-0.8f, -0.5f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];// 頂点情報を格納
	}
	for (int i = 0; i < 4; i++)m_vertex[i].position.y += 0.25f;// 頂点の座標を調整
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// 板ポリゴン描画用
}
/*
*	@brief シェーダー作成
*	@details Create関数から抜き出したシェーダーを作成する部分
*	@param なし
*	@return なし
*/
void Result::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);// 頂点シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);// ピクセルシェーダー作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダー(使わないのでnullptr)
}
/*
*	@brief 更新
*	@details 結果クラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void Result::Update(float elapsedTime)
{
	m_time += elapsedTime; // 時間をカウント
}
/*
*	@brief 描画
*	@details 結果クラスの描画
*	@param なし
*	@return なし
*/
void Result::Render()
{
	using namespace DirectX::SimpleMath;
	m_ConstBuffer.matView = m_view.Transpose();// ビュー行列
	m_ConstBuffer.matProj = m_proj.Transpose();// プロジェクション行列
	m_ConstBuffer.matWorld = m_world.Transpose();//	ワールド行列
	m_ConstBuffer.time = m_time;// 時間設定
	m_ConstBuffer.padding = Vector3(0, 0, 0);// パディング
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// シェーダをセットする
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);// 描画準備
	m_pDrawPolygon->DrawTexture(m_vertex);// 板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダの登録を解除しておく
}