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
const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyCounter::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
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
	m_pInputLayout{},
	m_states{},
	m_batchEffect{},
	m_verticesEnemyIndex10{},
	m_verticesEnemyIndex1{},
	m_verticesNowEnemy10{},
	m_verticesNowEnemy1{},
	m_verticesRemaining{},
	m_verticesSlash{},
	m_pCreateShader{ CreateShader::GetInstance() },
	m_pDrawPolygon{ DrawPolygon::GetInstance() }
{}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
EnemyCounter::~EnemyCounter() {}

//---------------------------------------------------------
// 画像を読み込む
//---------------------------------------------------------
void EnemyCounter::LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& tex)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	tex.push_back(texture);
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
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にセット
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
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
	DrawQuad(m_remaining, m_verticesRemaining, 0.4f, 1.0f, 0.25f, 0.18f, 0, 1, 1); // 「残り：」
	DrawQuad(m_slash, m_verticesSlash, 0.8f, 1.0f, 0.05f, 0.18f, 0, 1, 1); // 「/」
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
void EnemyCounter::DrawQuad(
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height,
	int frameIndex, int frameCols, int frameRows)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 頂点座標の設定
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(0, 0)) };
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(1, 0)) };
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(1, 1)) };
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(0, 1)) };
	m_constBuffer.matWorld = Matrix::Identity;
	m_constBuffer.matView = Matrix::Identity;
	m_constBuffer.matProj = Matrix::Identity;

	m_constBuffer.count = Vector4(frameIndex);
	m_constBuffer.height = Vector4(frameRows);
	m_constBuffer.width = Vector4(frameCols);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertices);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}
