/*
*	@file	EnemyCounter.h
*	@brief	敵カウントクラス
*/
#pragma once
#ifndef ENEMY_COUNTER_DEFINED
#define ENEMY_COUNTER_DEFINED
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"

// 前方宣言
class CommonResources;

// 敵カウントクラス
class EnemyCounter
{
private:
	// private構造体
	// シェーダーに渡す定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅
	};
public:
	// public関数
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
	// 敵の総数を設定する
	void SetEnemyIndex(int enemyIndex) { m_enemyIndex = enemyIndex; }
	// 現在の敵の数を設定する
	void SetNowEnemy(int nowEnemy) { m_nowEnemy = nowEnemy; }
private:
	// private関数
	// 総数の10の位を描画
	void DrawEnemyIndex10();
	// 総数の1の位を描画
	void DrawEnemyIndex1();
	// 現在の敵の数の10の位を描画
	void DrawNowEnemy10();
	// 現在の敵の数の1の位を描画
	void DrawNowEnemy1();
	// 画像を表示
	void DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
		DirectX::VertexPositionTexture* vertices,
		float startX, float startY, float width, float height,
		int frameIndex, int frameCols, int frameRows);
public:
	// public定数
	// 頂点数
	static const int VERTEX_COUNT = 4;
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 各画像の描画位置とサイズ
	// 「残り：」の描画位置・サイズ
	const float REMAINING_POS_X = 0.35f;// 残りの表示位置X
	const float REMAINING_POS_Y = 1.0f;// 残りの表示位置Y
	const float REMAINING_SIZE_X = 0.25f;// 残りの表示サイズX
	const float REMAINING_SIZE_Y = 0.18f;//	残りの表示サイズY
	// 「/」の描画位置・サイズ
	const float SLASH_POS_X = 0.8f;// 「/」の表示位置X
	const float SLASH_POS_Y = 1.0f;// 「/」の表示位置Y
	const float SLASH_SIZE_X = 0.05f;// 「/」の表示サイズX
	const float SLASH_SIZE_Y = 0.18f;// 「/」の表示サイズY
	// 現在の敵の数の1の位・10の位の描画位置・サイズ
	const float NOW_ENEMY_10_POS_X = 0.66f;// 現在の敵の数の10の位の表示位置X
	const float NOW_ENEMY_10_POS_Y = 1.0f;// 現在の敵の数の10の位の表示位置Y
	const float NOW_ENEMY_1_POS_X = 0.72f;// 現在の敵の数の1の位の表示位置X
	const float NOW_ENEMY_1_POS_Y = 1.0f;// 現在の敵の数の1の位の表示位置Y
	const float NOW_ENEMY_SIZE_X = 0.08f;// 現在の敵の数の表示サイズX
	const float NOW_ENEMY_SIZE_Y = 0.18f;// 現在の敵の数の表示サイズY
	// 敵の総数の1の位・10の位の描画位置・サイズ
	const float ENEMY_INDEX_10_POS_X = 0.86f;// 敵の総数の10の位の表示位置X
	const float ENEMY_INDEX_10_POS_Y = 1.0f;// 敵の総数の10の位の表示位置Y
	const float ENEMY_INDEX_1_POS_X = 0.92f;// 敵の総数の1の位の表示位置X
	const float ENEMY_INDEX_1_POS_Y = 1.0f;// 敵の総数の1の位の表示位置Y
	const float ENEMY_INDEX_SIZE_X = 0.08f;// 敵の総数の表示サイズX
	const float ENEMY_INDEX_SIZE_Y = 0.18f;// 敵の総数の表示サイズY
private:
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
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
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// 数字テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// 「残り：」テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pRemaining;
	// 「/」テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pSlash;
	// コンスタントバッファ
	ConstBuffer m_constBuffer;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// 頂点情報
	// 総数の10の位
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex10[VERTEX_COUNT];
	// 総数の1の位
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex1[VERTEX_COUNT];
	// 現在の敵の数の10の位
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy10[VERTEX_COUNT];
	// 現在の敵の数の1の位
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy1[VERTEX_COUNT];
	// 「のこり：」
	DirectX::DX11::VertexPositionTexture m_verticesRemaining[VERTEX_COUNT];
	// 「/」
	DirectX::DX11::VertexPositionTexture m_verticesSlash[VERTEX_COUNT];
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
};
#endif //ENEMY_COUNTER_DEFINED