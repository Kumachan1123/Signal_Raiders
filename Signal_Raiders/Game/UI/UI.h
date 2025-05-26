/*
*	@file		UI.h
*	@brief		UIクラス(タイトル・リザルト・セッティング)
*/
#pragma once
// 標準ライブラリ
#include <vector>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/Anchor.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

class  UI
{
public:// 構造体
	struct ConstBuffer// シェーダーに渡す状
	{
		DirectX::SimpleMath::Vector4 windowSize;// ウィンドウサイズ
		float time;// 時間
		DirectX::SimpleMath::Vector3 color;// 色
	};
	// 列挙型
	enum class ShaderType// シェーダータイプ
	{
		NORMAL = 0,// 通常のメニュー用
		STAGE_SELECT// ステージセレクト用
	};
public:// アクセサ
	void SetWindowSize(const int& width, const int& height);// ウィンドウのサイズを設定
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; };// シェーダータイプを設定
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }// スケールを取得
	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };// スケールを設定
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }// 選択状態のスケールを取得
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; } // 位置を取得
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };// 位置を設定
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }// アンカーを取得
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; }// アンカーを設定
	float GetTime() const { return m_time; }// 時間を取得
	void SetTime(float time) { m_time = time; }// 時間を設定
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const; // 指定位置にヒットしたかどうかを取得
public:// public関数
	UI();// コンストラクタ
	~UI();// デストラクタ
	void LoadTexture(const wchar_t* path);// テクスチャの読み込み
	void Create(DX::DeviceResources* pDR// UIの作成
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
	void Update(float elapsedTime);// 更新
	void Render();// 描画
private:// private関数
	void CreateShaders();// シェーダーの作成
public:// 定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// インプットレイアウト
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 時間
	float m_time;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// テクスチャリソース
	Microsoft::WRL::ComPtr<ID3D11Resource> m_pTextureResource;
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダー(普通のメニュー)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ピクセルシェーダー(ステージセレクト用)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderStageSelect;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	// シェーダーの構造体(普通のメニュー)
	DrawPolygon::Shaders m_shaders;
	// シェーダーの構造体(ステージセレクト用)
	DrawPolygon::Shaders m_StageSelectShaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// コンスタントバッファ(シェーダーに渡す情報)
	ConstBuffer m_constBuffer{};
	// シェーダータイプ
	ShaderType m_shaderType;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// テクスチャの幅と高さ
	int m_textureWidth, m_textureHeight;
	// スケール
	DirectX::SimpleMath::Vector2 m_scale;
	// ベーススケール
	DirectX::SimpleMath::Vector2 m_baseScale;
	// 位置
	DirectX::SimpleMath::Vector2 m_position;
	// アンカー
	KumachiLib::ANCHOR m_anchor;
};
