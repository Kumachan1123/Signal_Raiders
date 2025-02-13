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
	enum class EffectType
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

	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	//	共通ステートオブジェクトへのポインタ
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	エフェクト 
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//	テクスチャハンドル 
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;


	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

	// フレームの頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[4];

	// ParticleType
	EffectType m_type;
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
	float m_vertexMinX;//左
	float m_vertexMaxX;//右
	float m_vertexMinY;//下
	float m_vertexMaxY;//上

public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// 入力レイアウト
public:
	// コンストラクタ
	Effect(CommonResources* resources,
		EffectType type,
		DirectX::SimpleMath::Vector3 playPos,
		float scale,
		DirectX::SimpleMath::Matrix world);
	// デストラクタ
	~Effect();
	void Update(float elapsedTime);// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 描画
	void Finalize();// 終了処理


public:
	bool IsPlaying() const { return m_isPlaying; }// 再生中か
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 座標取得
private:
	void LoadTexture(const wchar_t* path);// 画像を読み込む

};