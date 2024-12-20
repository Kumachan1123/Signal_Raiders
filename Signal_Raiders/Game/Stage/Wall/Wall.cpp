#include "pch.h"
#include "Wall.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC>  Wall::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

Wall::Wall(CommonResources* resources)
	: m_commonResources(resources)
	, m_time(0.0f)
	, m_constBuffer()
	, m_wall()
	, m_pDR(nullptr)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pWallTexture()
	, m_world(DirectX::SimpleMath::Matrix::Identity)
	, m_wallBox()
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);

}

Wall::~Wall() {  }

void  Wall::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_pWallTexture.push_back(texture);
}
void  Wall::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	m_pDR = pDR;
	//	シェーダーの作成
	CreateShader();

	// 壁の初期化
	Initialize();
	//	画像の読み込み（2枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/Wall.png");
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
void Wall::CreateShader()
{
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_vertexShader);
	// ピクセルシェーダ作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;

}
void Wall::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	頂点情報（壁の頂点） 
	VertexPositionTexture wall[4][4] =
	{
		{
			VertexPositionTexture(Vector3(100.0f,  10.0f,  100.0f),	Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(-100.0f, 10.0f,  100.0f),	Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(-100.0f,  0.0f,  100.0f), Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(100.0f,   0.0f,  100.0f), Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(Vector3(100.0f,  10.0f,  -100.0f),Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(-100.0f, 10.0f,  -100.0f),Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(-100.0f,  0.0f,  -100.0f),Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(100.0f,   0.0f,  -100.0f),Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(Vector3(100.0f,10.0f,  100.0f),Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(100.0f,10.0f ,-100.0f),Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(100.0f, 0.0f ,-100.0f),Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(100.0f, 0.0f,  100.0f),Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(Vector3(-100.0f,10.0f,  100.0f),Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(-100.0f,10.0f ,-100.0f),Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(-100.0f, 0.0f ,-100.0f),Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(-100.0f, 0.0f,  100.0f),Vector2(0.0f, 1.0f))
		}
	};
	// 壁の頂点をコピー
	for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++) m_wall[i][j] = wall[i][j];

	// 壁の中心座標を求める
	m_wallBox[0].Center = Vector3(0.0f, 5.0f, 100.0f);
	m_wallBox[0].Extents = Vector3(100.0f, 5.0f, 0.0f);
	m_wallBox[1].Center = Vector3(0.0f, 5.0f, -100.0f);
	m_wallBox[1].Extents = Vector3(100.0f, 5.0f, 0.0f);
	m_wallBox[2].Center = Vector3(100.0f, 5.0f, 0.0f);
	m_wallBox[2].Extents = Vector3(0.0f, 5.0f, 100.0f);
	m_wallBox[3].Center = Vector3(-100.0f, 5.0f, 0.0f);
	m_wallBox[3].Extents = Vector3(0.0f, 5.0f, 100.0f);



}
void Wall::Update(float elapsedTime)
{
	m_time += elapsedTime;
}

void Wall::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = view.Transpose();
	m_constBuffer.matProj = proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(0, 1.0f, 1.0f, 0.0f);
	m_constBuffer.time = DirectX::SimpleMath::Vector4(m_time);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// 描画
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pWallTexture);
	// 壁を描画
	for (int i = 0; i < 4; i++)	m_pDrawPolygon->DrawTexture(m_wall[i]);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

