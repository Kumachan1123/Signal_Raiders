#include "pch.h"
#include "DrawPolygon.h"
//シングルトンにする

// 静的メンバーの初期化
std::unique_ptr<DirectX::BasicEffect> DrawPolygon::m_basicEffect = nullptr;
Microsoft::WRL::ComPtr<ID3D11InputLayout> DrawPolygon::m_inputLayout = nullptr;
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> DrawPolygon::m_primitiveBatchTexture = nullptr;
CommonResources* DrawPolygon::m_commonResources = nullptr;
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> DrawPolygon::m_primitiveBatchColorTexture = nullptr;
std::unique_ptr<DirectX::CommonStates> DrawPolygon::m_states = nullptr;

// 初期化（頂点、テクスチャ）
void DrawPolygon::InitializePositionTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = pDR->GetD3DDevice();

	// プリミティブバッチを作成する
	m_primitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	m_states = std::make_unique<CommonStates>(device);
}

// 初期化（頂点、色、テクスチャ）
void DrawPolygon::InitializePositionColorTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = pDR->GetD3DDevice();
	//	プリミティブバッチの作成
	m_primitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);
}

// 描画開始（頂点、テクスチャ）
void DrawPolygon::DrawStart(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures)
{


	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//	カリングは左周り
	context->RSSetState(m_states->CullNone());
	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < textures.size(); i++)
	{
		//	for文で一気に設定する
		context->PSSetShaderResources(i, 1, textures[i].GetAddressOf());
	}
	//	インプットレイアウトの登録
	context->IASetInputLayout(pInputLayout);
}

// サブリソースの更新
void DrawPolygon::UpdateSubResources(ID3D11DeviceContext1* context, ID3D11Resource* resource, const void* pSrcData)
{
	context->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);
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
	// 静的メンバーを解放
	m_basicEffect.reset();
	m_inputLayout.Reset();
	m_primitiveBatchTexture.reset();
	m_commonResources = nullptr;
}


// 解放
void DrawPolygon::ReleasePositionColorTexture()
{
	// 静的メンバーを解放
	m_basicEffect.reset();
	m_inputLayout.Reset();
	m_primitiveBatchColorTexture.reset();
	m_commonResources = nullptr;
}

void DrawPolygon::SetShaderBuffer(ID3D11DeviceContext1* context, UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	context->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	context->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	context->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
}

void DrawPolygon::SetShader(ID3D11DeviceContext1* context, const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	if (shaders.vs.Get() != nullptr) context->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.gs.Get() != nullptr) context->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.ps.Get() != nullptr) context->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances);
}

void DrawPolygon::ReleaseShader(ID3D11DeviceContext1* context)
{
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
