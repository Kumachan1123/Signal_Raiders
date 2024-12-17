//--------------------------------------------------------------------------------------
// File: Crisis.cpp
//
// 危険状態クラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Crisis.h"

using namespace DirectX;

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Crisis::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

/// <summary>
/// コンストラクタ
/// </summary>
Crisis::Crisis(CommonResources* resources)
	:m_pDR(nullptr)
	, m_time(0.0f)
	, m_constBuffer()
{
	m_commonResources = resources;
	// 色の初期化
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

}

/// <summary>
/// デストラクタ
/// </summary>
Crisis::~Crisis()
{
	DrawPolygon::ReleasePositionTexture();
}

/// <summary>
/// テクスチャリソース読み込み関数
/// </summary>
/// <param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
void  Crisis::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}

/// <summary>
/// 生成関数
/// </summary>
/// <param name="pDR">ユーザーリソース等から持ってくる</param>
void  Crisis::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	// 板ポリゴン描画用
	DrawPolygon::InitializePositionTexture(m_pDR);

	//	シェーダーの作成
	CreateShader();

	//	画像の読み込み
	LoadTexture(L"Resources/Textures/crisis.png");



}

/// <summary>
/// Shader作成部分だけ分離した関数
/// </summary>
void  Crisis::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	コンパイルされたシェーダファイルを読み込み
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/CRT/VS_CRT.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/CRT/PS_CRT.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

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

	// シェーダーの構造体にシェーダーをセット
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;

}

//更新
void  Crisis::Update(float elapsedTime)
{
	//	時間更新（m_timeを0.1ずつ増やし、１を超えたら０からやり直し）
	m_time += elapsedTime;
	// 色の更新
	// alpha値が0.7f未満だったらまず0.9fまで少しずつ上げる
	if (m_constBuffer.colors.w < 0.7f)
	{
		m_constBuffer.colors.w += 0.15f * elapsedTime;
	}
	else
	{
		// alpha値を0.7fから0.9fまでを往復する
		m_constBuffer.colors.w = 0.8f + 0.1f * sin(m_time);
	}



	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, m_constBuffer.colors.w);

}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void  Crisis::Render()
{

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0, -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.time = DirectX::SimpleMath::Vector4(m_time);
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	DrawPolygon::UpdateSubResources(context, m_cBuffer.Get(), &m_constBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	// 描画準備
	DrawPolygon::DrawStart(context, m_inputLayout.Get(), m_texture);
	//	シェーダをセットする
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	DrawPolygon::DrawTexture(vertex);
	//	シェーダの登録を解除しておく
	DrawPolygon::ReleaseShader(context);
}