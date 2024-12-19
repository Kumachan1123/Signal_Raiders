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
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include <memory>

class CommonResources;
class EnemyCounter
{
private:
	// データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅

	}m_constBuffer;
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
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// 「残り：」テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_remaining;
	// 「/」テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_slash;

	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
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
	// ワールドビュープロジェクション行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
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
};