/*
*	@file	Radar.h
*	@brief	レーダークラス
*/
#pragma once
#ifndef RADAR_DEFINED
#define RADAR_DEFINED
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

// レーダークラス
class Radar
{
public:
	// 構造体
	// シェーダーに渡す定数バッファの構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;      // 時間
	};
private:
	// 列挙型
	enum class RadarState// レーダーの状態
	{
		Background,	// 背景
		Player,		// プレイヤー
		Enemy,		// 敵
	};
public:
	// publicメンバ関数
	// コンストラクタ
	Radar(CommonResources* commonResources);
	// デストラクタ
	~Radar();
	// 初期化
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
private:
	// privateメンバ関数
	// 背景を描画する
	void DrawBackground();
	// プレイヤーを描画する
	void DrawPlayer();
	// 敵を描画する
	void DrawEnemy();
	// バッファを作成する
	void CreateBuffer();
	// 描画前設定
	void DrawSetting();
public:
	// 定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//レーダーの左端
	static const float RADAR_SIZE_L;
	//レーダーの右端
	static const float RADAR_SIZE_R;
	//レーダーの上端
	static const float RADAR_SIZE_T;
	//レーダーの下端
	static const float RADAR_SIZE_B;
	// 敵の点の幅
	static const float ENEMY_SIZE_W;
	// 敵の点の高さ
	static const float ENEMY_SIZE_H;
	// レーダーの位置X
	static const float RADAR_POSITION_X;
	// レーダーの位置Y
	static const float RADAR_POSITION_Y;
	// プレイヤーのXサイズ
	static const float PLAYER_SIZE_X;
	// プレイヤーのYサイズ
	static const float PLAYER_SIZE_Y;
	// 敵のXサイズ
	static const float ENEMY_SIZE_X;
	// 敵のYサイズ
	static const float ENEMY_SIZE_Y;
	// レーダーの範囲
	static const float RADAR_RANGE;
	// プレイヤーとの距離
	static const float DISTANCE;
private:
	// private変数
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
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// シェーダーに渡す定数バッファ
	ConstBuffer m_constBuffer;
	// テクスチャ配列
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
};
#endif // RADAR_DEFINED