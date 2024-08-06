#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
class CommonResources;

class Particle
{
private:
	// 共通リソース
	CommonResources* m_commonResources;

	DirectX::SimpleMath::Vector3 m_position;// エフェクトを再生する座標
	float m_scale;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Matrix m_Billboard;
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_States;

	//	テクスチャハンドル 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

	//	エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;

	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
	// アニメーション用
	// 今、エフェクトを再生してるか
	bool m_isPlaying;
	// フレーム数
	int anim;
	// アニメーションスピード
	float m_animSpeed;
	// アニメーションの経過時間
	float m_elapsedTime;

	// フレームの行数と列数
	int m_frameRows;
	int m_frameCols;



public:
	static const DirectX::VertexPositionTexture Vertices[4];
public:

	Particle(CommonResources* resources, DirectX::SimpleMath::Vector3 PlayPos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Matrix world);// デバイスと再生位置を受け取る
	~Particle();
	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();
	void CreateBillboard(DirectX::SimpleMath::Vector3 target,
						 DirectX::SimpleMath::Vector3 eye,
						 DirectX::SimpleMath::Vector3 up);

public:
	bool IsPlaying() { return m_isPlaying; };
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; };

};