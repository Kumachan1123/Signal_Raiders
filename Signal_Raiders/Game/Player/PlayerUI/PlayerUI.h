//--------------------------------------------------------------------------------------
//@file    PlayerUI.h
//@brief  プレイヤーUIクラス
//-------------------------------------------------------------------------------------


#pragma once
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/Anchor.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>


class PlayerUI
{
public:
	enum class ShaderType
	{
		HP = 0,// HPゲージのような動きをする
		CIRCLE,// 円形ゲージのような動きをする
		ANIM,// UVアニメーションさせる
		OTHER,// その他
	};

	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;
		DirectX::SimpleMath::Vector2	frame;
		float							animCount;
		float							renderRatio;
	};
	ConstBuffer m_constBuffer{};

private:
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;


	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;
	// HP以外のシェーダー
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	// HPで使うシェーダー
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_hpPixelShader;
	// 円形ゲージのシェーダー
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_circlePixelShader;
	// UVアニメーション用シェーダー
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_animPixelShader;

	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// シェーダーの構造体
	DrawPolygon::Shaders m_hpShaders;
	// シェーダーの構造体
	DrawPolygon::Shaders m_circleShaders;
	// シェーダーの構造体
	DrawPolygon::Shaders m_animShaders;

	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;

	// フレームの行数と列数
	int m_frameRows;
	int m_frameCols;
	// フレーム数
	int m_anim;
	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_baseScale;
	DirectX::SimpleMath::Vector2 m_position;

	KumachiLib::ANCHOR m_anchor;

	ShaderType m_shaderType;
	float m_renderRatio;
	float m_renderRatioOffset;
	//関数
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerUI();
	~PlayerUI();

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);

	void Render();

	void SetWindowSize(const int& width, const int& height);

	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector2 GetBaseScale() const { return m_baseScale; }
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; };
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }

	void SetRenderRatio(float ratio) { m_renderRatio = ratio; };
	float GetRenderRatio() const { return m_renderRatio; }

	void SetRenderRatioOffset(float offset) { m_renderRatioOffset = offset; };
	float GetRenderRatioOffset() const { return m_renderRatioOffset; }
	void SetShaderType(ShaderType type) { m_shaderType = type; }
	void SetAnim(int anim) { m_anim = anim; }
	void SetFrameRows(int rows) { m_frameRows = rows; }
	void SetFrameCols(int cols) { m_frameCols = cols; }
private:

	void CreateShader();
};


