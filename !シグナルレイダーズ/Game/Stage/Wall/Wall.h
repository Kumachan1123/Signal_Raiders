#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <SimpleMath.h>
#include <Effects.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/BinaryFile.h"
class Wall
{
public:
	// データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		float time = 0.0f;                             // 時間
		DirectX::SimpleMath::Vector3 padding;// パディング
	}m_constBuffer;
private:
	// 共通リソース
	CommonResources* m_commonResources;
	//	変数
	DX::DeviceResources* m_pDR;
	//	壁テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pWallTexture;

	//	ワールドビュープロジェクション行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatch;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_pStates;

	//	エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect> m_pBatchEffect;

public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// 初期ステータスを設定
	Wall(CommonResources* resources);
	~Wall();
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:

	void CreateShader();
};