/*
	@file	ReadyGo.cpp
	@brief	準備クラス
*/
#include "pch.h"
#include "ReadyGo.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Libraries/MyLib/DebugString.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
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
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_commonResources{ resources }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
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
	// シェーダーの作成
	CreateShader();
	// 画像の読み込み（2枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/crisis.png");
	LoadTexture(L"Resources/Textures/killAll.png");
	// 板ポリゴン描画
	m_pDrawPolygon->InitializePositionTexture(m_pDR);

}

// シェーダー作成部分
void  ReadyGo::CreateShader()
{
	// 頂点シェーダーをピクセルシェーダーを作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/ReadyGo/VS_ReadyGo.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/ReadyGo/PS_ReadyGo.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
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
	// 色の更新
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
	m_constBuffer.time = SimpleMath::Vector4(m_time);
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	//	シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}