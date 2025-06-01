/*
*	@file	Radar.cpp
*	@brief	レーダークラス
*/
#include <pch.h>
#include "Radar.h"
/*
*	@brief	インプットレイアウト
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Radar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const float Radar::RADAR_SIZE_L = 0.5f;	// レーダーの左端
const float Radar::RADAR_SIZE_R = 1.0f;	// レーダーの右端
const float Radar::RADAR_SIZE_T = 0.16f;	// レーダーの上端
const float Radar::RADAR_SIZE_B = 1.0f;	// レーダーの下端
const float Radar::ENEMY_SIZE_W = 0.01f;	// 敵の点の幅
const float Radar::ENEMY_SIZE_H = 0.02f;	// 敵の点の高さ
const float Radar::RADAR_POSITION_X = 0.745f;	// レーダーの位置X
const float Radar::RADAR_POSITION_Y = -0.55f;	// レーダーの位置Y
const float Radar::PLAYER_SIZE_X = 0.018f;	// プレイヤーのXサイズ
const float Radar::PLAYER_SIZE_Y = -0.032f;	// プレイヤーのYサイズ
const float Radar::ENEMY_SIZE_X = 0.027f;	// 敵のXサイズ
const float Radar::ENEMY_SIZE_Y = -0.048f;	// 敵のYサイズ
const float Radar::RADAR_RANGE = 300.0f;	// レーダーの範囲
const float Radar::DISTANCE = 125.0f;	// プレイヤーとの距離
/*
*	@brief	コンストラクタ
*	@details レーダークラスのコンストラクタ
*	@param	commonResources	共通リソース
*	@return なし
*/
Radar::Radar(CommonResources* commonResources)
	: m_pCommonResources{ commonResources }// 共通リソース
	, m_pPlayer{ nullptr }// プレイヤー
	, m_pEnemyManager{ nullptr }// 敵マネージャー
	, m_radarPos{ Radar::RADAR_POSITION_X , Radar::RADAR_POSITION_Y }// レーダーの位置
	, m_playerSize{ Radar::PLAYER_SIZE_X,Radar::PLAYER_SIZE_Y }// プレイヤーのサイズ
	, m_enemySize{ Radar::ENEMY_SIZE_X,Radar::ENEMY_SIZE_Y }// 敵のサイズ
	, m_range{ Radar::RADAR_RANGE }// レーダーの範囲
	, m_distance{ Radar::DISTANCE }// プレイヤーとの距離
	, m_time{ 0.0f }// 時間							
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // シェーダー作成クラスの初期化
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief	デストラクタ
*	@details レーダークラスのデストラクタ
*	@param	なし
*	@return なし
*/
Radar::~Radar()
{
	m_pDrawPolygon->ReleasePositionTexture();// 描画クラスの解放
}
/*
*	@brief	テクスチャの読み込み
*	@details テクスチャの読み込み
*	@param	path	テクスチャのパス
*	@return なし
*/
void Radar::LoadTexture(const wchar_t* path)
{
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャを格納するポインタ
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャを読み込む
	m_pTextures.push_back(texture);// テクスチャを格納
}
/*
*	@brief	初期化
*	@details レーダークラスの初期化
*	@param	pPlayer	プレイヤー
*	@param	pEnemyManager	敵マネージャー
*	@return なし
*/
void Radar::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	m_pPlayer = pPlayer;// プレイヤーのポインターを渡す
	m_pEnemyManager = pEnemyManager;// 敵マネージャーのポインターを渡す
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());// 板ポリゴン描画準備
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("RadarBack"));// 背景
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("PlayerPin"));// プレイヤーのピン
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("EnemyPin"));// 敵のピン
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Radar/VS_Radar.cso", m_pVertexShader); // 頂点シェーダ
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Radar/PS_Radar.cso", m_pPixelShader); // ピクセルシェーダ
	m_pInputLayout = m_pCreateShader->GetInputLayout();// インプットレイアウトを受け取る
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// シェーダーにデータを渡すためのコンスタントバッファ生成
	// シェーダーの構造体にシェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();// 頂点シェーダー
	m_shaders.ps = m_pPixelShader.Get();// ピクセルシェーダー
	m_shaders.gs = nullptr;// ジオメトリシェーダー(使わないのでnullptr)
}
/*
*	@brief	更新
*	@details レーダークラスの更新
*	@param	elapsedTime	経過時間
*	@return なし
*/
void Radar::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// 時間
	m_playerPos = m_pPlayer->GetPlayerPos();// プレイヤーの位置を取得
	m_enemyPos.clear();// 敵の位置をクリア
	for (auto& enemy : m_pEnemyManager->GetEnemies())// 敵の位置を取得
	{
		Vector3 enemyPos = enemy->GetPosition();// 敵の位置を取得
		float distance = Vector3::Distance(m_playerPos, enemyPos); // プレイヤーとの距離を計算
		if (distance <= m_range)// レーダー範囲内にいる敵だけ保存
			m_enemyPos.push_back(enemyPos);// 敵の位置を保存
	}
}
/*
*	@brief	描画
*	@details レーダークラスの描画
*	@param	なし
*	@return なし
*/
void Radar::Render()
{
	// レーダー描画
	DrawBackground();// 背景描画
	DrawPlayer();// プレイヤー描画
	DrawEnemy();// 敵描画
}
/*
*	@brief	背景描画
*	@details レーダークラスの背景描画
*	@param	なし
*	@return なし
*/
void Radar::DrawBackground()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =
	{
		//										頂点情報								UV情報
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),// 左下

	};
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);// グラデーションエフェクトの色設定(やや暗めの緑)
	CreateBuffer();// バッファを作成
	DrawSetting();// 描画前設定
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;	// 画像引き渡し
	textures.push_back(m_pTextures[(int)(RadarState::Background)].Get());// 背景画像
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);// 描画準備
	m_pDrawPolygon->DrawTexture(vertex);// ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダの登録を解除しておく
}

