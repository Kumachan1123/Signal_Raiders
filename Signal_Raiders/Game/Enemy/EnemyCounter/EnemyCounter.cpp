/*
	@file	EnemyCounter.cpp
	@brief	敵カウントクラス
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
	m_verticesEnemyIndex10{},
	m_verticesEnemyIndex1{},
	m_verticesNowEnemy10{},
	m_verticesNowEnemy1{},
	m_verticesRemaining{},
	m_verticesSlash{}

{}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
EnemyCounter::~EnemyCounter() {}

//---------------------------------------------------------
// 画像を読み込む
//---------------------------------------------------------
void EnemyCounter::LoadTexture(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& tex)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	tex = texture;
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void EnemyCounter::Initialize(CommonResources* commonResources)
{
	m_commonResources = commonResources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// テクスチャの読み込み
	LoadTexture(L"Resources/Textures/number.png", m_texture);//	数字
	LoadTexture(L"Resources/Textures/remaining.png", m_remaining);//	「残り：」
	LoadTexture(L"Resources/Textures/slash.png", m_slash);//	「/」
	// プリミティブバッチ作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

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

	// 各頂点の初期化
	for (int i = 0; i < 4; i++)
	{
		m_verticesEnemyIndex10[i] = {};
		m_verticesEnemyIndex1[i] = {};
		m_verticesNowEnemy10[i] = {};
		m_verticesNowEnemy1[i] = {};
		m_verticesRemaining[i] = {};
		m_verticesSlash[i] = {};
	}
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


	// 深度ステンシル状態を設定（深度バッファを有効にする）

	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);



	////	不透明のみ描画する設定 
	//m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	//m_batchEffect->SetReferenceAlpha(0);


	DrawRemaining();
	DrawEnemyIndex1();
	DrawEnemyIndex10();
	DrawSlash();
	DrawNowEnemy1();
	DrawNowEnemy10();
}



//---------------------------------------------------------
// 現在の敵の数の10の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy10()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	int currentRow = m_nowEnemy10 / m_frameCols;
	int currentCol = m_nowEnemy10 % m_frameCols;



	// テクスチャ座標の計算
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転
	m_verticesNowEnemy10[0] = { VertexPositionTexture(SimpleMath::Vector3(0.66f, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesNowEnemy10[1] = { VertexPositionTexture(SimpleMath::Vector3(0.74f, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesNowEnemy10[2] = { VertexPositionTexture(SimpleMath::Vector3(0.74f, 0.82f, 0), SimpleMath::Vector2(uMax, vMax)) };
	m_verticesNowEnemy10[3] = { VertexPositionTexture(SimpleMath::Vector3(0.66f, 0.82f, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	半透明部分を描画 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesNowEnemy10[0], m_verticesNowEnemy10[1], m_verticesNowEnemy10[2], m_verticesNowEnemy10[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// 現在の敵の数の1の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy1()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	int currentRow = m_nowEnemy1 / m_frameCols;
	int currentCol = m_nowEnemy1 % m_frameCols;



	// テクスチャ座標の計算
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転
	m_verticesNowEnemy1[0] = { VertexPositionTexture(SimpleMath::Vector3(0.72f, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesNowEnemy1[1] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesNowEnemy1[2] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 0.82f, 0), SimpleMath::Vector2(uMax, vMax)) };
	m_verticesNowEnemy1[3] = { VertexPositionTexture(SimpleMath::Vector3(0.72f, 0.82f, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	半透明部分を描画 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesNowEnemy1[0], m_verticesNowEnemy1[1], m_verticesNowEnemy1[2], m_verticesNowEnemy1[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// 総数の10の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex10()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	int currentRow = m_enemyIndex10 / m_frameCols;
	int currentCol = m_enemyIndex10 % m_frameCols;



	// テクスチャ座標の計算
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転
	m_verticesEnemyIndex10[0] = { VertexPositionTexture(SimpleMath::Vector3(0.86, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesEnemyIndex10[1] = { VertexPositionTexture(SimpleMath::Vector3(0.94, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesEnemyIndex10[2] = { VertexPositionTexture(SimpleMath::Vector3(0.94, 0.82f, 0), SimpleMath::Vector2(uMax, vMax)) };
	m_verticesEnemyIndex10[3] = { VertexPositionTexture(SimpleMath::Vector3(0.86, 0.82f, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	半透明部分を描画 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesEnemyIndex10[0], m_verticesEnemyIndex10[1], m_verticesEnemyIndex10[2], m_verticesEnemyIndex10[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// 総数の1の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex1()
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
	m_verticesEnemyIndex1[0] = { VertexPositionTexture(SimpleMath::Vector3(0.92f, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesEnemyIndex1[1] = { VertexPositionTexture(SimpleMath::Vector3(1.0f, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesEnemyIndex1[2] = { VertexPositionTexture(SimpleMath::Vector3(1.0f, 0.82f, 0), SimpleMath::Vector2(uMax,vMax)) };
	m_verticesEnemyIndex1[3] = { VertexPositionTexture(SimpleMath::Vector3(0.92f, 0.82f, 0), SimpleMath::Vector2(uMin,vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	半透明部分を描画 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesEnemyIndex1[0], m_verticesEnemyIndex1[1], m_verticesEnemyIndex1[2], m_verticesEnemyIndex1[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// 「のこり：」を描画
//---------------------------------------------------------
void EnemyCounter::DrawRemaining()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転
	m_verticesRemaining[0] = { VertexPositionTexture(SimpleMath::Vector3(0.4f, 1.0f, 0), SimpleMath::Vector2(0,0)) };
	m_verticesRemaining[1] = { VertexPositionTexture(SimpleMath::Vector3(0.65f, 1.0f, 0), SimpleMath::Vector2(1,0)) };
	m_verticesRemaining[2] = { VertexPositionTexture(SimpleMath::Vector3(0.65f, 0.82f, 0), SimpleMath::Vector2(1,1)) };
	m_verticesRemaining[3] = { VertexPositionTexture(SimpleMath::Vector3(0.4f, 0.82f, 0), SimpleMath::Vector2(0,1)) };
	m_batchEffect->SetTexture(m_remaining.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	半透明部分を描画 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesRemaining[0], m_verticesRemaining[1], m_verticesRemaining[2], m_verticesRemaining[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// 「/」を描画
//---------------------------------------------------------
void EnemyCounter::DrawSlash()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報
	// 頂点情報（板ポリゴンの頂点）を上下反転
	m_verticesSlash[0] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 1.0f, 0), SimpleMath::Vector2(0,0)) };
	m_verticesSlash[1] = { VertexPositionTexture(SimpleMath::Vector3(0.85f, 1.0f, 0), SimpleMath::Vector2(1,0)) };
	m_verticesSlash[2] = { VertexPositionTexture(SimpleMath::Vector3(0.85f, 0.82f, 0), SimpleMath::Vector2(1,1)) };
	m_verticesSlash[3] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 0.82f, 0), SimpleMath::Vector2(0,1)) };
	m_batchEffect->SetTexture(m_slash.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	半透明部分を描画 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesSlash[0], m_verticesSlash[1], m_verticesSlash[2], m_verticesSlash[3]);
	m_primitiveBatch->End();
}