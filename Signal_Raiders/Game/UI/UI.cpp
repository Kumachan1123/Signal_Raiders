/*
	@file	UI.cpp
	@brief	UIクラス(タイトル・リザルト・セッティング)
*/
#include "pch.h"
#include "UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
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
	, m_anchor{ KumachiLib::ANCHOR::TOP_LEFT }
	, m_shaderType{ ShaderType::NORMAL }
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
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
	, KumachiLib::ANCHOR anchor)
{
	m_pDR = pDR;// デバイスリソース
	m_position = position;// 位置
	m_baseScale = m_scale = scale;// スケール
	m_anchor = anchor;// アンカー
	//	シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	//	シェーダーの作成
	CreateShader();
	//	テクスチャを読み込む
	LoadTexture(path);
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);

}

// マウスとの当たり判定
bool UI::IsHit(const DirectX::SimpleMath::Vector2& pos) const
{
	//	画像の左上の座標を取得
	DirectX::SimpleMath::Vector2 leftTop = m_position - DirectX::SimpleMath::Vector2(float(m_textureWidth) / 2.0f, float(m_textureHeight) / 2.0f);
	//	画像の右下の座標を取得
	DirectX::SimpleMath::Vector2 rightBottom = m_position + DirectX::SimpleMath::Vector2(float(m_textureWidth) / 2.0f, float(m_textureHeight) / 2.0f);
	//	画像の左上の座標と右下の座標を比較して、マウスの座標が範囲内にあるかを判定する
	if (leftTop.x <= pos.x && pos.x <= rightBottom.x
		&& leftTop.y <= pos.y && pos.y <= rightBottom.y)
	{
		return true;
	}
	return false;

}

void UI::CreateShader()
{
	// シェーダーを作成する
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Menu/VS_Menu.cso", m_pVertexShader);
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Menu/GS_Menu.cso", m_pGeometryShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_Menu.cso", m_pPixelShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_StageMenu.cso", m_pPixelShaderStageSelect);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	m_shaders.gs = m_pGeometryShader.Get();
	m_shaders.ps = m_pPixelShader.Get();
	m_StageSelectShaders.vs = m_pVertexShader.Get();
	m_StageSelectShaders.gs = m_pGeometryShader.Get();
	m_StageSelectShaders.ps = m_pPixelShaderStageSelect.Get();
}

void UI::Update(float elapsedTime)
{
	m_time += elapsedTime;

}

void UI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

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
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// 描画
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	//	シェーダをセットする
	if (m_shaderType == ShaderType::NORMAL)
		m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	else
		m_pDrawPolygon->SetShader(m_StageSelectShaders, nullptr, 0);

	//	板ポリゴンを描画
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

void UI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
