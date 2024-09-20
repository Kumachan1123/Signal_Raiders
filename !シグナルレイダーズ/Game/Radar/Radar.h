/*
	@file	Radar.h
	@brief	レーダークラス
	作成者：くまち
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

#include <memory>
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
class CommonResources;
class Player;
class Enemies;
class Enemy;
class EnemyAI;
class Radar
{
	// 変数
	CommonResources* m_commonResources;
	Player* m_pPlayer;
	Enemies* m_pEnemies;
	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	float m_range = 400.0f;// レーダーの範囲
	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPos;
	// 敵の位置
	std::vector<DirectX::SimpleMath::Vector3> m_enemyPos;
	// 敵の状態
	std::vector<bool> m_enemyState;

	// レーダー表示位置とサイズ
	DirectX::SimpleMath::Vector2 m_radarPos;
	DirectX::SimpleMath::Vector2 m_radarSize;
public:
	// コンストラクタ
	Radar(CommonResources* commonResources);
	// デストラクタ
	~Radar();
	// 初期化
	void Initialize(Player* pPlayer, Enemies* pEnemies);
	// 更新
	void Update();
	// 描画
	void Render();



};