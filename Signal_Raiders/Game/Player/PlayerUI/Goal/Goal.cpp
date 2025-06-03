/*
*	@file	Goal.cpp
*	@brief	目標を表示するクラス
*/
#include <pch.h>
#include "Goal.h"
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Goal::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details Goalのコンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
Goal::Goal(CommonResources* resources)
	:m_pDR(nullptr)// デバイスリソース
	, m_time(0.0f) // 経過時間
	, m_constBuffer() // コンスタントバッファ
	, m_pDrawPolygon{ DrawPolygon::GetInstance() } // 板ポリゴン描画クラス
	, m_pCommonResources{ resources } // 共通リソースへのポインタ
	, m_pCreateShader{ CreateShader::GetInstance() } // シェーダー作成クラス
{

	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);// 色の初期化
}
/*
*	@brief	デストラクタ
*	@details Goalのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
Goal::~Goal() {/*do nothing*/ }
/*
*	@brief	生成関数
*	@details DX::DeviceResourcesを受け取り、必要なリソースを初期化する
*	@param pDR デバイスリソース
*	@return なし
*/
void  Goal::Create(DX::DeviceResources* pDR)
{
	// デバイスリソースをセット
	m_pDR = pDR;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// 各画像の読み込み
	// 背景テクスチャ
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Crisis"));
	// 「すべての敵を倒せ！」のテクスチャ
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("KillAll"));
	// 板ポリゴン描画クラスの初期化
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief	シェーダー作成関数
*	@details 頂点シェーダーとピクセルシェーダーを作成し、インプットレイアウトを設定する
*	@param なし
*	@return なし
*/
void  Goal::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/ReadyGo/VS_ReadyGo.cso", m_pVertexShader);
	// ピクセルシェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/ReadyGo/PS_ReadyGo.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();
	// ジオメトリシェーダーは使用しないのでnullptrをセット
	m_shaders.gs = nullptr;
}
/*
*	@brief	更新関数
*	@details 時間を更新し、色のアルファ値を変化させる
*	@param elapsedTime 前フレームからの経過時間
*	@return なし
*/
void  Goal::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 時間更新 
	m_time += elapsedTime;
	// alpha値が0.7f未満だったらまず少しずつ上げる
	// 0.7f以上だったら、0.9fまで往復する
	if (m_constBuffer.colors.w < 0.7f)m_constBuffer.colors.w += 0.15f * elapsedTime;
	else m_constBuffer.colors.w = 0.8f + 0.1f * sin(m_time);
	// 色の更新
	m_constBuffer.colors = Vector4(1.0f, 1.0f, 0.0f, m_constBuffer.colors.w);
}
/*
*	@brief	描画関数
*	@details 板ポリゴンを描画する
*	@param なし
*	@return なし
*/
void  Goal::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),  Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f),   Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),    Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f),   Vector2(0.0f, 1.0f)), // 左下
	};
	// シェーダーに渡す追加のバッファを作成する 
	// ビュー行列を転置して渡す
	m_constBuffer.matView = m_view.Transpose();
	// プロジェクション行列を転置して渡す
	m_constBuffer.matProj = m_proj.Transpose();
	// ワールド行列を転置して渡す
	m_constBuffer.matWorld = m_world.Transpose();
	// 時間を渡す
	m_constBuffer.time = SimpleMath::Vector4(m_time);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}