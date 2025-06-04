/*
*	@file	Result.cpp
*	@brief	結果クラス
*/
#include <pch.h>
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
}
/*
*	@brief デストラクタ
*	@details 結果クラスのデストラクタ
*	@param なし
*	@return なし
*/
Result::~Result() {/*do nothing */ }
/*
*	@brief 生成
*	@details 結果クラスで使う画像やシェーダーの生成・作成
*	@param pDR デバイスリソース
*	@return なし
*/
void Result::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスリソースを取得
	m_pDR = pDR;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// プレイシーンでの結果に応じて変化
	if (GetSceneID() == IScene::SceneID::GAMEOVER)// ゲームオーバー
	{
		// ゲームオーバー画像の読み込み
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("GameOver"));
		// グラデーションエフェクトの色設定
		m_ConstBuffer.Colors = Vector4(0.15f, 0, 0.5f, 0);
		// 頂点情報とUV情報
		VertexPositionTexture vertex[4] =
		{
			//	頂点情報													UV情報
			VertexPositionTexture(Vector3(-0.85f,  0.4f, 0.0f), Vector2(0.0f, 0.0f)),// 左上
			VertexPositionTexture(Vector3(0.85f,  0.4f, 0.0f),  Vector2(1.0f, 0.0f)),// 右上
			VertexPositionTexture(Vector3(0.85f, -0.4f, 0.0f),  Vector2(1.0f, 1.0f)),// 右下
			VertexPositionTexture(Vector3(-0.85f, -0.4f, 0.0f), Vector2(0.0f, 1.0f)),// 左下
		};
		// 頂点情報を格納
		for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];
	}
	else // ゲームクリア
	{
		// クリア画像の読み込み
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("GameClear"));
		// グラデーションエフェクトの色設定
		m_ConstBuffer.Colors = Vector4(0.5f, 0.5f, 0, 0);
		// 頂点情報とUV情報
		VertexPositionTexture vertex[4] =
		{
			//	頂点情報													UV情報
			VertexPositionTexture(Vector3(-0.8f,  0.5f, 0.0f), Vector2(0.0f, 0.0f)),// 左上
			VertexPositionTexture(Vector3(0.8f,  0.5f, 0.0f),  Vector2(1.0f, 0.0f)),// 右上
			VertexPositionTexture(Vector3(0.8f, -0.5f, 0.0f),  Vector2(1.0f, 1.0f)),// 右下
			VertexPositionTexture(Vector3(-0.8f, -0.5f, 0.0f), Vector2(0.0f, 1.0f)),// 左下
		};
		// 頂点情報を格納
		for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];
	}
	// 頂点の座標を調整
	for (int i = 0; i < 4; i++)m_vertex[i].position.y += 0.25f;
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief シェーダー作成
*	@details Create関数から抜き出したシェーダーを作成する部分
*	@param なし
*	@return なし
*/
void Result::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);
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
}
/*
*	@brief 更新
*	@details 結果クラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void Result::Update(float elapsedTime)
{
	// 時間をカウント
	m_time += elapsedTime;
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
	// ビュー行列を転置して設定
	m_ConstBuffer.matView = m_view.Transpose();
	// プロジェクション行列を転置して設定
	m_ConstBuffer.matProj = m_proj.Transpose();
	// ワールド行列を転置して設定
	m_ConstBuffer.matWorld = m_world.Transpose();
	// 時間を設定
	m_ConstBuffer.time = m_time;
	// パディング
	m_ConstBuffer.padding = Vector3(0, 0, 0);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);
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
	m_pDrawPolygon->DrawTexture(m_vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}