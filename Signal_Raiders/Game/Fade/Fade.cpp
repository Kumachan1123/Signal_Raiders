#include "pch.h"
#include "Fade.h"
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
#include "Game/KumachiLib/KumachiLib.h"
#include "Libraries/MyLib/DebugString.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// フェード最小値
const float Fade::FADE_MIN = -1.01f;
// フェード最大値
const float Fade::FADE_MAX = 1.01f;
// フェード速度
const float Fade::FADE_SPEED = 1.5f;


// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
// コンストラクタ
Fade::Fade(CommonResources* commonResources)
	:
	m_pDR{ },
	m_CBuffer{},
	m_pInputLayout{},
	m_commonResources{ commonResources },
	m_batch{},
	m_states{},
	m_texture{},
	m_texture2{},
	m_vertexShader{},
	m_pixelShader{},
	m_world{},
	m_view{},
	m_proj{},
	m_time{ -0.75f },
	m_fadeState{ FadeState::None },
	m_fadeTexNum{ },
	m_pDrawPolygon{ DrawPolygon::GetInstance() },
	m_pCreateShader{ CreateShader::GetInstance() }
{
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
// テクスチャの読み込み
void Fade::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}
// 生成
void Fade::Create(DX::DeviceResources* pDR)
{
	// デバイスリソースの取得
	m_pDR = pDR;
	// シェーダーの作成
	CreateShader();
	// 画像の読み込み
	LoadTexture(L"Resources/Textures/fade2.png");//fadeTex
	LoadTexture(L"Resources/Textures/Ready.png");//readyTex
	LoadTexture(L"Resources/Textures/Go.png");//goTex
	LoadTexture(L"Resources/Textures/Black.png");//backTex
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
// シェーダー作成部分
void Fade::CreateShader()
{
	// 頂点シェーダーとピクセルシェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Fade/VS_Fade.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Fade/PS_Fade.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}
// 更新
void Fade::Update(float elapsedTime)
{
	// フェードイン
	if (m_fadeState == FadeState::FadeIn)
	{
		// 時間を減算
		m_time += elapsedTime * FADE_SPEED;
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);
		if (m_time >= FADE_MAX)
		{
			m_fadeState = FadeState::FadeInEnd;
		}
	}
	// フェードアウト
	if (m_fadeState == FadeState::FadeOut)
	{
		// 時間を加算
		m_time -= elapsedTime * FADE_SPEED;
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);
		if (m_time <= FADE_MIN)
		{
			m_fadeState = FadeState::FadeOutEnd;
		}
	}
}

// 描画
void Fade::Render()
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
	ConstBuffer cbuff;
	cbuff.power = 0.01f;// フェードとディゾルブの設定
	cbuff.fadeAmount = m_time;
	cbuff.num = m_fadeTexNum;	// 画像番号
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &cbuff);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	//	描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	//	シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}