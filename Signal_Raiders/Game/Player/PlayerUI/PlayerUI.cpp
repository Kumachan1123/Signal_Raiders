//--------------------------------------------------------------------------------------
//@file    PlayerUI.cpp
//@brief  プレイヤーUIクラス
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "PlayerUI.h"

using namespace DirectX;

// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
// コンストラクタ
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
	, m_anchor(KumachiLib::ANCHOR::TOP_LEFT)
	, m_renderRatio(1.0f)
	, m_renderRatioOffset(0.0f)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
}
PlayerUI::~PlayerUI() {  }

// テクスチャを読み込む
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

// 生成関数
void PlayerUI::Create(DX::DeviceResources* pDR
	, const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor)
{
	m_pDR = pDR;
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);

	//	シェーダーの作成
	CreateShader();

	//	画像の読み込み
	LoadTexture(path);
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);

}


// シェーダーの作成
void PlayerUI::CreateShader()
{
	// シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/PlayerHP/VS_PlayerHP.cso", m_vertexShader);
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/PlayerHP/GS_PlayerHP.cso", m_geometryShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerHP/PS_PlayerHP.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = m_geometryShader.Get();
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
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
	//	シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	板ポリゴンを描画
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();

}

void PlayerUI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
