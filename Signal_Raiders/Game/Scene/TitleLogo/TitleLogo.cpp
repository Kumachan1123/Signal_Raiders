/*
	@file	TitleLogo.cpp
	@brief	タイトルロゴクラス
*/
#include "pch.h"
#include "TitleLogo.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"



using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC>  TitleLogo::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
TitleLogo::TitleLogo(CommonResources* resources)
	:
	m_commonResources{ resources },
	m_vertexShader{},
	m_pixelShader{},
	m_inputLayout{},
	m_pDR{},
	m_CBuffer{},
	m_batch{},
	m_states{},
	m_titleTexture{},
	m_time{},
	m_world{},
	m_view{},
	m_proj{},
	m_ConstBuffer{}
{}

TitleLogo::~TitleLogo()
{
	DrawPolygon::ReleasePositionTexture();
}

void TitleLogo::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_titleTexture.push_back(texture);
}

void TitleLogo::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	// シェーダーの作成
	CreateShader();
	// 画像の読み込み（2枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/Title.png");
	// 板ポリゴンの描画用
	DrawPolygon::InitializePositionTexture(m_pDR);
}

void TitleLogo::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();
	//	コンパイルされたシェーダファイルを読み込み
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
}

void TitleLogo::Update(float elapsedTime)
{
	m_time += elapsedTime; // 時間をカウント
}

void TitleLogo::Render()
{
	//	板ポリ描画処理
	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(-0.85f,  0.75f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.85f,  0.75f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.85f, -0.75f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.85f, -0.75f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// ビュー設定
	m_ConstBuffer.matView = m_view.Transpose();
	// プロジェクション設定
	m_ConstBuffer.matProj = m_proj.Transpose();
	// ワールド設定
	m_ConstBuffer.matWorld = m_world.Transpose();
	// グラデーションエフェクトの色設定 
	m_ConstBuffer.Colors = SimpleMath::Vector4(0.25f, 0.75f, 0.75f, 0);
	// 時間設定
	m_ConstBuffer.time = SimpleMath::Vector4(m_time);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	DrawPolygon::UpdateSubResources(context, m_CBuffer.Get(), &m_ConstBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	// 描画準備
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_titleTexture);
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す

	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	// 板ポリゴンを描画
	DrawPolygon::DrawTexture(vertex);
	// シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}