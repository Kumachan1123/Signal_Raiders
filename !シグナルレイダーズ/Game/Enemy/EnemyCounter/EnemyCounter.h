/*
	@file	EnemyCounter.h
	@brief	敵カウントクラス
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
	// 「残り：」テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_remaining;
	// 「/」テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_slash;

	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	// 頂点情報
	// 総数の10の位
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex10[4];
	// 総数の1の位
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex1[4];
	// 現在の敵の数の10の位
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy10[4];
	// 現在の敵の数の1の位
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy1[4];
	// 「のこり：」
	DirectX::DX11::VertexPositionTexture m_verticesRemaining[4];
	// 「/」
	DirectX::DX11::VertexPositionTexture m_verticesSlash[4];

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

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
	void LoadTexture(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& tex);
	// 総数の10の位を描画
	void DrawEnemyIndex10();
	// 総数の1の位を描画
	void DrawEnemyIndex1();
	// 現在の敵の数の10の位を描画
	void DrawNowEnemy10();
	// 現在の敵の数の1の位を描画
	void DrawNowEnemy1();
	// 「のこり：」を描画
	void DrawRemaining();
	// 「/」を描画
	void DrawSlash();
};