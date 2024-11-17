#include "pch.h"

#include "Game/Stage/Wall/Wall.h"

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
{
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

	ID3D11Device1* device = pDR->GetD3DDevice();

	//	シェーダーの作成
	CreateShader();

	// 壁の初期化
	Initialize();
	//	画像の読み込み（2枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/Wall.png");

	//	プリミティブバッチの作成
	m_pPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	m_pPrimitiveBatchColor = std::make_unique<PrimitiveBatch<VertexPositionColor>>(pDR->GetD3DDeviceContext());
	m_pStates = std::make_unique<CommonStates>(device);
	// ベーシックエフェクトを作成する
	m_pBasicEffect = std::make_unique<BasicEffect>(device);
	m_pBasicEffect->SetVertexColorEnabled(true);

}
void Wall::CreateShader()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	ID3D11Device1* device = m_pDR->GetD3DDevice();
	//	コンパイルされたシェーダファイルを読み込み
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/VS_Title.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/PS_Title.cso");
	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_pInputLayout.GetAddressOf());

	//	頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);


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
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = view.Transpose();
	m_constBuffer.matProj = proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(0, 1.0f, 1.0f, 0.0f);
	m_constBuffer.time = m_time;
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &m_constBuffer, 0, 0);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	テクスチャを設定する
	context->PSSetShaderResources(0, 1, m_pWallTexture[0].GetAddressOf());
	context->IASetInputLayout(m_pInputLayout.Get());
	// 壁を描画
	m_pPrimitiveBatch->Begin();
	for (int i = 0; i < 4; i++)m_pPrimitiveBatch->DrawQuad(m_wall[i][0], m_wall[i][1], m_wall[i][2], m_wall[i][3]);

	m_pPrimitiveBatch->End();
	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

	// 描画する
	auto states = m_commonResources->GetCommonStates();

	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_pInputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_pBasicEffect->SetView(view);
	m_pBasicEffect->SetProjection(proj);
	m_pBasicEffect->Apply(context);
	//m_pPrimitiveBatchColor->Begin();
	//DX::Draw(m_pPrimitiveBatchColor.get(), m_wallBox[0], Colors::Black);
	//m_pPrimitiveBatchColor->End();
}

