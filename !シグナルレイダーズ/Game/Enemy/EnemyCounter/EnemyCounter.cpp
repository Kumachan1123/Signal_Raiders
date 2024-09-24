/*
	@file	EnemyCounter.cpp
	@brief	敵カウントクラス
	作成者：くまち
*/
#include "pch.h"
#include "EnemyCounter.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
EnemyCounter::EnemyCounter()
	:
	m_commonResources{},
	m_enemyIndex{ 0 },
	m_nowEnemy{ 0 },
	m_texture{},
	m_enemyIndex10{ 0 },
	m_enemyIndex1{ 0 },
	m_nowEnemy10{ 0 },
	m_nowEnemy1{ 0 },
	m_frameRows{ 1 },
	m_frameCols{ 10 },
	m_inputLayout{},
	m_states{},
	m_batchEffect{},
	m_vertices{},
	m_vertexMinX{ 0.5f },
	m_vertexMaxX{ 1.0f },
	m_vertexMinY{ 0.7f },
	m_vertexMaxY{ 1.0f }

{}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
EnemyCounter::~EnemyCounter() {}

//---------------------------------------------------------
// 画像を読み込む
//---------------------------------------------------------
void EnemyCounter::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture = texture;
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void EnemyCounter::Initialize(CommonResources* commonResources)
{
	m_commonResources = commonResources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	//	assert(commonResources);

	LoadTexture(L"Resources/Textures/number.png");


	// エフェクトの作成 
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_batchEffect->SetReferenceAlpha(1); // 0ではなく1に設定して完全な黒を除外する

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	// 共通ステート生成
	m_states = std::make_unique<CommonStates>(device);

}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void EnemyCounter::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// 受け取った値から10の位と1の位を計算
	m_enemyIndex10 = m_enemyIndex / 10;
	m_enemyIndex1 = m_enemyIndex % 10;
	m_nowEnemy10 = m_nowEnemy / 10;
	m_nowEnemy1 = m_nowEnemy % 10;


}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void EnemyCounter::Render()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	int currentRow = m_enemyIndex1 / m_frameCols;
	int currentCol = m_enemyIndex1 % m_frameCols;



	// テクスチャ座標の計算
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMin)) };
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMin)) };
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMax)) };
	// プリミティブバッチ作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	// 深度ステンシル状態を設定（深度バッファを有効にする）

	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	// カリングは左周り（反時計回り）
	context->RSSetState(m_states->CullCounterClockwise());

	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード）
	ID3D11SamplerState* samplers[1] = { m_states->AnisotropicWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	不透明のみ描画する設定 
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_batchEffect->SetReferenceAlpha(0);

	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	//	半透明部分を描画 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[2], m_vertices[3]);
	m_primitiveBatch->End();
}