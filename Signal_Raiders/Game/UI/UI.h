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
#include "Game/CommonResources.h"
#include "Game/KumachiLib/Anchor.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;
class  UI
{
public:// 列挙型
	// シェーダータイプ
	enum class ShaderType
	{
		NORMAL = 0,// 通常のメニュー用
		STAGE_SELECT// ステージセレクト用
	};
public:// 構造体
	// シェーダーに渡す値
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 windowSize;// ウィンドウサイズ
		float time;// 時間
		DirectX::SimpleMath::Vector3 color;// 色
	};

public:// アクセサ
	// ウィンドウのサイズを設定
	void SetWindowSize(const int& width, const int& height);
	// シェーダータイプを設定
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; }
	// スケールを取得
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	// スケールを設定
	void SetScale(const DirectX::SimpleMath::Vector2& scale) { m_scale = scale; }
	// 選択状態のスケールを取得
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	// 位置を取得
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// 位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; };
	// アンカーを取得
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	// アンカーを設定
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; }
	// 時間を取得
	float GetTime() const { return m_time; }
	// 時間を設定
	void SetTime(float time) { m_time = time; }
	// 指定位置にヒットしたかどうかを取得
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const;
public:// public関数
	// コンストラクタ
	UI(CommonResources* pCommonResources);
	// デストラクタ
	~UI();
	// テクスチャの読み込み
	void LoadTexture(std::string key);
	// UIの作成
	void Create(DX::DeviceResources* pDR
		, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
private:// private関数
	// シェーダーの作成
	void CreateShaders();
public:// 定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
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
	// テクスチャの配列
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
