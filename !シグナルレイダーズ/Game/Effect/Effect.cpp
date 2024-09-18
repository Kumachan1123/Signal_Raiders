/*
	@file	Particle.cpp
	@brief	パーティクルクラス
	作成者：くまち
*/
#include "pch.h"
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Effect.h"



using namespace DirectX;


Effect::Effect(CommonResources* resources, ParticleType type, DirectX::SimpleMath::Vector3 PlayPos, DirectX::SimpleMath::Matrix world)
	: m_position{ PlayPos }
	, m_type{ type }
	, m_scale{ 3.0f }
	, m_commonResources{ resources }
	, m_world{ world }
	, m_isPlaying{ true }
	, m_anim{ 0 }
	, m_animSpeed{ 30 }
	, m_elapsedTime{ 0.0f }
	, m_frameRows{}
	, m_frameCols{}
	, m_vertices{}
	, m_offSetY{ 0.0f }

{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	const wchar_t* texturePath = L"Resources/Textures/effect.png";// テクスチャパスを仮設定
	switch (m_type)
	{
		case ParticleType::ENEMY_DEAD:// 敵死亡エフェクト
			texturePath = L"Resources/Textures/effect.png";
			m_frameRows = 4; // 画像の行数
			m_frameCols = 5; // 画像の列数
			m_offSetY = 2.0f;// 高さを変える
			m_vertexMinX = -2.0f;
			m_vertexMaxX = 2.0f;
			m_vertexMinY = -2.0f;
			m_vertexMaxY = 2.0f;
			break;
		case ParticleType::ENEMY_HIT:// 敵ヒットエフェクト
			texturePath = L"Resources/Textures/hit.png";
			m_frameRows = 2; // 画像の行数
			m_frameCols = 4; // 画像の列数
			m_offSetY = 2.0f;// 高さを変える
			m_vertexMinX = -1.0f;
			m_vertexMaxX = 1.0f;
			m_vertexMinY = -1.0f;
			m_vertexMaxY = 1.0f;
			break;
		case ParticleType::ENEMY_SPAWN:// 敵スポーンエフェクト
			texturePath = L"Resources/Textures/Born.png";
			m_frameRows = 4; // 画像の行数
			m_frameCols = 4; // 画像の列数
			m_offSetY = 2.0f;// 高さを変える
			m_vertexMinX = -1.0f;
			m_vertexMaxX = 1.0f;
			m_vertexMinY = -1.0f;
			m_vertexMaxY = 1.0f;
			break;

	}




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
		texturePath,
		nullptr,
		m_Texture.GetAddressOf()
	);

	// アニメーション再生中
	m_isPlaying = true;
}
Effect::~Effect() {}

void Effect::Update(float elapsedTime)
{
	m_elapsedTime += elapsedTime * m_animSpeed;
	if (m_elapsedTime >= 1.0f)
	{
		m_anim++;
		m_elapsedTime = 0.0f;
	}
	if (m_anim == m_frameRows * m_frameCols)
	{

		m_isPlaying = false;// 再生終了
	}
}

void Effect::Render(ID3D11DeviceContext1* context, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	int currentRow = m_anim / m_frameCols;
	int currentCol = m_anim % m_frameCols;



	// テクスチャ座標の計算
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMin)) };
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX,m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMin)) };
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX,m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMax)) };

	// プリミティブバッチ作成
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	VertexPositionTexture billboardVertex[4]{};
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = m_vertices[i];

		// 大きさを変える場合は最初にかける
		billboardVertex[i].position.x *= m_scale;
		billboardVertex[i].position.y *= m_scale;

		// 高さをちょっと下げる
		billboardVertex[i].position.y -= m_offSetY;

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

void Effect::Finalize()
{

}
