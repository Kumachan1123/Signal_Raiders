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
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"

#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
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
		DirectX::SimpleMath::Vector4 time;    // 時間
	}m_constBuffer;
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// 変数
	DX::DeviceResources* m_pDR;
	// 壁テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pWallTexture;
	// 壁の頂点
	DirectX::VertexPositionTexture m_wall[4][4];
	// 壁の当たり判定
	DirectX::BoundingBox m_wallBox[4];
	// ワールドビュープロジェクション行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;

	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatch;


	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_pStates;


	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;


public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// 初期ステータスを設定
	Wall(CommonResources* resources);
	~Wall();
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);
	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	DirectX::BoundingBox& GetBoundingBox(int index) { return m_wallBox[index]; }
private:

	void CreateShader();
};