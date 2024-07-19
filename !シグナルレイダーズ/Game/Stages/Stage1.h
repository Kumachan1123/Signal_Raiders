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

class Stage1
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// モデル
	std::unique_ptr<DirectX::Model> m_stage;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	//	テクスチャハンドル 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

	// ノーマルマップハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NormalMap;

	std::unique_ptr<DirectX::NormalMapEffect> m_BatchEffect;

	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_States;

	//	エフェクト 
	//std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;

	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

public:
	// 初期ステータスを設定
	Stage1();
	~Stage1();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

};