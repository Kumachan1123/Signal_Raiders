/*
*	@file Effect.h
*	@brief エフェクトクラス
*/
#pragma once
// DirectX
#include <PrimitiveBatch.h>
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <DeviceResources.h>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
// 自作ヘッダーファイル
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
// 前方宣言
class CommonResources;
class Effect
{
public:
	// 列挙型
	enum class EffectType// エフェクトの種類
	{
		ENEMY_DEAD = 0,// 敵死亡エフェクト
		ENEMY_HIT,// 敵ヒットエフェクト
		NONE,// なし
	};
public:
	// 構造体
	struct ConstBuffer	// データ受け渡し用コンスタントバッファ(送信側)
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅
	};
public:
	// アクセサ
	bool IsPlaying() const { return m_isPlaying; }// 再生中か
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 座標取得
	// publicメンバ関数
	Effect(	// コンストラクタ
		CommonResources* resources,// 共通リソース
		EffectType type,// エフェクトの種類
		DirectX::SimpleMath::Vector3 playPos,// 再生する座標
		float scale,// スケール
		DirectX::SimpleMath::Matrix world);// ワールド行列
	~Effect();	// デストラクタ
	void Update(float elapsedTime);// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 描画
	void Finalize();// 終了処理


private:
	void LoadTexture(const wchar_t* path);// 画像を読み込む
private:// 定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// 入力レイアウト
	// 板ポリゴンの頂点座標
	static const float m_vertexMinX;//左
	static const float m_vertexMaxX;//右
	static const float m_vertexMinY;//下
	static const float m_vertexMaxY;//上
private:
	// メンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// エフェクトを再生する座標
	DirectX::SimpleMath::Vector3 m_position;
	// エフェクトのスケール
	float m_scale;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	// コンスタントバッファ
	ConstBuffer m_constBuffer;
	// テクスチャハンドル 
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
	// フレームの頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[4];
	// エフェクトの種類
	EffectType m_type;
	// 今、エフェクトを再生してるか
	bool m_isPlaying;
	// フレーム数
	int m_anim;
	// アニメーションスピード
	float m_animSpeed;
	// アニメーションの経過時間
	float m_animTime;
	// フレームの行数と列数
	int m_frameRows;//行
	int m_frameCols;//列
	// エフェクトによって高さを変える
	float m_offSetY;



};