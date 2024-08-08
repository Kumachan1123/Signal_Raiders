#include "pch.h"
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Particle.h"



using namespace DirectX;


Particle::Particle(CommonResources* resources, DirectX::SimpleMath::Vector3 PlayPos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Matrix world)
	:m_position{ PlayPos }
	, m_scale{ 3.0f }
	, m_commonResources{ resources }
	, m_Billboard{ DirectX::SimpleMath::Matrix::Identity }
	, m_rotation{ rot }
	, m_world{ world }
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_anim = 0;
	m_animSpeed = 25; // フレーム切り替え速度
	m_elapsedTime = 0.0f;
	m_frameRows = 4; // 画像の行数
	m_frameCols = 5; // 画像の列数


	// エフェクトの作成 
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_BatchEffect->SetReferenceAlpha(1); // 0ではなく1に設定して完全な黒を除外する

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	// 共通ステート生成
	m_States = std::make_unique<CommonStates>(device);

	// テクスチャロード
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/effect.png",
		nullptr,
		m_Texture.GetAddressOf()
	);

	// アニメーション再生中
	m_isPlaying = true;
}
Particle::~Particle() {}

void Particle::Update(float elapsedTime)
{
	m_elapsedTime += elapsedTime * m_animSpeed;
	if (m_elapsedTime >= 1.0f)
	{
		m_anim++;
		m_elapsedTime = 0.0f;
	}
	if (m_anim == m_frameRows * m_frameCols)
	{

		//m_isPlaying = false;// 再生終了
	}
}

void Particle::Render(ID3D11DeviceContext1* context, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	int currentRow = m_anim / m_frameCols;
	int currentCol = m_anim % m_frameCols;

	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転

	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(-1, 1, 0), SimpleMath::Vector2(uMax, vMin)) };
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(1, 1, 0), SimpleMath::Vector2(uMax, vMax)) };
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(1, -1, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(-1, -1, 0), SimpleMath::Vector2(uMin, vMin)) };



	// プリミティブバッチ作成
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	VertexPositionTexture billboardVertex[4];
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = m_vertices[i];

		// 大きさを変える場合は最初にかける
		billboardVertex[i].position.x *= m_scale;
		billboardVertex[i].position.y *= m_scale;

		// 高さをちょっと下げる
		billboardVertex[i].position.y -= 2.0f;

	}

	// ビルボード行列を作成
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;

	// ビルボードをアフィン変換
	Matrix worldBillboard = m_world * billboardMatrix;
	worldBillboard *= Matrix::CreateTranslation(m_position);

	// 深度ステンシル状態を設定（深度バッファを有効にする）

	context->OMSetDepthStencilState(m_States->DepthDefault(), 0);
	context->OMSetBlendState(m_States->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	// カリングは左周り（反時計回り）
	context->RSSetState(m_States->CullCounterClockwise());

	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード）
	ID3D11SamplerState* samplers[1] = { m_States->AnisotropicWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	不透明のみ描画する設定 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_BatchEffect->SetReferenceAlpha(0);


	m_BatchEffect->SetWorld(worldBillboard);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_Texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	半透明部分を描画 
	m_Batch->Begin();
	m_Batch->DrawQuad(billboardVertex[0], billboardVertex[1], billboardVertex[2], billboardVertex[3]);
	m_Batch->End();


}

void Particle::Finalize()
{

}
