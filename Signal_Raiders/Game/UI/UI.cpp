/*
	@file	UI.cpp
	@brief	UIクラス(タイトル・リザルト・セッティング)
*/
#include "pch.h"
#include "UI.h"
#include "Game/KumachiLib/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
using namespace DirectX;
const std::vector<D3D11_INPUT_ELEMENT_DESC> UI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UI::UI()
	: m_pDR{ nullptr }
	, m_time{ 0 }
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_textureWidth{ 0 }
	, m_textureHeight{ 0 }
	, m_scale{ DirectX::SimpleMath::Vector2::One }
	, m_baseScale{ DirectX::SimpleMath::Vector2::One }
	, m_position{ DirectX::SimpleMath::Vector2::Zero }
	, m_anchor{ kumachi::ANCHOR::TOP_LEFT }
{
}

UI::~UI()
{
}

void UI::LoadTexture(const wchar_t* path)
{
	//	指定された画像を読み込む
	HRESULT result = DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, m_pTextureResource.ReleaseAndGetAddressOf(), m_pTexture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_pTextureResource.As(&tex));

	//	読み込んだ画像の情報を取得する
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	m_pTextures.push_back(m_pTexture);
	//	読み込んだ画像のサイズを取得する
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;
	UNREFERENCED_PARAMETER(result);
}

void UI::Create(DX::DeviceResources* pDR, const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, kumachi::ANCHOR anchor)
{
	m_pDR = pDR;// デバイスリソース
	m_position = position;// 位置
	m_baseScale = m_scale = scale;// スケール
	m_anchor = anchor;// アンカー
	//	シェーダーの作成
	CreateShader();
	//	テクスチャを読み込む
	LoadTexture(path);
	// 板ポリゴン描画用
	DrawPolygon::InitializePositionColorTexture(m_pDR);

}

void UI::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();// デバイス
	// コンパイルされたシェーダーを読み込む
	kumachi::BinaryFile VS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Menu/VS_Menu.cso");
	kumachi::BinaryFile GS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Menu/GS_Menu.cso");
	kumachi::BinaryFile PS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Menu/PS_Menu.cso");
	// インプットレイアウト作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VS.GetData(), VS.GetSize(),
		m_pInputLayout.GetAddressOf());
	//	頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, m_pVertexShader.ReleaseAndGetAddressOf())))
	{//	エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, m_pGeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	//	ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, m_pPixelShader.ReleaseAndGetAddressOf())))
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
	device->CreateBuffer(&bd, nullptr, &m_pCBuffer);
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	m_shaders.gs = m_pGeometryShader.Get();
	m_shaders.ps = m_pPixelShader.Get();
}

void UI::Update(float elapsedTime)
{
	m_time += elapsedTime;

}

void UI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pDR->GetD3DDeviceContext();// コンテキスト
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))
		, SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, SimpleMath::Vector2(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)))
	};
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	m_constBuffer.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	m_constBuffer.time = m_time;
	m_constBuffer.color = Vector3(0.5, 0.5, 0.5);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	DrawPolygon::UpdateSubResources(context, m_pCBuffer.Get(), &m_constBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	// 描画準備
	DrawPolygon::DrawStartColorTexture(context, m_pInputLayout.Get(), m_pTextures);
	//	シェーダをセットする
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	//	板ポリゴンを描画
	DrawPolygon::DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//	シェーダの登録を解除しておく
	DrawPolygon::ReleaseShader(context);
}

void UI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
