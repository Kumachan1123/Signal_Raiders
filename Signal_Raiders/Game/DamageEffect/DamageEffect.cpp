//--------------------------------------------------------------------------------------
// File: DamageEffect.h
//
// 攻撃を受けた時の演出クラス
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "DamageEffect.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
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

/// <summary>
/// コンストラクタ
/// </summary>
DamageEffect::DamageEffect(CommonResources* resources)
	:m_pDR{ nullptr }
	, m_time{ 0.0f }
	, m_constBuffer{}
	, m_commonResources{ resources }
	, m_pPlayer{ nullptr }
	, m_enemyDirection{  }
	, m_playEffect{ false }
{
	// 色の初期化
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);
}

/// <summary>
/// デストラクタ
/// </summary>
DamageEffect::~DamageEffect()
{
	DrawPolygon::ReleasePositionTexture();
}

/// <summary>
/// 初期化関数
/// </summary>
void DamageEffect::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_playEffect = true;
}

/// <summary>
/// テクスチャリソース読み込み関数
/// </summary>
/// <param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
void  DamageEffect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}

/// <summary>
/// 生成関数
/// </summary>
/// <param name="pDR">ユーザーリソース等から持ってくる</param>
void  DamageEffect::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	// シェーダーの作成
	CreateShader();
	// 画像の読み込み（2枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/crisis.png");
	// 板ポリゴン描画用
	DrawPolygon::InitializePositionTexture(m_pDR);
}


/// <summary>
/// Shader作成部分だけ分離した関数
/// </summary>
void  DamageEffect::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	コンパイルされたシェーダファイルを読み込み
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/DamageEffect/VS_Damage.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/DamageEffect/PS_Damage.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);

	// シェーダーの構造体にセット
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}

// 更新
void  DamageEffect::Update(float elapsedTime)
{

	if (m_time >= PLAY_TIME)
	{
		m_time = 0.0f;
		m_constBuffer.colors.w = 0.0f;
		m_playEffect = false;
		m_pPlayer->SetisPlayEffect(m_playEffect);
		return;
	}
	// 時間更新
	m_time += elapsedTime;
	m_constBuffer.time = m_time;
	// アルファ値(sin関数を使って二秒以内に0から1を往復する)
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);
	// プレイヤーの向き
	m_playerDirection = m_pPlayer->GetPlayerDir();
	// プレイヤーの向きと敵の向きの差から0から360の間の角度を求める
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(m_playerDirection.x, m_playerDirection.z);
	// ラジアンから度に変換
	angle = angle * 180 / DirectX::XM_PI;
	// 0から360の間に収める
	if (angle < 0)angle += 360;

	// プレイヤーの向きによってUV座標を変える（16方向）
	if (angle >= 11.25 && angle < 33.75)           m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_H); // 右後1
	else if (angle >= 33.75 && angle < 56.25)      m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_W + UV_C) / 2, UV_H); // 右後2
	else if (angle >= 56.25 && angle < 78.75)      m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, (UV_H + UV_C) / 2); // 右後3
	else if (angle >= 78.75 && angle < 101.25)     m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_C); // 右
	else if (angle >= 101.25 && angle < 123.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, (UV_Y + UV_C) / 2); // 右前1
	else if (angle >= 123.75 && angle < 146.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_W + UV_C) / 2, UV_Y); // 右前2
	else if (angle >= 146.25 && angle < 168.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_Y); // 右前3
	else if (angle >= 168.75 && angle < 191.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_C, UV_Y); // 前
	else if (angle >= 191.25 && angle < 213.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_Y); // 左前3
	else if (angle >= 213.75 && angle < 236.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_X + UV_C) / 2, UV_Y); // 左前2
	else if (angle >= 236.25 && angle < 258.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, (UV_Y + UV_C) / 2); // 左前1
	else if (angle >= 258.75 && angle < 281.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_C); // 左
	else if (angle >= 281.25 && angle < 303.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, (UV_H + UV_C) / 2); // 左後1
	else if (angle >= 303.75 && angle < 326.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_X + UV_C) / 2, UV_H); // 左後2
	else if (angle >= 326.25 && angle < 348.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_H); // 左後3
	else                                           m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_C, UV_H); // 後
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void  DamageEffect::Render()
{

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionTexture vertex[4] =
	{
		// 頂点情報													UV情報
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	DrawPolygon::UpdateSubResources(context, m_cBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	// 描画準備
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_texture);
	// シェーダをセットする
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	DrawPolygon::DrawTexture(vertex);
	//	シェーダの登録を解除しておく
	DrawPolygon::ReleaseShader(context);
}