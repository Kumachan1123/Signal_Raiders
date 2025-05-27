/*
*	@file	Fade.h
*	@brief	フェードクラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Libraries/MyLib/DebugString.h"
// 自作ヘッダーファイル
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/KumachiLib.h"
// 前方宣言
class CommonResources;
class Fade
{
public:
	// 構造体
	struct ConstBuffer
	{
		float smoothness = 0;                             // フェードの滑らかさ
		float fadeAmount = 0;                       // フェードの進行度（0.0～1.0）
		DirectX::SimpleMath::Vector2 padding;							// パディング
	};
	// 列挙型
	enum class FadeState
	{
		None,// なし
		// 以下、フェードイン・アウトの開始状態
		FadeIn,// フェードイン
		FadeOut,// フェードアウト
		// 以下、フェードイン・アウトの途中状態
		FadeInEnd,// フェードイン終了
		FadeOutEnd,// フェードアウト終了
	};


public:
	// アクセサ
	void SetState(FadeState state) { m_fadeState = state; }
	FadeState GetState() const { return m_fadeState; }
public:
	// public関数
	Fade(CommonResources* commonResources);	//	コンストラクタ
	~Fade();	//	デストラクタ
	void LoadTexture(const wchar_t* path);// テクスチャの読み込み
	void Initialize();// 初期化
	void Update(float elapsedTime);// 更新
	void Render();// 描画
private:
	// private関数
	void CreateShaders();// シェーダーの作成
	void FadeOut(float elapsedTime);// フェードアウト
	void FadeIn(float elapsedTime);// フェードイン
public:
	// public定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// フェード最小値
	static const float FADE_MIN;
	// フェード最大値
	static const float FADE_MAX;
	// フェード速度
	static const float FADE_SPEED;
	// フェード初期値
	static const float FADE_INIT;
	// フェードの滑らかさ
	static const float FADE_SMOOTHNESS;
private:
	// 変数
	DX::DeviceResources* m_pDR;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;

	// テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
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
	// フェード状態
	FadeState m_fadeState;
	// 時間
	float m_time;
};