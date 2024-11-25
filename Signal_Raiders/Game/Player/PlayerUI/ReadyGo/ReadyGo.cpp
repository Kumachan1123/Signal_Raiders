/*
	@file	ReadyGo.cpp
	@brief	準備クラス
*/
#include "pch.h"
#include "ReadyGo.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Libraries/MyLib/DebugString.h"

using namespace DirectX;

// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC>  ReadyGo::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};
// コンストラクタ
ReadyGo::ReadyGo(CommonResources* resources)
	:m_pDR(nullptr)
	, m_time(0.0f)
	, m_constBuffer()
{
	m_commonResources = resources;
	// 色の初期化
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

}

// デストラクタ
ReadyGo::~ReadyGo()
{
}

// テクスチャリソース読み込み関数
void  ReadyGo::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}

// 生成関数
void  ReadyGo::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	ID3D11Device1* device = pDR->GetD3DDevice();

	//	シェーダーの作成
	CreateShader();

	//	画像の読み込み（2枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/crisis.png");
	LoadTexture(L"Resources/Textures/killAll.png");

	//	プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

}

// シェーダー作成部分
void  ReadyGo::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	コンパイルされたシェーダファイルを読み込み
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/ReadyGo/VS_ReadyGo.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/ReadyGo/PS_ReadyGo.cso");

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
}

//更新
void  ReadyGo::Update(float elapsedTime)
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



	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, m_constBuffer.colors.w);

}


// 描画関数
void  ReadyGo::Render()
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
	m_constBuffer.time = m_time;
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &m_constBuffer, 0, 0);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	//	カリングはなし
	context->RSSetState(m_states->CullNone());

	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	Create関数で読み込んだ画像をピクセルシェーダに登録する。
	for (int i = 0; i < m_texture.size(); i++)
	{
		//	for文で一気に設定する
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}