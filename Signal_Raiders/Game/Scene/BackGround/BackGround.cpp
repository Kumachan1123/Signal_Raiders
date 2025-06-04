/*
*	@file BackGround.h
*	@brief 背景クラス
*/

#include <pch.h>
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


}
/*
*	@brief デストラクタ
*	@details 背景クラスのデストラクタ
*	@param なし
*	@return なし
*/
BackGround::~BackGround() {/* do nothing */ }
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
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("BackGround"));// テクスチャの取得
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
	// デバイスの取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(device, &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/CRT/VS_CRT.cso", m_pVertexShader);
	// ピクセルシェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/CRT/PS_CRT.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーを渡す
	m_shaders.ps = m_pPixelShader.Get();
	// ジオメトリシェーダーは使わないのでnullptrを設定
	m_shaders.gs = nullptr;
	// 色の初期化
	m_color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}
/*
*	@brief 更新
*	@details 背景クラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void  BackGround::Update(float elapsedTime)
{
	// 時間更新
	m_time += elapsedTime;
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
	// 頂点情報とUV情報を格納する配列
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f), Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),  Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),// 左下
	};
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	// ビュー行列を転置して格納する
	m_constBuffer.matView = m_view.Transpose();
	// プロジェクション行列を転置して格納する
	m_constBuffer.matProj = m_proj.Transpose();
	// ワールド行列を転置して格納する
	m_constBuffer.matWorld = m_world.Transpose();
	// 色を格納する
	m_constBuffer.colors = Vector4(m_color);
	// 時間を格納する
	m_constBuffer.time = Vector4(m_time);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);//	デプスステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}