/*
*	@file Crisis.cpp
*	@brief	危険信号クラス（HPが20以下で表示される）
*/
#include <pch.h>
#include "Crisis.h"
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Crisis::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details Crisisのコンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
Crisis::Crisis(CommonResources* resources)
	: m_pDR(nullptr)// デバイスリソース
	, m_time(0.0f)// 経過時間
	, m_constBuffer()// コンスタントバッファ
	, m_pCommonResources{ resources }// 共通リソースへのポインタ
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	using namespace DirectX::SimpleMath;

	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(),
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);// シェーダー作成クラスの初期化
	m_constBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 0.0f);// 色の初期化
}
/*
*	@brief	デストラクタ
*	@details Crisisのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
Crisis::~Crisis() {/*do nothing*/ }
/*
*	@brief	テクスチャリソース読み込み
*	@details 指定されたパスからテクスチャを読み込み、m_pTextureに追加する
*	@param path 読み込むテクスチャのパス
*	@return なし
*/
void  Crisis::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// 一時保存用テクスチャハンドル
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// 指定された画像を読み込む
	m_pTexture.push_back(texture);// 配列に追加
}
/*
*	@brief	生成関数
*	@details DX::DeviceResourcesを受け取り、必要なリソースを初期化する
*	@param pDR デバイスリソース
*	@return なし
*/
void  Crisis::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;// デバイスリソースをセット
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// 板ポリゴン描画用
	CreateShaders();// シェーダーの作成
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Crisis"));
}
/*
*	@brief	シェーダー作成関数
*	@details 頂点シェーダーとピクセルシェーダーを作成し、インプットレイアウトを設定する
*	@param なし
*	@return なし
*/
void  Crisis::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/CRT/VS_CRT.cso", m_pVertexShader);// 頂点シェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/CRT/PS_CRT.cso", m_pPixelShader);// ピクセルシェーダーの作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダーをセット
	m_shaders.gs = nullptr;// ジオメトリシェーダーは使用しない
}
/*
*	@brief	更新関数
*	@details 時間を更新し、色のアルファ値を変化させる
*	@param elapsedTime 前フレームからの経過時間
*	@return なし
*/
void  Crisis::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_time += elapsedTime;// 時間更新 
	// 色の更新
	if (m_constBuffer.colors.w < 0.7f)// alpha値が0.7f未満だったらまず0.9fまで少しずつ上げる
		m_constBuffer.colors.w += 0.15f * elapsedTime;
	else// alpha値を0.7fから0.9fまでを往復する
		m_constBuffer.colors.w = 0.8f + 0.1f * sin(m_time);
	m_constBuffer.colors = Vector4(1.0f, 0.0f, 0.0f, m_constBuffer.colors.w);// 色の更新
}
/*
*	@brief	描画関数
*	@details 板ポリゴンを描画する
*	@param なし
*	@return なし
*/
void  Crisis::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =//	頂点情報 
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),  Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f),   Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),    Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f),   Vector2(0.0f, 1.0f)),// 左下
	};
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();// ビュー行列を転置
	m_constBuffer.matProj = m_proj.Transpose();// プロジェクション行列を転置
	m_constBuffer.matWorld = m_world.Transpose();// ワールド行列を転置
	m_constBuffer.time = Vector4(m_time);// 時間をVector4に変換(16バイト境界に合わせるためにVector4を使用)
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// 受け渡し用バッファの内容更新
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// ConstBufferからID3D11Bufferへの変換
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// シェーダーにバッファを渡す
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);// 描画準備
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// シェーダをセットする
	m_pDrawPolygon->DrawTexture(vertex);// 板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダの登録を解除しておく
}