/*
	@file	Effect.cpp
	@brief	エフェクトクラス
*/
#include "pch.h"
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Effect.h"



using namespace DirectX;
using namespace DirectX::SimpleMath;

const std::vector<D3D11_INPUT_ELEMENT_DESC>  Effect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


Effect::Effect(CommonResources* resources, ParticleType type, DirectX::SimpleMath::Vector3 playPos, float scale, DirectX::SimpleMath::Matrix world)
	: m_position{ playPos }
	, m_type{ type }
	, m_scale{ scale }
	, m_commonResources{ resources }
	, m_world{ world }
	, m_isPlaying{ true }
	, m_anim{ 0 }
	, m_animSpeed{ 30.0f }
	, m_elapsedTime{ 0.0f }
	, m_frameRows{}
	, m_frameCols{}
	, m_vertices{}
	, m_offSetY{ 0.0f }
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }

{
	switch (m_type)// エフェクトの種類によって画像を読み込む
	{
	case ParticleType::ENEMY_DEAD:// 敵死亡エフェクト
		LoadTexture(L"Resources/Textures/effect.png");
		m_frameRows = 4; // 画像の行数
		m_frameCols = 5; // 画像の列数
		m_vertexMinX = -1.0f;
		m_vertexMaxX = 1.0f;
		m_vertexMinY = -1.0f;
		m_vertexMaxY = 1.0f;
		break;
	case ParticleType::ENEMY_HIT:// 敵ヒットエフェクト
		LoadTexture(L"Resources/Textures/hit.png");
		m_frameRows = 2; // 画像の行数
		m_frameCols = 4; // 画像の列数
		m_vertexMinX = -1.0f;
		m_vertexMaxX = 1.0f;
		m_vertexMinY = -1.0f;
		m_vertexMaxY = 1.0f;
		break;

	}
	// 板ポリゴン描画用クラスの初期化
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
	// アニメーション再生中
	m_isPlaying = true;
}

//---------------------------------------------------------
// 画像を読み込む
//---------------------------------------------------------
void Effect::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	m_textures.push_back(texture);
}
Effect::~Effect() {}

void Effect::Update(float elapsedTime)
{
	m_elapsedTime += elapsedTime * m_animSpeed;
	if (m_elapsedTime >= 1.0f)// 1秒経過
	{
		m_anim++;// アニメのコマを薦める
		m_elapsedTime = 0.0f;// タイマーをリセット
	}
	if (m_anim == m_frameRows * m_frameCols)// アニメーション終了
	{
		m_isPlaying = false;// 再生終了
	}
}

void Effect::Render(ID3D11DeviceContext1* context, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// 頂点情報
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(0, 1)) };
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(1, 1)) };
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX,m_vertexMinY, 0), SimpleMath::Vector2(1, 0)) };
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX,m_vertexMinY, 0), SimpleMath::Vector2(0, 0)) };
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
	m_constBuffer.matWorld = worldBillboard.Transpose();
	m_constBuffer.matView = view.Transpose();
	m_constBuffer.matProj = proj.Transpose();
	m_constBuffer.count = Vector4(m_anim);
	m_constBuffer.height = Vector4(m_frameRows);
	m_constBuffer.width = Vector4(m_frameCols);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::ANISOTROPIC_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(billboardVertex);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

void Effect::Finalize()
{

}
