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
#include "Libraries/MyLib/DebugString.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
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
	m_timer{},
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
	m_time{ 0 }
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
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	// シェーダーの作成
	CreateShader();

	// 画像の読み込み
	LoadTexture(L"Resources/Textures/fade.png");

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());

	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(device);

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

}
// 更新
void Fade::Update(float elapsedTime)
{
	// 時間を加算
	m_time += elapsedTime;

}

// 描画
void Fade::Render()
{
	// 時間
	auto debugString = m_commonResources->GetDebugString();
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

	cbuff.power = 0.1f;

	// フェードとディゾルブの設定
	cbuff.fadeAmount = m_time;
	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->PointWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	//	カリングはなし
	context->RSSetState(m_states->CullNone());

	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

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