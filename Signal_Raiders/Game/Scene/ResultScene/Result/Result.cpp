/*
	@file	Result.cpp
	@brief	結果クラス
*/
#include "pch.h"
#include "Result.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Result::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
Result::Result(CommonResources* resources)
	:
	m_commonResources{ resources },
	m_vertexShader{},
	m_pixelShader{},
	m_pInputLayout{},
	m_pDR{},
	m_CBuffer{},
	m_batch{},
	m_states{},
	m_texture{},
	m_time{},
	m_world{},
	m_view{},
	m_proj{},
	m_vertex{},
	m_ConstBuffer{},
	m_pDrawPolygon{ DrawPolygon::GetInstance() },
	m_pCreateShader{ CreateShader::GetInstance() }
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}

Result::~Result()
{}

void Result::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}

void Result::Create(DX::DeviceResources* pDR, const wchar_t* path)
{
	m_pDR = pDR;
	// シェーダーの作成
	CreateShader();
	// 画像の読み込み
	LoadTexture(path);
	// ゲームオーバー画像
	if (wcscmp(path, L"Resources/Textures/GameOver.png") == 0)
	{
		// グラデーションエフェクトの色設定
		m_ConstBuffer.Colors = SimpleMath::Vector4(0.15f, 0, 0.5f, 0);
		//	頂点情報(板ポリゴンの４頂点の座標情報）
		VertexPositionTexture vertex[4] =
		{
			//	頂点情報													UV情報
			VertexPositionTexture(SimpleMath::Vector3(-0.85f,  0.4f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.85f,  0.4f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.85f, -0.4f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-0.85f, -0.4f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
		};
		for (int i = 0; i < 4; i++)
		{
			m_vertex[i] = vertex[i];
		}
	}
	else // クリア画像
	{
		// グラデーションエフェクトの色設定
		m_ConstBuffer.Colors = SimpleMath::Vector4(0.5f, 0.5f, 0, 0);
		//	頂点情報(板ポリゴンの４頂点の座標情報）
		VertexPositionTexture vertex[4] =
		{
			//	頂点情報													UV情報
			VertexPositionTexture(SimpleMath::Vector3(-0.8f,  0.5f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.8f,  0.5f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.8f, -0.5f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-0.8f, -0.5f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
		};
		for (int i = 0; i < 4; i++)
		{
			m_vertex[i] = vertex[i];
		}
	}
	// 頂点の座標を調整
	for (int i = 0; i < 4; i++)
	{
		m_vertex[i].position.y += 0.25f;
	}
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}

void Result::CreateShader()
{
	// 頂点シェーダーとピクセルシェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}

void Result::Update(float elapsedTime)
{
	m_time += elapsedTime; // 時間をカウント
}

void Result::Render()
{
	//	板ポリ描画処理
	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	ビュー設定
	m_ConstBuffer.matView = m_view.Transpose();
	//	プロジェクション設定
	m_ConstBuffer.matProj = m_proj.Transpose();
	//	ワールド設定
	m_ConstBuffer.matWorld = m_world.Transpose();
	// グラデーションエフェクトの色設定はCreate関数で設定済み
	// 時間設定
	m_ConstBuffer.time = m_time;
	//	パディング
	m_ConstBuffer.padding = SimpleMath::Vector3(0, 0, 0);
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_ConstBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	//	シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(m_vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}