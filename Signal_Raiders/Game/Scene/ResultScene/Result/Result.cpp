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
#include "Game/KumachiLib//BinaryFile.h"
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
	m_inputLayout{},
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
	m_ConstBuffer{}
{}

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
	DrawPolygon::InitializePositionTexture(m_pDR);
}

void Result::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();
	//	コンパイルされたシェーダファイルを読み込み（タイトル画像のシェーダーを使いまわす）
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/VS_Title.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/PS_Title.cso");
	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), VSData.GetData(), VSData.GetSize(), m_inputLayout.GetAddressOf());
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
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
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
	DrawPolygon::UpdateSubResources(context, m_CBuffer.Get(), &m_ConstBuffer);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	//	シェーダをセットする
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	// 描画準備
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_texture);
	// 板ポリゴンを描画
	DrawPolygon::DrawTexture(m_vertex);
	// シェーダの登録を解除しておく
	DrawPolygon::ReleaseShader(context);
}