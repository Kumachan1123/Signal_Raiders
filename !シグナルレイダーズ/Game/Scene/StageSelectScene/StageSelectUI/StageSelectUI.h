/*
	@file	StageSelectUI.h
	@brief	ステージセレクトUIクラス
*/
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

class StageSelectUI
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
	Microsoft::WRL::ComPtr<ID3D11Resource> m_pTextureResource;
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;
	DirectX::SimpleMath::Vector2 m_scale;// スケール
	DirectX::SimpleMath::Vector2 m_baseScale;// ベーススケール
	DirectX::SimpleMath::Vector2 m_position;// 位置
	// アンカー
	kumachi::ANCHOR m_anchor;
	// 関数
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	StageSelectUI();
	~StageSelectUI();
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, kumachi::ANCHOR anchor);
	void Update(float elapsedTime);

	void Render();

	void SetWindowSize(const int& width, const int& height);

	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	void SetAnchor(kumachi::ANCHOR anchor) { m_anchor = anchor; };
	kumachi::ANCHOR GetAnchor() const { return m_anchor; }
	void SetTime(float time) { m_time = time; };
	float GetTime() const { return m_time; }

private:

	void CreateShader();
};
