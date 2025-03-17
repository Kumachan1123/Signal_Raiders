//--------------------------------------------------------------------------------------
//@file   UI.h
//@brief  UIクラス
//-------------------------------------------------------------------------------------

#pragma once
#include "Game/KumachiLib/Anchor.h"
#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
class  UI
{
public:
	// データ受け渡し用コンスタントバッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 windowSize;
		float time;
		DirectX::SimpleMath::Vector3 color;
	};
	ConstBuffer m_constBuffer{};
	enum class ShaderType
	{
		NORMAL,
		STAGE_SELECT
	};

public:
	//	変数
	DX::DeviceResources* m_pDR;// デバイスリソース
	float m_time;// 時間
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;// 入力レイアウト
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_pPrimitiveBatch;// プリミティブバッチ
	std::unique_ptr<DirectX::CommonStates> m_pStates;// ステート
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
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
	// シェーダータイプ
	ShaderType m_shaderType;
	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;
	DirectX::SimpleMath::Vector2 m_scale;// スケール
	DirectX::SimpleMath::Vector2 m_baseScale;// ベーススケール
	DirectX::SimpleMath::Vector2 m_position;// 位置
	// アンカー
	KumachiLib::ANCHOR m_anchor;
	// 関数
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	UI();
	~UI();
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
	void Update(float elapsedTime);

	void Render();

	void SetWindowSize(const int& width, const int& height);
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; };
	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; };
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	void SetTime(float time) { m_time = time; };
	float GetTime() const { return m_time; }
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const;
private:

	void CreateShader();
};
