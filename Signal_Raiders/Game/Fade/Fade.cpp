#include "pch.h"
#include "Fade.h"
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
	m_inputLayout{},
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
	m_fadeTexNum{ }
{
	// do nothing.
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
	DrawPolygon::InitializePositionTexture(m_pDR);
}
// シェーダー作成部分
void Fade::CreateShader()
{
	// デバイスリソースの取得
	auto device = m_pDR->GetD3DDevice();
	// 頂点シェーダーの作成
	kumachi::BinaryFile VSFade = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Fade/VS_Fade.cso");
	// ピクセルシェーダーの作成
	kumachi::BinaryFile PSFade = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Fade/PS_Fade.cso");
	// インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSFade.GetData(), VSFade.GetSize(),
		m_inputLayout.GetAddressOf());
	// 頂点シェーダーの作成
	if (FAILED(device->CreateVertexShader(VSFade.GetData(), VSFade.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
	{
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダーの作成
	if (FAILED(device->CreatePixelShader(PSFade.GetData(), PSFade.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
	{
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
	DrawPolygon::UpdateSubResources(context, m_CBuffer.Get(), &cbuff);
	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	//	描画準備
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_texture);
	//	シェーダをセットする
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	//	板ポリゴンを描画
	DrawPolygon::DrawTexture(vertex);
	//	シェーダの登録を解除しておく
	DrawPolygon::ReleaseShader(context);
}