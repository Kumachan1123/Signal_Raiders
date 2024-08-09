#include "pch.h"

#include "Game//Stages/Stage1.h"


Stage1::Stage1()
	: m_commonResources(nullptr)
	, m_BatchEffect(nullptr)
	, m_Batch(nullptr)
	, m_InputLayout(nullptr)
	, m_States(nullptr)
	, m_Texture(nullptr)
	, m_depthStencilState_Floor(nullptr)
{

}
Stage1::~Stage1() {}

void Stage1::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	//auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	エフェクトの作成 
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);
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
	// 深度ステンシルバッファを初期化する
	this->InitializeDepthStencilState(device);
}
void Stage1::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

}
void Stage1::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	//	プリミティブバッチの作成 
	m_Batch =
		std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	頂点情報（板ポリゴンの頂点） 
	VertexPositionTexture vertex[4] =
	{//												座標					UV座標（ふつうは0～1の間で指定。超えた場合は繰り返す
		VertexPositionTexture(SimpleMath::Vector3(100.0f, 0.00f, 100.0f),	SimpleMath::Vector2(.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.0f, 100.0f),	SimpleMath::Vector2(50.0f, .0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.f, -100.0f),	SimpleMath::Vector2(50.0f,50.0f)),
		VertexPositionTexture(SimpleMath::Vector3(100.0f,0.f, -100.0f),	SimpleMath::Vector2(0.0f, 50.0f)),
	};


	//	深度バッファに書き込み参照する 
	context->OMSetDepthStencilState(m_depthStencilState_Floor.Get(), 0);
	//	テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード） 
	ID3D11SamplerState* samplers[1] = { m_States->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	カリングは左周り（反時計回り） 
	context->RSSetState(m_States->CullCounterClockwise());

	//	不透明のみ描画する設定 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_Texture.Get());

	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	半透明部分を描画 
	m_Batch->Begin();
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}
//---------------------------------------------------------
// 深度ステンシルステートを初期化する
//---------------------------------------------------------
void Stage1::InitializeDepthStencilState(ID3D11Device* device)
{
	assert(device);

	// 深度ステンシルバッファを設定する
	D3D11_DEPTH_STENCIL_DESC desc{};

	/*
		床の設定
	*/
	// 床（描画時０を１にする）
	desc.DepthEnable = TRUE;									// 深度テストを行う
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// 深度バッファを更新する
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// テストの条件：深度値以下なら→手前なら

	desc.StencilEnable = TRUE;									// ステンシルテストを行う
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// 表面
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// 参照値がステンシル値と同じなら：ゼロなら
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK　ステンシル値をインクリメントする
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG　何もしない
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG　何もしない

	// 裏面も同じ設定
	desc.BackFace = desc.FrontFace;

	// 深度ステンシルステートを作成する
	DX::ThrowIfFailed(
		device->CreateDepthStencilState(&desc, m_depthStencilState_Floor.ReleaseAndGetAddressOf())
	);


}
