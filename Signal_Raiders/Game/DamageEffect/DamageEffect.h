//--------------------------------------------------------------------------------------
// File: DamageEffect.h
//
// 攻撃を受けた時の演出クラス
//
//-------------------------------------------------------------------------------------

#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/Player/Player.h"
class Player;
class CommonResources;
class DamageEffect
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
		DirectX::SimpleMath::Vector2 uv;// uv座標
		float alpha = 0.0f;// アルファ値
	}m_constBuffer;
private:
	//	変数
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// 共通リソース
	CommonResources* m_commonResources;
	// プレイヤーのポインター
	Player* m_pPlayer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	//	ワールドビュープロジェクション行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// 時間
	float m_time;
	// 攻撃してきた敵のいる向き
	DirectX::SimpleMath::Vector3 m_enemyDirection;
	// プレイヤーの向き
	DirectX::SimpleMath::Vector3 m_playerDirection;

	// 固定値
	// 板ポリゴンのXサイズ
	const float SIZE_X = 0.0625;
	// 板ポリゴンのYサイズ
	const float SIZE_Y = 0.11;
	// 拡大率
	const float SCALE = 10.0f;
	// UV座標
	const float UV_X = 0.1f;// 左
	const float UV_Y = 0.1f;// 上
	const float UV_W = 0.9f;// 右
	const float UV_H = 0.9f;// 下
	const float UV_C = 0.5f;// 中央
	// 再生時間
	const float PLAY_TIME = 1.75f;
	// 破棄時間
	const float DESTROY_TIME = 2.0f;
	// エフェクト再生フラグ
	bool m_playEffect;
public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	DamageEffect(CommonResources* resources);
	~DamageEffect();

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);

	void Initialize(Player* pPlayer);
	void Update(float elapsedTime);

	void Render();

	// getter
	bool GetPlayEffect()const { return m_playEffect; }
	DirectX::SimpleMath::Vector3 GetEnemyDirection()const { return m_enemyDirection; }
	bool Destroy()const
	{
		if (m_time >= DESTROY_TIME)
		{

			return true;

		}

		return false;
	}
	// setter
	void SetEnemyDirection(DirectX::SimpleMath::Vector3 direction) { m_enemyDirection = direction; }
private:

	void CreateShader();
};