//---------------------------------------------------------
// プレイヤー描画
//---------------------------------------------------------
void Radar::DrawPlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture playerVertex[4] =
	{
		//														頂点情報													UV情報
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(0.0f, 1.0f)),

	};
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);// グラデーションエフェクトの色設定(暗めの緑） 
	CreateBuffer();// バッファを作成
	DrawSetting();// 描画前設定
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;// 画像引き渡し
	textures.push_back(m_pTextures[(int)(RadarState::Player)].Get());// プレイヤー画像
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);// 描画準備
	m_pDrawPolygon->DrawTexture(playerVertex);// 板ポリゴンを描画
	m_pDrawPolygon->ReleaseShader();// シェーダの登録を解除しておく
}
/*
*	@brief	敵描画
*	@details レーダークラスの敵描画
*	@param	なし
*	@return なし
*/
void Radar::DrawEnemy()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_constBuffer.colors = Vector4(0.9f, 0.0f, 0.0f, 0);// グラデーションエフェクトの色設定(赤)
	CreateBuffer();// バッファを作成
	for (const auto& enemyPos : m_enemyPos)	// 敵の位置を描画	 
	{
		Vector3 relativePos = enemyPos - m_playerPos;// プレイヤーから敵への相対位置を計算
		Vector3 cameraDirection = m_pPlayer->GetPlayerDir();// カメラの向きを取得
		// カメラの向きに基づいて回転行列を作成
		float playerRotation = atan2(cameraDirection.x, cameraDirection.z); // カメラの前方ベクトルから回転角を計算
		Matrix rotationMatrix = Matrix::CreateRotationZ(XMConvertToRadians(180));// 回転を調整
		rotationMatrix *= Matrix::CreateRotationY(playerRotation); // Y軸回転
		Vector3 rotatedPos = Vector3::Transform(relativePos, rotationMatrix);// 敵の位置をプレイヤーの位置を軸にして回転
		Vector2 radarPos = Vector2(// レーダーの中心からの相対位置を計算
			rotatedPos.x / m_range * (RADAR_SIZE_L - RADAR_SIZE_R), // ミニマップの横幅
			rotatedPos.z / m_range * (RADAR_SIZE_B - RADAR_SIZE_T) // ミニマップの高さ
		);
		radarPos += m_radarPos; // ミニマップの中心位置を加算
		// ミニマップの範囲内にいるかを確認
		if (radarPos.x > RADAR_SIZE_L + m_enemySize.x && radarPos.x < RADAR_SIZE_R + m_enemySize.x &&
			radarPos.y > -RADAR_SIZE_B + m_enemySize.y && radarPos.y < -RADAR_SIZE_T + m_enemySize.y)
		{
			DrawSetting();// 描画前設定
			std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;// 画像引き渡し
			textures.push_back(m_pTextures[(int)(RadarState::Enemy)].Get());// 敵画像
			m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);	// 描画準備
			VertexPositionTexture enemyVertex[4] =
			{
				//	 　 　　　　　　　　　　　　　　　　　　　　頂点情報													UV情報
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y,					 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y,					 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(0.0f, 1.0f))
			};
			m_pDrawPolygon->DrawTexture(enemyVertex);// 板ポリゴンを描画
		}
	}
	m_pDrawPolygon->ReleaseShader();//	シェーダの登録を解除しておく
}
/*
*	@brief	バッファを作成する
*	@details レーダークラスのバッファを作成する
*	@param	なし
*	@return なし
*/
void Radar::CreateBuffer()
{
	using namespace DirectX::SimpleMath;
	m_constBuffer.time = Vector4(m_time);// 時間設定
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// 受け渡し用バッファの内容更新
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// シェーダをセットする
}
/*
*	@brief	描画前設定
*	@details レーダークラスの描画前設定
*	@param	なし
*	@return なし
*/
void Radar::DrawSetting()
{
	m_pDrawPolygon->DrawSetting(// 描画前設定
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// デプスステンシルステート
}
