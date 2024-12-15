/*
	@file	DrawPolygon.h
	@brief	当たり判定描画用クラス
*/
#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
//前方宣言
class CommonResources;

class DrawPolygon
{
public:

	// 初期化（頂点、テクスチャ）
	static void InitializePositionTexture(DX::DeviceResources* pDR);
	// 初期化（頂点、色、テクスチャ）
	static void InitializePositionColorTexture(DX::DeviceResources* pDR);
	// 描画開始（頂点、テクスチャ）
	static void DrawStartTexture(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures);
	// 描画開始（頂点、色、テクスチャ）
	static void DrawStartColorTexture(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures);
	// サブリソースの更新
	static void UpdateSubResources(ID3D11DeviceContext1* context, ID3D11Resource* resource, const void* pSrcData);
	// 板ポリゴン描画（頂点、テクスチャ）
	static void DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices);
	// 板ポリゴン描画（頂点、色、テクスチャ）
	static void DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count);
	// 解放（頂点、テクスチャ）
	static void ReleasePositionTexture();
	// 解放（頂点、色、テクスチャ）
	static void ReleasePositionColorTexture();

private:
	// ベーシックエフェクト
	static std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プリミティブバッチ（頂点、テクスチャ）
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatchTexture;
	// プリミティブバッチ（頂点、色、テクスチャ）
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatchColorTexture;
	// 共通リソース
	static CommonResources* m_commonResources;
	//コモンステート
	static std::unique_ptr<DirectX::CommonStates> m_states;
};