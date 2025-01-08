/*
	@file	EnemyHPBar.cpp
	@brief	敵HPBarクラス
*/
#include "pch.h"
#include "EnemyHPBar.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyHPBar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
EnemyHPBar::EnemyHPBar()
	: m_commonResources{}
	, m_maxHP(100)
	, m_currentHP(100)
	, m_displayedHP(100)
	, m_lerpSpeed(5.0f)
	, m_scale(1.5f)
	, m_time{ 0.0f }
	, m_isDead(false)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	// ビルボードの頂点情報を設定する
	m_hpbarVert[0] = { Vector3(-1.5f,3.25f,0.0f),Vector2(0.0f,0.0f) };	// 左上
	m_hpbarVert[1] = { Vector3(1.5f,3.25f,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarVert[2] = { Vector3(1.5f,3.0f,0.0f),Vector2(1.0f,1.0f) };	// 右下
	m_hpbarVert[3] = { Vector3(-1.5f,3.0f,0.0f),Vector2(0.0f,1.0f) };	// 左下
	// ビルボードの頂点情報を設定する
	m_hpbarBackVert[0] = { Vector3(-1.60f,3.27f,0.0f),Vector2(0.0f,0.0f) };	// 左上
	m_hpbarBackVert[1] = { Vector3(1.60f,3.27f,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarBackVert[2] = { Vector3(1.60f,2.98f,0.0f),Vector2(1.0f,1.0f) };	// 右下
	m_hpbarBackVert[3] = { Vector3(-1.60f,2.98f,0.0f),Vector2(0.0f,1.0f) };	// 左下

}
// デストラクタ
EnemyHPBar::~EnemyHPBar() {}

//---------------------------------------------------------
// 画像を読み込む
//---------------------------------------------------------
void EnemyHPBar::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	m_gaugeTexture.push_back(texture);
}



void EnemyHPBar::CreateShader()
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 頂点シェーダーとピクセルシェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/EnemyHP/VS_EnemyHP.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/EnemyHP/PS_EnemyHP.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}


//---------------------------------------------------------
//// 初期化する
////---------------------------------------------------------
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	// 板ポリゴン描画用クラスの初期化
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());
	// シェーダーの作成
	CreateShader();
	LoadTexture(L"Resources/Textures/EnemyHPBar.png");// テクスチャ読み込み
	m_displayedHP = (float)(m_maxHP);// 初期HP
}
// 描画
void EnemyHPBar::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// ビルボード行列を作成
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;

	// ビルボードをアフィン変換
	Matrix worldBillboard{};
	worldBillboard = billboardMatrix;
	worldBillboard *= Matrix::CreateScale(m_scale);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));	// ③
	worldBillboard *= Matrix::CreateTranslation(pos);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));	// ③
	// ビュー設定
	m_constBuffer.matView = view.Transpose();
	// プロジェクション設定
	m_constBuffer.matProj = proj.Transpose();
	// ワールド設定
	m_constBuffer.matWorld = worldBillboard.Transpose();
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// グラデーションエフェクトの色設定 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	CreateBuffer();

	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_gaugeTexture);
	// ポリゴンを描画
	m_pDrawPolygon->DrawTexture(m_hpbarBackVert);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();

	// HPbar(緑)描画///////////////////////////////////////////////////////////////////////////////
	// 色をへんこう 
	m_constBuffer.colors = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	CreateBuffer();

	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_gaugeTexture);
	// ポリゴンを描画
	m_pDrawPolygon->DrawTexture(m_hpbarVert);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();

	//// ビルボードを描画
	//m_primitiveBatch->Begin();
	//m_primitiveBatch->DrawQuad(m_hpbarVert[0], m_hpbarVert[1], m_hpbarVert[3], m_hpbarVert[2]);
	//m_primitiveBatch->End();
}
// 更新
void EnemyHPBar::Update(float elapsedTime)
{
	// 時間
	m_time += elapsedTime;


	// HPを滑らかに更新するための線形補間
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);

	// 表示HPに基づいてHP割合を計算
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);

	// HPバーの幅を計算 
	float maxBarWidth = 3.0f;
	float currentBarWidth = maxBarWidth * hpPercentage;

	// HPバーの頂点を設定 (バーの左端を -1.50f として、右端を 1.50f にする)
	m_hpbarVert[0].position.x = -1.50f;                    // 左上
	m_hpbarVert[1].position.x = -1.50f + currentBarWidth;  // 右上
	m_hpbarVert[2].position.x = -1.50f + currentBarWidth;  // 右下
	m_hpbarVert[3].position.x = -1.50f;                    // 左下

	if (m_hpbarVert[1].position.x <= -1.50f)
	{
		m_isDead = true;
	}

}

void EnemyHPBar::CreateBuffer()
{
	// 時間設定
	// 時間設定
	m_constBuffer.hp = SimpleMath::Vector4(float(m_currentHP), float(m_maxHP), 0, 0);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
}