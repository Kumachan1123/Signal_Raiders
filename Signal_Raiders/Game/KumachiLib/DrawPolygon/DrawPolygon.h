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
	// 三つのシェーダーをひとまとめにして送るための構造体
	struct Shaders
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> gs;
	};
public:
	// シングルトンインスタンスを取得
	static DrawPolygon* const GetInstance();

public:

	// デストラクタ
	~DrawPolygon();

	// 初期化（頂点、テクスチャ）
	void InitializePositionTexture(DX::DeviceResources* pDR);
	// 初期化（頂点、色、テクスチャ）
	void InitializePositionColorTexture(DX::DeviceResources* pDR);
	// 描画開始
	void DrawStart(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures);
	// サブリソースの更新
	void UpdateSubResources(ID3D11DeviceContext1* context, ID3D11Resource* resource, const void* pSrcData);
	// 板ポリゴン描画（頂点、テクスチャ）
	void DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices);
	// 板ポリゴン描画（頂点、色、テクスチャ）
	void DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count);
	// 解放（頂点、テクスチャ）
	void ReleasePositionTexture();
	// 解放（頂点、色、テクスチャ）
	void ReleasePositionColorTexture();
	// シェーダーにバッファを送る
	void SetShaderBuffer(ID3D11DeviceContext1* context, UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer);
	// シェーダーをセットする
	void SetShader(ID3D11DeviceContext1* context, const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances);
	// シェーダーを解放する
	void ReleaseShader(ID3D11DeviceContext1* context);
private:
	// コンストラクタ
	DrawPolygon();
	// コピーコンストラクタと代入演算子の禁止
	DrawPolygon(const DrawPolygon&) = delete;
	DrawPolygon& operator=(const DrawPolygon&) = delete;

private:
	// プリミティブバッチ（頂点、テクスチャ）
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatchTexture;
	// プリミティブバッチ（頂点、色、テクスチャ）
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatchColorTexture;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// シングルトンインスタンス
	static std::unique_ptr<DrawPolygon> m_instance;
};