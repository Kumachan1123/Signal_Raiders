/*
*	@file	StageSelect.cpp
*	@brief	ステージ選択クラス
*/
#include <pch.h>
#include "StageSelect.h"
/*
*	@brief インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  StageSelect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
StageSelect::StageSelect(CommonResources* resources)
	: m_pCommonResources{ resources }// 共通リソース
	, m_pVertexShader{}// 頂点シェーダー
	, m_pPixelShader{}// ピクセルシェーダー
	, m_pInputLayout{}// 入力レイアウト
	, m_pDR{}// デバイスリソース
	, m_pCBuffer{}// コンスタントバッファ
	, m_pTexture{}// テクスチャ
	, m_time{ 0.0f }// 時間
	, m_world{}// ワールド行列
	, m_view{}// ビュー行列
	, m_proj{}// プロジェクション行列
	, m_vertex{}// 頂点情報
	, m_ConstBuffer{}// コンスタントバッファ
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
}
/*
*	@brief デストラクタ
*	@details ステージセレクトクラスのデストラクタ
*	@param なし
*	@return なし
*/
StageSelect::~StageSelect() {/*do nothing*/ }
/*
*	@brief テクスチャ読み込み
*	@details テクスチャを読み込む
*	@param path テクスチャのパス
*	@return なし
*/
void StageSelect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// 一時保存用テクスチャ
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャの読み込み
	m_pTexture.push_back(texture);// 配列に登録
}
/*
*	@brief 初期化
*	@details ステージセレクトクラスの初期化
*	@param pDR デバイスリソース
*	@return なし
*/
void StageSelect::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスリソースをセット
	m_pDR = pDR;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// テクスチャの取得
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("StageSelectText"));
	// グラデーションエフェクトの色設定
	m_ConstBuffer.color = SimpleMath::Vector4(.25, .25, .25, 0);
	// 頂点情報とUV情報
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(0.85f,0.45f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(SimpleMath::Vector3(-0.85f,0.45f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),// 左下
		VertexPositionTexture(SimpleMath::Vector3(-0.85f, 1.05f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(SimpleMath::Vector3(0.85f, 1.05f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),// 右上
	};
	// 頂点情報をセット
	for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief シェーダーを作成
*	@details シェーダーを作成する
*	@param なし
*	@return なし
*/
void StageSelect::CreateShaders()
{
	// 頂点シェーダー作成
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
	// ジオメトリシェーダーは使わないのでnullptrをセット
	m_shaders.gs = nullptr;
}
/*
*	@brief 更新
*	@details ステージセレクトクラスの更新
*	@param elapsedTime 更新時間
*	@return なし
*/
void StageSelect::Update(float elapsedTime)
{
	// 時間をカウント
	m_time += elapsedTime;
}
/*
*	@brief 描画
*	@details ステージセレクトクラスの描画
*	@param なし
*	@return なし
*/
void StageSelect::Render()
{
	using namespace DirectX::SimpleMath;
	// ビュー行列を転置してセット
	m_ConstBuffer.matView = m_view.Transpose();
	// プロジェクション行列を転置してセット
	m_ConstBuffer.matProj = m_proj.Transpose();
	// ワールド行列を転置してセット
	m_ConstBuffer.matWorld = m_world.Transpose();
	// 時間を設定
	m_ConstBuffer.time = Vector4(m_time);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);
	// コンスタントバッファをセット
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// シェーダーをセット
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);//深度ステンシルステート
	// テクスチャ描画開始
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(m_vertex);
	// シェーダーの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}