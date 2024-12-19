#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
class CommonResources;

class Effect
{
public:
	enum class ParticleType
	{
		ENEMY_DEAD = 0,
		ENEMY_HIT,
		NONE
	};
private:
	// データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅

	}m_constBuffer;

	// 共通リソース
	CommonResources* m_commonResources;

	DirectX::SimpleMath::Vector3 m_position;// エフェクトを再生する座標
	float m_scale;// エフェクトのスケール


	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//	テクスチャハンドル 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;



	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

	// フレームの頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[4];

	// ParticleType
	ParticleType m_type;
	// 今、エフェクトを再生してるか
	bool m_isPlaying;
	// フレーム数
	int m_anim;
	// アニメーションスピード
	float m_animSpeed;
	// アニメーションの経過時間
	float m_elapsedTime;

	// フレームの行数と列数
	int m_frameRows;
	int m_frameCols;

	// Particleによって高さを変える
	float m_offSetY;

	// 板ポリゴンの頂点座標
	float m_vertexMinX;
	float m_vertexMaxX;
	float m_vertexMinY;
	float m_vertexMaxY;

public:
	static const DirectX::VertexPositionTexture Vertices[4];
public:

	Effect(CommonResources* resources,
		ParticleType type,
		DirectX::SimpleMath::Vector3 playPos,
		float scale,
		DirectX::SimpleMath::Matrix world);// 再生位置を受け取る
	~Effect();
	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();


public:
	bool IsPlaying() const { return m_isPlaying; };
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; };
private:
	void LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& tex);

};