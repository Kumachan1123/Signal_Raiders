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
// レーダーの左端
const float Radar::RADAR_SIZE_L = 0.5f;
// レーダーの右端
const float Radar::RADAR_SIZE_R = 1.0f;
// レーダーの上端
const float Radar::RADAR_SIZE_T = 0.16f;
// レーダーの下端
const float Radar::RADAR_SIZE_B = 1.0f;
// 敵の点の幅
const float Radar::ENEMY_SIZE_W = 0.01f;
// 敵の点の高さ
const float Radar::ENEMY_SIZE_H = 0.02f;
// レーダーの位置X
const float Radar::RADAR_POSITION_X = 0.745f;
// レーダーの位置Y
const float Radar::RADAR_POSITION_Y = -0.55f;
// プレイヤーのXサイズ
const float Radar::PLAYER_SIZE_X = 0.018f;
// プレイヤーのYサイズ
const float Radar::PLAYER_SIZE_Y = -0.032f;
// 敵のXサイズ
const float Radar::ENEMY_SIZE_X = 0.027f;
// 敵のYサイズ
const float Radar::ENEMY_SIZE_Y = -0.048f;
// レーダーの範囲
const float Radar::RADAR_RANGE = 300.0f;
// プレイヤーとの距離
const float Radar::DISTANCE = 125.0f;
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
*	@brief	初期化
*	@details レーダークラスの初期化
*	@param	pPlayer	プレイヤー
*	@param	pEnemyManager	敵マネージャー
*	@return なし
*/
void Radar::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	// プレイヤーのポインターを渡す
	m_pPlayer = pPlayer;
	// 敵マネージャーのポインターを渡す
	m_pEnemyManager = pEnemyManager;
	// 板ポリゴン描画準備
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// デバイスを取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(device, &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 頂点シェーダ
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Radar/VS_Radar.cso", m_pVertexShader);
	// ピクセルシェーダ
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Radar/PS_Radar.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーを渡す
	m_shaders.ps = m_pPixelShader.Get();
	// ジオメトリシェーダーは使わないのでnullptrを設定
	m_shaders.gs = nullptr;
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
	// 時間
	m_time += elapsedTime;
	// プレイヤーの位置を取得
	m_playerPos = m_pPlayer->GetPlayerPos();
	// 敵の位置をクリア
	m_enemyPos.clear();
	// 登録した敵の分だけループ
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		// 敵の位置を取得
		Vector3 enemyPos = enemy->GetPosition();
		// プレイヤーとの距離を計算
		float distance = Vector3::Distance(m_playerPos, enemyPos);
		// レーダー範囲内にいる敵だけ保存
		if (distance <= m_range)
		{
			// 敵の位置を保存
			m_enemyPos.push_back(enemyPos);
		}
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
	// 背景描画
	DrawBackground();
	// プレイヤーピン描画
	DrawPlayer();
	// 敵ピン描画
	DrawEnemy();
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
	// レーダーのサイズを設定
	VertexPositionTexture vertex[4] =
	{
		//										頂点情報								UV情報
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),// 左下

	};
	// グラデーションエフェクトの色設定(やや暗めの緑)
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// バッファを作成
	CreateBuffer();
	// 描画前設定
	DrawSetting();
	// 画像引き渡し
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
	// 背景画像を取得
	textures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("RadarBack"));
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);
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
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// プレイヤーピンの描画位置を設定
	VertexPositionTexture playerVertex[4] =
	{
		//														頂点情報													UV情報
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(0.0f, 1.0f)),

	};
	// グラデーションエフェクトの色設定(暗めの緑） 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// バッファを作成
	CreateBuffer();
	// 描画前設定
	DrawSetting();
	// 画像引き渡し
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
	// プレイヤーピン画像を取得
	textures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("PlayerPin"));
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(playerVertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
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
	// グラデーションエフェクトの色設定(赤)
	m_constBuffer.colors = Vector4(0.9f, 0.0f, 0.0f, 0);
	// バッファを作成
	CreateBuffer();
	// 敵の位置を描画	 
	for (const auto& enemyPos : m_enemyPos)
	{
		// プレイヤーから敵への相対位置を計算
		Vector3 relativePos = enemyPos - m_playerPos;
		// カメラの向きを取得
		Vector3 cameraDirection = m_pPlayer->GetPlayerDir();
		// カメラの向きに基づいて回転行列を作成
		// カメラの前方ベクトルから回転角を計算
		float playerRotation = atan2(cameraDirection.x, cameraDirection.z);
		// 回転を調整
		Matrix rotationMatrix = Matrix::CreateRotationZ(XMConvertToRadians(180));
		// Y軸回転を追加
		rotationMatrix *= Matrix::CreateRotationY(playerRotation);
		// 敵の位置をプレイヤーの位置を軸にして回転
		Vector3 rotatedPos = Vector3::Transform(relativePos, rotationMatrix);
		// レーダーの中心からの相対位置を計算
		Vector2 radarPos = Vector2(
			rotatedPos.x / m_range * (RADAR_SIZE_L - RADAR_SIZE_R), // ミニマップの横幅
			rotatedPos.z / m_range * (RADAR_SIZE_B - RADAR_SIZE_T) // ミニマップの高さ
		);
		// ミニマップの中心位置を加算
		radarPos += m_radarPos;
		// ミニマップの範囲内にいるかを確認し、範囲内なら描画
		if (radarPos.x > RADAR_SIZE_L + m_enemySize.x && radarPos.x < RADAR_SIZE_R + m_enemySize.x &&
			radarPos.y > -RADAR_SIZE_B + m_enemySize.y && radarPos.y < -RADAR_SIZE_T + m_enemySize.y)
		{
			// 描画前設定
			DrawSetting();
			// 画像引き渡し
			std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
			// 敵ピン画像を取得
			textures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("EnemyPin"));
			// 描画準備
			m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);
			// 敵ピンの描画位置を設定
			VertexPositionTexture enemyVertex[4] =
			{
				//	 　 　　　　　　　　　　　　　　　　　　　　頂点情報													UV情報
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y,					 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y,					 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(0.0f, 1.0f))
			};
			// 板ポリゴンを描画
			m_pDrawPolygon->DrawTexture(enemyVertex);
		}
	}
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
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
	// 時間設定
	m_constBuffer.time = Vector4(m_time);
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
}
/*
*	@brief	描画前設定
*	@details レーダークラスの描画前設定
*	@param	なし
*	@return なし
*/
void Radar::DrawSetting()
{
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// デプスステンシルステート
}
