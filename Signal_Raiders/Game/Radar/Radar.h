/*
*	@file	Radar.h
*	@brief	レーダークラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <memory>
// DirectXTK
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
// 前方宣言
class CommonResources;
class Player;
class EnemyManager;
class Enemy;
class EnemyAI;
class Radar
{
public:
	// レーダーの大きさの定義


	// シェーダーに渡す定数バッファの構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;      // 時間
	}m_constBuffer;
private:// 列挙型
	enum class RadarState// レーダーの状態
	{
		Background,	// 背景
		Player,		// プレイヤー
		Enemy,		// 敵
	};
public:// publicメンバ関数
	Radar(CommonResources* commonResources);// コンストラクタ
	~Radar();// デストラクタ
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);// 初期化
	void Update(float elapsedTime);// 更新
	void Render();// 描画
private:// privateメンバ関数
	void LoadTexture(const wchar_t* path);// テクスチャの読み込み
	void DrawBackground();// 背景を描画する
	void DrawPlayer();// プレイヤーを描画する
	void DrawEnemy();// 敵を描画する
	void CreateBuffer();// バッファを作成する
	void DrawSetting();// 描画前設定
public:// 定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// インプットレイアウト
	static const float RADAR_SIZE_L;//レーダーの左端
	static const float RADAR_SIZE_R;//レーダーの右端
	static const float RADAR_SIZE_T;//レーダーの上端
	static const float RADAR_SIZE_B;//レーダーの下端
	static const float ENEMY_SIZE_W;// 敵の点の幅
	static const float ENEMY_SIZE_H;// 敵の点の高さ
	static const float RADAR_POSITION_X;// レーダーの位置X
	static const float RADAR_POSITION_Y;// レーダーの位置Y
	static const float PLAYER_SIZE_X;// プレイヤーのXサイズ
	static const float PLAYER_SIZE_Y;// プレイヤーのYサイズ
	static const float ENEMY_SIZE_X;// 敵のXサイズ
	static const float ENEMY_SIZE_Y;// 敵のYサイズ
	static const float RADAR_RANGE;// レーダーの範囲
	static const float DISTANCE;// プレイヤーとの距離
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// プレイヤー
	Player* m_pPlayer;
	// 敵マネージャー
	EnemyManager* m_pEnemyManager;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// レーダーの範囲
	float m_range;
	// プレイヤーとの距離
	float m_distance;
	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPos;
	// 敵の位置
	std::vector<DirectX::SimpleMath::Vector3> m_enemyPos;
	// 敵の状態
	std::vector<bool> m_enemyState;
	// レーダー表示位置
	DirectX::SimpleMath::Vector2 m_radarPos;
	// プレイヤーのサイズ
	DirectX::SimpleMath::Vector2 m_playerSize;
	// 敵のサイズ
	DirectX::SimpleMath::Vector2 m_enemySize;
	// 時間
	float m_time;
	// テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;

};