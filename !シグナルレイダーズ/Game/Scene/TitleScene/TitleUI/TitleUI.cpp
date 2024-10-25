/*
	@file	TitleUI.cpp
	@brief	タイトルUIクラス
*/
#include "pch.h"
#include "TitleUI.h"
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
const std::vector<D3D11_INPUT_ELEMENT_DESC> TitleUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
TitleUI::TitleUI()
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

TitleUI::~TitleUI()
{
}

void TitleUI::LoadTexture(const wchar_t* path)
{
	//	指定された画像を読み込む
	HRESULT result = DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, m_pTextureResource.ReleaseAndGetAddressOf(), m_pTexture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_pTextureResource.As(&tex));

	//	読み込んだ画像の情報を取得する
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	//	読み込んだ画像のサイズを取得する
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;
	UNREFERENCED_PARAMETER(result);
}

void TitleUI::Create(DX::DeviceResources* pDR, const wchar_t* path
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

	//	プリミティブバッチを生成
	m_pPrimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());

	//	ステートを生成
	m_pStates = std::make_unique<DirectX::CommonStates>(m_pDR->GetD3DDevice());

}

void TitleUI::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();// デバイス
	// コンパイルされたシェーダーを読み込む
	kumachi::BinaryFile VS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/Menu/VS_Menu.cso");
	kumachi::BinaryFile GS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/Menu/GS_Menu.cso");
	kumachi::BinaryFile PS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/Menu/PS_Menu.cso");
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
}

void TitleUI::Update(float elapsedTime)
{
	m_time += elapsedTime;

}

void TitleUI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	板ポリ描画処理
	auto context = m_pDR->GetD3DDeviceContext();// コンテキスト
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))
		,Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		,Vector2(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)))
	};
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	m_constBuffer.time = m_time;
	m_constBuffer.color = Vector3(0.0, 0.5, 0.75);
	// 受け渡し用バッファの内容更新
	context->UpdateSubresource(m_pCBuffer.Get(), 0, NULL, &m_constBuffer, 0, 0);
	// シェーダーにバッファを渡す
	ID3D11Buffer* pCB[1] = { m_pCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, pCB);
	context->GSSetConstantBuffers(0, 1, pCB);
	context->PSSetConstantBuffers(0, 1, pCB);
	// 画像用サンプラーステートの設定
	ID3D11SamplerState* sampler[1] = { m_pStates->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	// 半透明描画指定
	ID3D11BlendState* pBlendState = m_pStates->NonPremultiplied();
	// 透明判定
	context->OMSetBlendState(pBlendState, nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照
	context->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);
	// カリングなし
	context->RSSetState(m_pStates->CullNone());
	// シェーダーの設定
	context->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	// ピクセルシェーダーにテクスチャを渡す
	context->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	// インプットレイアウトを設定
	context->IASetInputLayout(m_pInputLayout.Get());
	//	板ポリゴンを描画
	m_pPrimitiveBatch->Begin();
	m_pPrimitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_pPrimitiveBatch->End();
	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void TitleUI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
