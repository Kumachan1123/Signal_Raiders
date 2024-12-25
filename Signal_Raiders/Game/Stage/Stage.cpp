#include "pch.h"

#include "Game/Stage/Stage.h"



Stage::Stage()
	: m_commonResources(nullptr)
	, m_pBatchEffect(nullptr)
	, m_pPrimitiveBatch(nullptr)
	, m_pInputLayout(nullptr)
	, m_pStates(nullptr)
	, m_pTexture(nullptr)
	, m_depthStencilState_Floor(nullptr)
{
}
Stage::~Stage() {}

void Stage::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	// エフェクトの作成 
	m_pBatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_pBatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_pBatchEffect->SetReferenceAlpha(255);
	// 入力レイアウト生成 
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_pBatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_pInputLayout.GetAddressOf()
	);
	// 共通ステートの作成
	m_pStates = std::make_unique<CommonStates>(device);

	// 床テクスチャのロード 
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tile.png",
		nullptr,
		m_pTexture.GetAddressOf()
	);



}

void Stage::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	//	プリミティブバッチの作成 
	m_pPrimitiveBatch =
		std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	頂点情報（床の頂点） 
	VertexPositionTexture vertex[4] =
	{//												座標					UV座標（ふつうは0～1の間で指定。超えた場合は繰り返す
		VertexPositionTexture(SimpleMath::Vector3(100.0f, 0.00f, 100.0f),	SimpleMath::Vector2(.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.0f, 100.0f),	SimpleMath::Vector2(25.0f, .0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.f, -100.0f),	SimpleMath::Vector2(25.0f,25.0f)),
		VertexPositionTexture(SimpleMath::Vector3(100.0f,0.f, -100.0f),	SimpleMath::Vector2(0.0f, 25.0f)),
	};

	//	深度バッファに書き込み参照する 
	context->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);
	//	テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード） 
	ID3D11SamplerState* samplers[1] = { m_pStates->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	カリングは左周り（反時計回り） 
	context->RSSetState(m_pStates->CullNone());

	//	不透明のみ描画する設定 
	m_pBatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_pBatchEffect->SetReferenceAlpha(0);
	m_pBatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_pBatchEffect->SetView(view);
	m_pBatchEffect->SetProjection(proj);
	m_pBatchEffect->SetTexture(m_pTexture.Get());
	m_pBatchEffect->Apply(context);
	context->IASetInputLayout(m_pInputLayout.Get());


	//	半透明部分を描画 
	m_pPrimitiveBatch->Begin();
	m_pPrimitiveBatch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_pPrimitiveBatch->End();


}
