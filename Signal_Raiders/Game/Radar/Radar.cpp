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
	m_time{ 0.0f }

{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
Radar::~Radar()
{
	DrawPolygon::ReleasePositionTexture();
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
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_pPlayer = pPlayer;
	m_pEnemies = pEnemies;
	// 板ポリゴン描画準備
	DrawPolygon::InitializePositionTexture(m_commonResources->GetDeviceResources());

	// 画像の読み込み 
	LoadTexture(L"Resources/Textures/RadarBack.png", m_backTextures);
	LoadTexture(L"Resources/Textures/PlayerPin.png", m_playerTextures);
	LoadTexture(L"Resources/Textures/EnemyPin.png", m_enemyTextures);
	//	コンパイルされたシェーダファイルを読み込み
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Radar/VS_Radar.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Radar/PS_Radar.cso");
	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), VSData.GetData(), VSData.GetSize(), m_inputLayout.GetAddressOf());
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
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
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
	CreateBuffer(context);
	// 描画準備
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_backTextures);
	// ポリゴンを描画
	DrawPolygon::DrawTexture(vertex);
	// シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

//---------------------------------------------------------
// プレイヤー描画
//---------------------------------------------------------
void Radar::DrawPlayer()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
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
	CreateBuffer(context);
	// 描画準備
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_playerTextures);
	// 板ポリゴンを描画
	DrawPolygon::DrawTexture(playerVertex);
	// シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

//---------------------------------------------------------
// 敵描画
//---------------------------------------------------------
void Radar::DrawEnemy()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// グラデーションエフェクトの色設定 
	m_constBuffer.colors = SimpleMath::Vector4(0.9f, 0.0f, 0.0f, 0);
	// バッファを作成
	CreateBuffer(context);
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
			// 描画準備
			DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_enemyTextures);
			VertexPositionTexture enemyVertex[4] =
			{
				//	頂点情報													UV情報
				VertexPositionTexture(Vector3(radarPos.x, radarPos.y, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + 0.01f, radarPos.y, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + 0.01f, radarPos.y + 0.02f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
				VertexPositionTexture(Vector3(radarPos.x, radarPos.y + 0.02f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f))
			};
			// 板ポリゴンを描画
			DrawPolygon::DrawTexture(enemyVertex);
		}
	}
	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}


//---------------------------------------------------------
// バッファを作成
//---------------------------------------------------------
void Radar::CreateBuffer(ID3D11DeviceContext1* context)
{
	// 時間設定
	m_constBuffer.time = SimpleMath::Vector4(m_time);
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	DrawPolygon::UpdateSubResources(context, m_cBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	// シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}