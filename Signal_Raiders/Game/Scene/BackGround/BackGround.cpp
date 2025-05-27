/*
*	@file BackGround.h
*	@brief 背景クラス
*/

#include "pch.h"
#include "BackGround.h"


/*
*	@brief インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  BackGround::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};
/*
*	@brief コンストラクタ
*	@details 背景クラスのコンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
BackGround::BackGround(CommonResources* resources)
	: m_pDR(nullptr)// デバイスリソース
	, m_time(0.0f)// 時間
	, m_constBuffer()// コンスタントバッファ
	, m_pCommonResources{ resources }// 共通リソース
	, m_pDrawPolygon(DrawPolygon::GetInstance())// 描画クラス
	, m_pCreateShader(CreateShader::GetInstance())// シェーダー作成クラス
{

	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // シェーダー作成クラスの初期化
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	m_color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);// 色の初期化
}
/*
*	@brief デストラクタ
*	@details 背景クラスのデストラクタ
*	@param なし
*	@return なし
*/
BackGround::~BackGround() {/* do nothing */ }
/*
*	@brief テクスチャリソース読み込み
*	@details テクスチャリソースを読み込む
*	@param path 読み込むテクスチャのパス
*	@return なし
*/
void  BackGround::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャハンドル
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャの読み込み
	m_pTexture.push_back(texture);// テクスチャを読み込む
}
/*
*	@brief 生成
*	@details 背景クラスで使う画像やシェーダーの生成・作成
*	@param pDR デバイスリソース
*	@return なし
*/
void  BackGround::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;// デバイスリソースの取得
	CreateShaders();// シェーダーの作成
	LoadTexture(L"Resources/Textures/Back.png");// 画像の読み込み　
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// 板ポリゴン描画用
}
/*
*	@brief シェーダー作成
*	@details Create関数から抜き出したシェーダーを作成する部分
*	@param なし
*	@return なし
*/
void  BackGround::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/CRT/VS_CRT.cso", m_pVertexShader);// 頂点シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/CRT/PS_CRT.cso", m_pPixelShader);// ピクセルシェーダーの作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダー(使わないのでnullptr)
}
/*
*	@brief 更新
*	@details 背景クラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void  BackGround::Update(float elapsedTime)
{
	m_time += elapsedTime;// 時間更新
}
/*
*	@brief 描画
*	@details 背景クラスの描画
*	@param なし
*	@return なし
*/
void  BackGround::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =// 頂点情報とUV情報を格納する配列
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),  Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),
	};
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();// ビュー行列
	m_constBuffer.matProj = m_proj.Transpose();// プロジェクション行列
	m_constBuffer.matWorld = m_world.Transpose();// ワールド行列
	m_constBuffer.colors = Vector4(m_color);// 色
	m_constBuffer.time = Vector4(m_time);// 時間
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->DrawSetting(	// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);//	デプスステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);// 描画準備
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// シェーダをセットする
	m_pDrawPolygon->DrawTexture(vertex);// 板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダの登録を解除しておく
}