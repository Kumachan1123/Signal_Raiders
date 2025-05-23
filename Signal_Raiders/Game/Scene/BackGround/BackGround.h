/*
*	@file BackGround.h
*	@brief 背景クラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
#include <WICTextureLoader.h>
// 外部ライブラリ
#include "Libraries/MyLib/DebugString.h"
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
// 前方宣言
class CommonResources;
class BackGround
{
public:
	// シェーダーに渡す構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;		// 時間
	};
public:// public関数
	BackGround(CommonResources* resources);// コンストラクタ
	~BackGround();// デストラクタ
	void LoadTexture(const wchar_t* path);// テクスチャリソース読み込み
	void Create(DX::DeviceResources* pDR);// 生成
	void Update(float elapsedTime); // 更新
	void Render();// 描画
private:// private関数
	void CreateShaders();// シェーダー作成
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// デバイスコンテキスト
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// 共通リソース
	CommonResources* m_commonResources;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// コンスタントバッファ
	ConstBuffer m_constBuffer;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;
	// シェーダーに渡す色
	DirectX::SimpleMath::Vector4 m_color;
public:
	// 定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
};