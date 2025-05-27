/*
*	@file Wall.cpp
*	@brief 壁クラス
*/
#include <pch.h>
#include "Wall.h"

// 壁の幅
const float Wall::WALL_WIDTH = 100.0f;
// 壁の高さ
const float Wall::WALL_HEIGHT = 5.0f;

// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Wall::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details 壁のコンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
Wall::Wall(CommonResources* resources)
	: m_pCommonResources(resources)// 共通リソース
	, m_time(0.0f)// 時間
	, m_constBuffer()// シェーダーに渡す情報
	, m_wall()// 壁の頂点
	, m_pDR(nullptr)// デバイスリソース
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 描画クラス
	, m_pWallTexture()// 壁テクスチャ
	, m_world(DirectX::SimpleMath::Matrix::Identity)// ワールド行列
	, m_wallBox()// 壁の当たり判定
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // シェーダー作成クラスの初期化
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief	デストラクタ
*	@details 何もしない
*	@param なし
*	@return なし
*/
Wall::~Wall() {/*do nothing*/ }
/*
*	@brief	テクスチャの読み込み
*	@details テクスチャの読み込み
*	@param path テクスチャのパス
*	@return なし
*/
void  Wall::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャ一時保存用変数
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャの読み込み
	m_pWallTexture.push_back(texture);// テクスチャを保存
}
/*
*	@brief	初期化
*	@details 壁の初期化
*	@param pDR デバイスリソース
*	@return なし
*/
void  Wall::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	m_pDR = pDR;// デバイスリソースを保存
	CreateShaders();// シェーダーの作成
	CreateWalls();// 壁の初期化
	LoadTexture(L"Resources/Textures/Wall.png");// 画像の読み込み 
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// 板ポリゴン描画用
}
void Wall::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);// 頂点シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);// ピクセルシェーダ作成
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));	// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダー（使わないのでnullptr）

}
void Wall::CreateWalls()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	頂点情報（壁の頂点） 
	VertexPositionTexture wall[4][4] =
	{	// プレイヤーの初期位置から見て
		{// 後ろの壁
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,  Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),	Vector2(0.0f, 0.0f)),// 左上
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),	Vector2(1.0f,0.0f)),// 右上
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,  0.0f,  Wall::WALL_WIDTH), Vector2(1.0f,1.0f)),// 右下
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,   0.0f,  Wall::WALL_WIDTH), Vector2(0.0f, 1.0f))// 左下
		},
		{// 前の壁
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,  Wall::WALL_HEIGHT * 2,  -Wall::WALL_WIDTH),Vector2(0.0f, 0.0f)),// 左上
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, Wall::WALL_HEIGHT * 2,  -Wall::WALL_WIDTH),Vector2(1.0f,0.0f)),// 右上
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,  0.0f,  -Wall::WALL_WIDTH),Vector2(1.0f,1.0f)),// 右下
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,   0.0f,  -Wall::WALL_WIDTH),Vector2(0.0f, 1.0f))// 左下
		},
		{// 右の壁
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),Vector2(0.0f, 0.0f)),// 左上
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2 ,-Wall::WALL_WIDTH),Vector2(1.0f,0.0f)),// 右上
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH, 0.0f ,-Wall::WALL_WIDTH),Vector2(1.0f,1.0f)),// 右下
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH, 0.0f,  Wall::WALL_WIDTH),Vector2(0.0f, 1.0f))// 左下
		},
		{// 左の壁
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),Vector2(0.0f, 0.0f)),// 左上
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2 ,-Wall::WALL_WIDTH),Vector2(1.0f,0.0f)),// 右上
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, 0.0f ,-Wall::WALL_WIDTH),Vector2(1.0f,1.0f)),// 右下
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, 0.0f,  Wall::WALL_WIDTH),Vector2(0.0f, 1.0f))// 左下
		}
	};

	for (int i = 0; i < WALL_NUM; i++)for (int j = 0; j < WALL_NUM; j++) m_wall[i][j] = wall[i][j];// 壁の頂点をコピー

	// 壁の中心座標を求める
	m_wallBox[0].Center = Vector3(0.0f, Wall::WALL_HEIGHT, Wall::WALL_WIDTH);	// 後ろの壁の中心座標
	m_wallBox[0].Extents = Vector3(Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);	// 後ろの壁の大きさ
	m_wallBox[1].Center = Vector3(0.0f, Wall::WALL_HEIGHT, -Wall::WALL_WIDTH);	// 前の壁の中心座標
	m_wallBox[1].Extents = Vector3(Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);	// 前の壁の大きさ
	m_wallBox[2].Center = Vector3(Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);	// 右の壁の中心座標
	m_wallBox[2].Extents = Vector3(0.0f, Wall::WALL_HEIGHT, Wall::WALL_WIDTH);	// 右の壁の大きさ
	m_wallBox[3].Center = Vector3(-Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);	// 左の壁の中心座標
	m_wallBox[3].Extents = Vector3(0.0f, Wall::WALL_HEIGHT, Wall::WALL_WIDTH);	// 左の壁の大きさ
}
/*
*	@brief	更新
*	@details 壁の更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void Wall::Update(float elapsedTime) { m_time += elapsedTime; }
/*
*	@brief	描画
*	@details 壁の描画
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void Wall::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = view.Transpose();// ビュー行列
	m_constBuffer.matProj = proj.Transpose();// プロジェクション行列
	m_constBuffer.matWorld = m_world.Transpose();// ワールド行列
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(0, 1.0f, 1.0f, 0.0f);// 色（水色）
	m_constBuffer.time = DirectX::SimpleMath::Vector4(m_time);// 時間
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// シェーダをセットする
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pWallTexture);// 描画
	for (int i = 0; i < WALL_NUM; i++)	m_pDrawPolygon->DrawTexture(m_wall[i]);	// 壁を描画
	m_pDrawPolygon->ReleaseShader();	// シェーダの登録を解除しておく
}