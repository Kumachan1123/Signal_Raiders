/*
	@file	EnemyCounter.h
	@brief	敵カウントクラス
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
#include "Game/KumachiLib/BinaryFile.h"
#include <memory>

class CommonResources;
class EnemyCounter
{
private:
	CommonResources* m_commonResources;
	// 敵の総数(保存用）
	int m_enemyIndex;
	// 現在の敵の数(保存用）
	int m_nowEnemy;

	// 敵の総数(10の位)
	int m_enemyIndex10;
	// 敵の総数(1の位)
	int m_enemyIndex1;
	// 現在の敵の数(10の位)
	int m_nowEnemy10;
	// 現在の敵の数(1の位)
	int m_nowEnemy1;
	int m_frameRows = 1; // 画像の行数
	int m_frameCols = 10; // 画像の列数
	// 数字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	// 頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[4];
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
	// 板ポリゴンの頂点座標
	float m_vertexMinX;
	float m_vertexMaxX;
	float m_vertexMinY;
	float m_vertexMaxY;
public:
	// コンストラクタ
	EnemyCounter();
	// デストラクタ
	~EnemyCounter();
	// 初期化する
	void Initialize(CommonResources* commonResources);
	// 更新する
	void Update(float elapsedTime);
	// 描画する
	void Render();
	void SetEnemyIndex(int enemyIndex) { m_enemyIndex = enemyIndex; }
	// 現在の敵の数を設定する
	void SetNowEnemy(int nowEnemy) { m_nowEnemy = nowEnemy; }
private:
	void LoadTexture(const wchar_t* path);
};