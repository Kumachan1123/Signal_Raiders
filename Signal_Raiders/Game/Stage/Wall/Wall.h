/*
*	@file Wall.h
*	@brief 壁クラス
*/
#pragma once
#ifndef WALL_DEFINED
#define WALL_DEFINED
// 標準ライブラリ
#include <cassert>
// DirectX
#include <DeviceResources.h>
#include <WICTextureLoader.h> 
#include <SimpleMath.h>
// 外部ライブラリ
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// 壁クラス
class Wall
{
public:
	// 構造体
	// シェーダーに渡す情報
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;    // 時間
	};
public:
	// public関数
	// コンストラクタ
	Wall(CommonResources* resources);
	// デストラクタ
	~Wall();
	// テクスチャの読み込み
	void LoadTexture(const wchar_t* path);
	// 初期化
	void Create(DX::DeviceResources* pDR);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 当たり判定の取得
	const DirectX::BoundingBox& GetBoundingBox(int index) { return m_wallBox[index]; }
	// 壁の数の取得
	int GetWallNum()const { return WALL_NUM; }
private:
	// private関数
	void CreateWalls();// 壁の初期化
	void CreateShaders();// シェーダーの作成
public:
	// 定数
	// 壁の数
	static const int WALL_NUM = 4;
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 高さ
	static const float WALL_HEIGHT;
	// 幅
	static const float WALL_WIDTH;
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 変数
	DX::DeviceResources* m_pDR;
	// 壁テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pWallTexture;
	// 壁の頂点
	DirectX::VertexPositionTexture m_wall[Wall::WALL_NUM][WALL_NUM];
	// 壁の当たり判定
	DirectX::BoundingBox m_wallBox[WALL_NUM];
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
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
	// コンスタントバッファ(シェーダーに渡す情報)
	ConstBuffer m_constBuffer;
	// 入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
};
#endif // WALL_DEFINED