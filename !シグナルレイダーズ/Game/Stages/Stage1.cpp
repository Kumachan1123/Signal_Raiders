#include "pch.h"

#include "Game//Stages/Stage1.h"


Stage1::Stage1()
{

}
Stage1::~Stage1() {}

void Stage1::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	エフェクトの作成 
	m_BatchEffect = std::make_unique<NormalMapEffect>(device);
	//	入力レイアウト生成 
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	//	共通ステートの作成
	m_States = std::make_unique<CommonStates>(device);

	//	テクスチャのロード 
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tile.png",
		nullptr,
		m_Texture.GetAddressOf()
	);
	// ノーマルマップのロード
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tile_normal.png",
		nullptr,
		m_NormalMap.GetAddressOf()
	);
	m_BatchEffect->SetNormalTexture(*m_NormalMap.GetAddressOf());

}
void Stage1::Update(float elapsedTime)
{

}
void Stage1::Render(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	//	プリミティブバッチの作成 
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	頂点情報（板ポリゴンの頂点） 
	VertexPositionTexture vertex[4] =
	{//												座標					UV座標（ふつうは0～1の間で指定。超えた場合は繰り返す
		VertexPositionTexture(SimpleMath::Vector3(50.0f, 0.00f, 50.0f),	SimpleMath::Vector2(.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-50.0f, 0.0f, 50.0f),	SimpleMath::Vector2(50.0f, .0f)),
		VertexPositionTexture(SimpleMath::Vector3(-50.0f, 0.f, -50.0f),	SimpleMath::Vector2(50.0f,50.0f)),
		VertexPositionTexture(SimpleMath::Vector3(50.0f,0.f, -50.0f),	SimpleMath::Vector2(0.0f, 50.0f)),
	};

	//	テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード） 
	ID3D11SamplerState* samplers[1] = { m_States->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	深度バッファに書き込み参照する 
	context->OMSetDepthStencilState(m_States->DepthDefault(), 0);

	//	カリングは左周り（反時計回り） 
	context->RSSetState(m_States->CullCounterClockwise());

	//	不透明のみ描画する設定 

	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_Texture.Get());
	// ノーマルマップのセット
	context->PSSetShaderResources(1, 1, m_NormalMap.GetAddressOf());

	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	半透明部分を描画 
	m_Batch->Begin();
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}
