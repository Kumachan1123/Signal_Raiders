//--------------------------------------------------------------------------------------
//@file    PlayerUI.cpp
//@brief  プレイヤーUIクラス
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "PlayerUI.h"

using namespace DirectX;

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/// <summary>
/// コンストラクタ
/// </summary>
PlayerUI::PlayerUI()
	:m_pDR(nullptr)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_texture(nullptr)
	, m_res(nullptr)
	, m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_anchor(kumachi::ANCHOR::TOP_LEFT)
	, m_renderRatio(1.0f)
	, m_renderRatioOffset(0.0f)
{

}
PlayerUI::~PlayerUI() { DrawPolygon::ReleasePositionColorTexture(); }
/// <summary>
/// テクスチャリソース読み込み関数
/// </summary>
/// <param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
void PlayerUI::LoadTexture(const wchar_t* path)
{


	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, m_res.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	m_textures.push_back(m_texture);
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;

}

/// <summary>
/// 生成関数
/// </summary>
/// <param name="pDR">ユーザーリソース等から持ってくる</param>
void PlayerUI::Create(DX::DeviceResources* pDR
	, const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, kumachi::ANCHOR anchor)
{
	m_pDR = pDR;
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;

	//	シェーダーの作成
	CreateShader();

	//	画像の読み込み
	LoadTexture(path);
	// 板ポリゴン描画用
	DrawPolygon::InitializePositionColorTexture(m_pDR);

}


/// <summary>
/// Shader作成部分だけ分離した関数
/// </summary>
void PlayerUI::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	//	コンパイルされたシェーダファイルを読み込み
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/PlayerHP/VS_PlayerHP.cso");
	kumachi::BinaryFile GSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/PlayerHP/GS_PlayerHP.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/PlayerHP/PS_PlayerHP.cso");

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

	//	ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
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
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}


/// <summary>
/// 描画関数
/// </summary>
void PlayerUI::Render()
{
	auto context = m_pDR->GetD3DDeviceContext();
	//	頂点情報
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(
			 SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))//	Position.xy	:拡縮用スケール	Position.z	:アンカータイプ(0～8)の整数で指定
			,SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))	//	Color.xy　	:アンカー座標(ピクセル指定:1280 ×720) Color.zw	:画像サイズ
			,SimpleMath::Vector2(m_renderRatio - m_renderRatioOffset,0))//	Tex.xy		:x = ゲージ画像の描画範囲(0～1), y = 0
	};

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	m_constBuffer.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	m_constBuffer.renderRatio = m_renderRatio - m_renderRatioOffset;
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	DrawPolygon::UpdateSubResources(context, m_CBuffer.Get(), &m_constBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// 描画準備
	DrawPolygon::DrawStartColorTexture(context, m_inputLayout.Get(), m_textures);

	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	板ポリゴンを描画
	DrawPolygon::DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

void PlayerUI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
