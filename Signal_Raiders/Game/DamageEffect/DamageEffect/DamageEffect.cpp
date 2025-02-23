//--------------------------------------------------------------------------------------
// File: DamageEffect.h
//
// 攻撃を受けた時の演出クラス
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "DamageEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC>  DamageEffect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
DamageEffect::DamageEffect(CommonResources* resources)
	: m_commonResources{ resources }
	, m_time{ 0.0f }
	, m_constBuffer{}
	, m_pDR{ resources->GetDeviceResources() }
	, m_pPlayer{ nullptr }
	, m_enemyDirection{  }
	, m_playEffect{ false }
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	// 色の初期化
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}

// デストラクタ
DamageEffect::~DamageEffect()
{
}

// 初期化
void DamageEffect::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_playEffect = true;
	// シェーダーの作成
	CreateShader();
	// 画像の読み込み（2枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/crisis.png");
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
	// 撃ってきた敵の向きを取得
	m_enemyDirection = m_pPlayer->GetEnemyBulletDirection();
}

// テクスチャリソース読み込み関数
void  DamageEffect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}



// シェーダー作成部分
void  DamageEffect::CreateShader()
{
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/DamageEffect/VS_Damage.cso", m_vertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/DamageEffect/PS_Damage.cso", m_pixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にセット
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}



// 更新
void  DamageEffect::Update(float elapsedTime)
{
	if (!m_playEffect)return;
	// 時間更新
	m_time += elapsedTime;
	if (m_time >= PLAY_TIME)
	{
		m_time = 0.0f;
		m_constBuffer.colors.w = 0.0f;
		m_playEffect = false;
		m_pPlayer->SetisPlayEffect(m_playEffect);
		return;
	}
	m_constBuffer.time = Vector4(m_time);
	// アルファ値(sin関数を使って二秒以内に0から1を往復する)
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);
	// 角度を計算
	float angle = CalculateAngle();
	m_constBuffer.uv = GetUVFromAngle(angle);
}
// 角度計算
float DamageEffect::CalculateAngle() const
{
	Vector3 playerDir = m_pPlayer->GetPlayerDir();// プレイヤーの向き
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(playerDir.x, playerDir.z);// プレイヤーと敵の向きの差
	angle = DirectX::XMConvertToDegrees(angle);// ラジアンから度に変換
	return (angle < 0) ? angle + 360 : angle;// 0から360の間に収める
}
// 角度からUV座標を求める
DirectX::SimpleMath::Vector4 DamageEffect::GetUVFromAngle(float angle) const
{
	// 角度（degree）をラジアンに変換
	float rad = DirectX::XMConvertToRadians(angle);

	// 半径は左右または上下の中心からの距離
	float radius = UV_W - UV_C;

	// 座標を計算
	float u = UV_C - radius * -sin(rad);
	float v = UV_C + radius * cos(rad);

	return DirectX::SimpleMath::Vector4(u, v, 0, 0);
}
// 描画
void  DamageEffect::Render()
{

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionTexture vertex[4] =
	{
		// 頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),  SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),  SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}