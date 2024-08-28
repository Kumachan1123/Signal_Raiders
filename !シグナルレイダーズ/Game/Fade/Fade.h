#pragma once
#include "StepTimer.h"
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
class CommonResources;
class Fade
{
public:
	// データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		float power;                             // 強度
		float fadeAmount;                       // フェードの進行度（0.0～1.0）
		DirectX::SimpleMath::Vector2 padding;// パディング
	};
private:
	//	変数
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DX::StepTimer                           m_timer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 共通リソース
	CommonResources* m_commonResources;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	//	テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// 時間
	float m_time;
public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//	コンストラクタ
	Fade(CommonResources* commonResources);
	//	デストラクタ
	~Fade() = default;
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);

	void Update(float elapsedTime);

	void Render();
private:

	void CreateShader();
};