/*
	@file	Radar.cpp
	@brief	レーダークラス
*/
#include "pch.h"
#include "Radar.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Radar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
Radar::Radar(CommonResources* commonResources)
	: m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemies{ nullptr },
	m_radarPos{ 0.745f, -0.55f },
	m_playerSize{ 0.018f, -0.032f },
	m_enemySize{ 0.027f, -0.048f },
	m_time{ 0.0f },
	m_pDrawPolygon{ DrawPolygon::GetInstance() },
	m_pCreateShader{ CreateShader::GetInstance() }
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
Radar::~Radar()
{
	m_pDrawPolygon->ReleasePositionTexture();
}
void Radar::LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	textures.push_back(texture);
}

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
void Radar::Initialize(Player* pPlayer, Enemies* pEnemies)
{
	m_pPlayer = pPlayer;
	m_pEnemies = pEnemies;
	// 板ポリゴン描画準備
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());

	// 画像の読み込み 
	LoadTexture(L"Resources/Textures/RadarBack.png", m_backTextures);
	LoadTexture(L"Resources/Textures/PlayerPin.png", m_playerTextures);
	LoadTexture(L"Resources/Textures/EnemyPin.png", m_enemyTextures);
	// シェーダーを作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Radar/VS_Radar.cso", m_vertexShader); // 頂点シェーダ
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Radar/PS_Radar.cso", m_pixelShader); // ピクセルシェーダ
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}

//---------------------------------------------------------
// 更新
//---------------------------------------------------------
void Radar::Update(float elapsedTime)
{
	// 時間
	m_time += elapsedTime;
	// プレイヤーの位置を取得
	m_playerPos = m_pPlayer->GetPlayerPos();
	// 敵の位置をクリア
	m_enemyPos.clear();
	// 敵の位置を取得
	for (auto& enemy : m_pEnemies->GetEnemies())
	{
		Vector3 enemyPos = enemy->GetPosition();
		float distance = Vector3::Distance(m_playerPos, enemyPos);

		// レーダー範囲内にいる敵だけ保存
		if (distance <= m_range)
		{
			m_enemyPos.push_back(enemyPos);
		}
	}

}

//---------------------------------------------------------
// 描画
//---------------------------------------------------------
void Radar::Render()
{
	// レーダー描画
	DrawBackground();// 背景描画
	DrawPlayer();// プレイヤー描画
	DrawEnemy();// 敵描画
}


//---------------------------------------------------------
// 背景描画
// ---------------------------------------------------------
void Radar::DrawBackground()
{
	//	頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(0.5f, -.16f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0f, -.16f, 0.0f),SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0f, -1.0f, 0.0f),SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(0.5f, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),

	};
	// グラデーションエフェクトの色設定 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// バッファを作成
	CreateBuffer();
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_backTextures);
	// ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

//---------------------------------------------------------
// プレイヤー描画
//---------------------------------------------------------
void Radar::DrawPlayer()
{
	VertexPositionTexture playerVertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(m_radarPos.x, m_radarPos.y, 0.0f)										, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y, 0.0f)					, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y + m_playerSize.y, 0.0f)	, SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x, m_radarPos.y + m_playerSize.y, 0.0f)					, SimpleMath::Vector2(0.0f, 1.0f)),

	};
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// グラデーションエフェクトの色設定 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// バッファを作成
	CreateBuffer();
	// 描画前設定
	m_pDrawPolygon->DrawSetting(DrawPolygon::SamplerStates::LINEAR_WRAP, DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE, DrawPolygon::DepthStencilStates::DEPTH_NONE);

	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_playerTextures);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(playerVertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

//---------------------------------------------------------
// 敵描画
//---------------------------------------------------------
void Radar::DrawEnemy()
{
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// グラデーションエフェクトの色設定 
	m_constBuffer.colors = SimpleMath::Vector4(0.9f, 0.0f, 0.0f, 0);
	// バッファを作成
	CreateBuffer();
	// 敵の位置を描画	 
	for (const auto& enemyPos : m_enemyPos)
	{
		// プレイヤーから敵への相対位置を計算
		Vector3 relativePos = enemyPos - m_playerPos;
		// カメラの向きを取得
		Vector3 cameraDirection = m_pPlayer->GetPlayerDir(); // カメラの向き（前方ベクトル）

		// カメラの向きに基づいて回転行列を作成
		float playerRotation = atan2(cameraDirection.x, cameraDirection.z); // カメラの前方ベクトルから回転角を計算
		Matrix rotationMatrix = Matrix::CreateRotationZ(XMConvertToRadians(180));// 回転を調整
		rotationMatrix *= Matrix::CreateRotationY(playerRotation); // Y軸回転

		// 敵の位置をプレイヤーの位置を軸にして回転
		Vector3 rotatedPos = Vector3::Transform(relativePos, rotationMatrix);
		// レーダーの中心からの相対位置を計算
		Vector2 radarPos = Vector2(
			rotatedPos.x / m_range * (1.0f - 0.5f), // ミニマップの横幅
			rotatedPos.z / m_range * (1.0f - 0.16f) // ミニマップの高さ
		);
		radarPos += m_radarPos;

		// ミニマップの範囲内にいるかを確認
		if (radarPos.x > 0.5f + m_enemySize.x && radarPos.x < 1.0f + m_enemySize.x &&
			radarPos.y > -1.0f + m_enemySize.y && radarPos.y < -0.16f + m_enemySize.y)
		{
			// 描画前設定
			m_pDrawPolygon->DrawSetting(DrawPolygon::SamplerStates::LINEAR_WRAP, DrawPolygon::BlendStates::NONPREMULTIPLIED,
				DrawPolygon::RasterizerStates::CULL_NONE, DrawPolygon::DepthStencilStates::DEPTH_NONE);

			// 描画準備
			m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_enemyTextures);
			VertexPositionTexture enemyVertex[4] =
			{
				//	頂点情報													UV情報
				VertexPositionTexture(Vector3(radarPos.x, radarPos.y, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + 0.01f, radarPos.y, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + 0.01f, radarPos.y + 0.02f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
				VertexPositionTexture(Vector3(radarPos.x, radarPos.y + 0.02f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f))
			};
			// 板ポリゴンを描画
			m_pDrawPolygon->DrawTexture(enemyVertex);
		}
	}
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}


//---------------------------------------------------------
// バッファを作成
//---------------------------------------------------------
void Radar::CreateBuffer()
{
	// 時間設定
	m_constBuffer.time = SimpleMath::Vector4(m_time);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
}