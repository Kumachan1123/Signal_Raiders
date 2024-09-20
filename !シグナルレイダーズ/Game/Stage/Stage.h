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
class Stage
{


private:
	// 共通リソース
	CommonResources* m_commonResources;
	// モデル
	std::unique_ptr<DirectX::Model> m_pStageModel;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	//	テクスチャハンドル 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_pStates;

	//	エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect> m_pBatchEffect;

	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatch;

	// 床の深度ステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Floor;



public:

	// 初期ステータスを設定
	Stage();
	~Stage();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
private:
	// 深度ステンシルステートを初期化する
	void InitializeDepthStencilState(ID3D11Device* device);

};