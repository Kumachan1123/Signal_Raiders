/*
*	@file	Goal.h
*	@brief	目標を表示するクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
 // 前方宣言
class CommonResources;
class Goal
{
public:// 構造体
	// シェーダーに渡すデータ構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;		// 時間
	};
public:// public関数
	Goal(CommonResources* resources);// コンストラクタ
	~Goal();// デストラクタ
	void LoadTexture(const wchar_t* path);// テクスチャリソース読み込み
	void Create(DX::DeviceResources* pDR);// 生成
	void Update(float elapsedTime);// 更新
	void Render();// 描画
private:// private関数
	void CreateShaders();// シェーダーの作成
private:// 定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// インプットレイアウト
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
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
	// コンスタントバッファ(シェーダーに送るデータ)
	ConstBuffer m_constBuffer;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;

};