/*
*	@file	Result.h
*	@brief	結果クラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/Scene/IScene.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;

class Result final
{
public:	// 構造体
	struct ConstBuffer// シェーダーに渡す情報の構造体
	{
		DirectX::SimpleMath::Matrix		matWorld;	// ワールド行列
		DirectX::SimpleMath::Matrix		matView;	// ビュー行列
		DirectX::SimpleMath::Matrix		matProj;	// プロジェクション行列
		DirectX::SimpleMath::Vector4	Colors;		// 色
		float time;									// 時間
		DirectX::SimpleMath::Vector3 padding;		// パディング
	};
public:// アクセサ
	// シーンIDを取得
	IScene::SceneID GetSceneID() const { return m_sceneID; }
	// シーンIDを設定
	void SetSceneID(IScene::SceneID sceneID) { m_sceneID = sceneID; }
public:// public関数
	// コンストラクタ
	Result(CommonResources* resources);
	// デストラクタ
	~Result();
	//// テクスチャリソース読み込み
	//void LoadTexture(const wchar_t* path);
	// 初期化
	void Create(DX::DeviceResources* pDR);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
private:// private関数
	// シェーダーを作成
	void CreateShaders();
public:// 定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成
	CreateShader* m_pCreateShader;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 変数
	DX::DeviceResources* m_pDR;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// データ受け渡し用コンスタントバッファ
	ConstBuffer m_ConstBuffer;
	// タイトル画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// テクスチャパス
	wchar_t* m_texturePath;
	// 時間
	float m_time;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 頂点情報
	DirectX::VertexPositionTexture m_vertex[4];
	// シーンID
	IScene::SceneID m_sceneID;
};