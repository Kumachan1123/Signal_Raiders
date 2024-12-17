/*
	@file	Radar.h
	@brief	レーダークラス
*/
#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include <memory>
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
class CommonResources;
class Player;
class Enemies;
class Enemy;
class EnemyAI;
class Radar
{
public:
	// データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;      // 時間
	}m_constBuffer;
private:
	enum class RadarState
	{
		Background,
		Player,
		Enemy,
	};
	// 変数
	CommonResources* m_commonResources;
	Player* m_pPlayer;
	Enemies* m_pEnemies;
	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;


	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
	float m_range = 300.0f;// レーダーの範囲
	// プレイヤーとの距離
	float m_distance = 125.0f;
	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPos;
	// 敵の位置
	std::vector<DirectX::SimpleMath::Vector3> m_enemyPos;
	// 敵の状態
	std::vector<bool> m_enemyState;

	// レーダー表示位置とプレイヤーのサイズ
	DirectX::SimpleMath::Vector2 m_radarPos;
	DirectX::SimpleMath::Vector2 m_playerSize;
	// 敵のサイズ
	DirectX::SimpleMath::Vector2 m_enemySize;
	// 時間
	float m_time;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_enemyTexture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_backTextures;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_playerTextures;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_enemyTextures;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// コンストラクタ
	Radar(CommonResources* commonResources);
	// デストラクタ
	~Radar();
	// 初期化
	void Initialize(Player* pPlayer, Enemies* pEnemies);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();

	// テクスチャの読み込み
	void LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures);

	void DrawBackground();// 背景を描画する
	void DrawPlayer();// プレイヤーを描画する
	void DrawEnemy();// 敵を描画する

	void CreateBuffer(ID3D11DeviceContext1* context);// バッファを作成する

};