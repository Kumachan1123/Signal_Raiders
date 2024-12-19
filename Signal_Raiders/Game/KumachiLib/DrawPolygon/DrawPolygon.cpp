#include "pch.h"
#include "DrawPolygon.h"
//シングルトンにする

std::unique_ptr<DrawPolygon> DrawPolygon::m_instance = nullptr;

DrawPolygon* const DrawPolygon::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new DrawPolygon());
	}
	return m_instance.get();
}

// コンストラクタ
DrawPolygon::DrawPolygon()
	:
	m_primitiveBatchTexture(nullptr),
	m_primitiveBatchColorTexture(nullptr),
	m_states(nullptr),
	m_context(nullptr),
	m_pDR(nullptr),
	m_device(nullptr)
{
}

// デストラクタ
DrawPolygon::~DrawPolygon()
{
	// プリミティブバッチの解放
	ReleasePositionTexture();
	ReleasePositionColorTexture();
}

// 初期化（頂点、テクスチャ）
void DrawPolygon::InitializePositionTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_device = pDR->GetD3DDevice();
	m_context = pDR->GetD3DDeviceContext();
	// プリミティブバッチを作成する
	m_primitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	m_states = std::make_unique<CommonStates>(m_device);
}

// 初期化（頂点、色、テクスチャ）
void DrawPolygon::InitializePositionColorTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_device = pDR->GetD3DDevice();
	m_context = pDR->GetD3DDeviceContext();
	// プリミティブバッチの作成
	m_primitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());
	m_states = std::make_unique<CommonStates>(m_device);
}

// 描画開始（頂点、テクスチャ）
void DrawPolygon::DrawStart(ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	m_context->PSSetSamplers(0, 1, sampler);
	//	透明判定処理
	m_context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	//	深度バッファに書き込み参照する
	m_context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//	カリングは左周り
	m_context->RSSetState(m_states->CullNone());
	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < textures.size(); i++)
	{
		//	for文で一気に設定する
		m_context->PSSetShaderResources(i, 1, textures[i].GetAddressOf());
	}
	//	インプットレイアウトの登録
	m_context->IASetInputLayout(pInputLayout);
}

// サブリソースの更新
void DrawPolygon::UpdateSubResources(ID3D11Resource* resource, const void* pSrcData)
{
	m_context->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);
}

// 板ポリゴン描画（頂点、テクスチャ）
void DrawPolygon::DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices)
{
	m_primitiveBatchTexture->Begin();
	m_primitiveBatchTexture->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	m_primitiveBatchTexture->End();

}

// 板ポリゴン描画（頂点、色、テクスチャ）
void DrawPolygon::DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count)
{
	m_primitiveBatchColorTexture->Begin();
	m_primitiveBatchColorTexture->Draw(topology, vertices, count);
	m_primitiveBatchColorTexture->End();
}

void DrawPolygon::ReleasePositionTexture()
{
	// 全ポインターの解放
	m_primitiveBatchTexture.reset();
	m_states.reset();
	m_context = nullptr;
	m_pDR = nullptr;
	m_device = nullptr;
}


// 解放
void DrawPolygon::ReleasePositionColorTexture()
{
	m_primitiveBatchColorTexture.reset();
	m_states.reset();
	m_context = nullptr;
	m_pDR = nullptr;
	m_device = nullptr;
}

void DrawPolygon::SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	m_context->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	m_context->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	m_context->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
}

void DrawPolygon::SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	if (shaders.vs.Get() != nullptr) m_context->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.gs.Get() != nullptr) m_context->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.ps.Get() != nullptr) m_context->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances);
}

void DrawPolygon::ReleaseShader()
{
	m_context->VSSetShader(nullptr, nullptr, 0);
	m_context->GSSetShader(nullptr, nullptr, 0);
	m_context->PSSetShader(nullptr, nullptr, 0);
}
