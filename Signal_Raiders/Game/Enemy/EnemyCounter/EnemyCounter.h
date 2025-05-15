/*
	@file	EnemyCounter.h
	@brief	敵カウントクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
// 外部ライブラリ
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
// 前方宣言
class CommonResources;
class EnemyCounter
{
private:
	// private構造体
	struct ConstBuffer// コンスタントバッファ
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅

	};
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
	void LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& tex);
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
	static const int VERTEX_COUNT = 4; // 頂点数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// インプットレイアウト

	// 各画像の描画位置とサイズ
	// 「残り：」の描画位置・サイズ
	const float REMAINING_POS_X = 0.4f;
	const float REMAINING_POS_Y = 1.0f;
	const float REMAINING_SIZE_X = 0.25f;
	const float REMAINING_SIZE_Y = 0.18f;

	// 「/」の描画位置・サイズ
	const float SLASH_POS_X = 0.8f;
	const float SLASH_POS_Y = 1.0f;
	const float SLASH_SIZE_X = 0.05f;
	const float SLASH_SIZE_Y = 0.18f;

	// 現在の敵の数の1の位・10の位の描画位置・サイズ
	const float NOW_ENEMY_10_POS_X = 0.66f;
	const float NOW_ENEMY_10_POS_Y = 1.0f;
	const float NOW_ENEMY_1_POS_X = 0.72f;
	const float NOW_ENEMY_1_POS_Y = 1.0f;
	const float NOW_ENEMY_SIZE_X = 0.08f;
	const float NOW_ENEMY_SIZE_Y = 0.18f;

	// 敵の総数の1の位・10の位の描画位置・サイズ
	const float ENEMY_INDEX_10_POS_X = 0.86f;
	const float ENEMY_INDEX_10_POS_Y = 1.0f;
	const float ENEMY_INDEX_1_POS_X = 0.92f;
	const float ENEMY_INDEX_1_POS_Y = 1.0f;
	const float ENEMY_INDEX_SIZE_X = 0.08f;
	const float ENEMY_INDEX_SIZE_Y = 0.18f;

private:
	// private変数
	// 共通リソースへのポインタ
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
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// 数字テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// 「残り：」テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_remaining;
	// 「/」テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_slash;
	// コンスタントバッファ
	ConstBuffer m_constBuffer;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
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
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;



};