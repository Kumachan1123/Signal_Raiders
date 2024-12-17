/*
	@file	EnemyCounter.cpp
	@brief	敵カウントクラス(現在シェーダー未使用)
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
	DrawQuad(m_remaining, m_verticesRemaining, 0.4f, 1.0f, 0.25f, 0.18f, 0, 1, 1); // 「残り：」
	DrawQuad(m_slash, m_verticesSlash, 0.8f, 1.0f, 0.05f, 0.18f, 0, 1, 1);       // 「/」
	DrawEnemyIndex1();// 総数の1の位を描画
	DrawEnemyIndex10();// 総数の10の位を描画
	DrawNowEnemy1();// 現在の敵の数の1の位を描画
	DrawNowEnemy10();// 現在の敵の数の10の位を描画
}



//---------------------------------------------------------
// 現在の敵の数の10の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy10()
{
	DrawQuad(
		m_texture, m_verticesNowEnemy10,
		0.66f, 1.0f, 0.08f, 0.18f, // 位置とサイズ
		m_nowEnemy10, m_frameCols, m_frameRows // フレーム情報
	);
}

//---------------------------------------------------------
// 現在の敵の数の1の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy1()
{
	DrawQuad(
		m_texture, m_verticesNowEnemy1,
		0.72f, 1.0f, 0.08f, 0.18f, // 位置とサイズ
		m_nowEnemy1, m_frameCols, m_frameRows // フレーム情報
	);
}

//---------------------------------------------------------
// 総数の10の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex10()
{
	DrawQuad(
		m_texture, m_verticesEnemyIndex10,
		0.86f, 1.0f, 0.08f, 0.18f, // 位置とサイズ
		m_enemyIndex10, m_frameCols, m_frameRows // フレーム情報
	);

}

//---------------------------------------------------------
// 総数の1の位を描画
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex1()
{
	DrawQuad(
		m_texture, m_verticesEnemyIndex1,
		0.92f, 1.0f, 0.08f, 0.18f, // 位置とサイズ
		m_enemyIndex1, m_frameCols, m_frameRows // フレーム情報
	);
}

//---------------------------------------------------------
// テクスチャを描画
//---------------------------------------------------------
void EnemyCounter::DrawQuad(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture, DirectX::VertexPositionTexture* vertices, float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// フレーム位置の計算
	int currentRow = frameIndex / frameCols;
	int currentCol = frameIndex % frameCols;

	// テクスチャ座標の計算
	float uMin = static_cast<float>(currentCol) / frameCols;
	float vMin = static_cast<float>(currentRow) / frameRows;
	float uMax = static_cast<float>(currentCol + 1) / frameCols;
	float vMax = static_cast<float>(currentRow + 1) / frameRows;

	// 頂点座標の設定
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(uMin, vMin)) };
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(uMax, vMin)) };
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(uMax, vMax)) };
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(uMin, vMax)) };

	// テクスチャ適用と描画
	m_batchEffect->SetTexture(texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	m_primitiveBatch->End();
}
