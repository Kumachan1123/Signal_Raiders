/*
*	@file DamageEffect.h
*	@brief 攻撃を受けた時の演出クラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include <DeviceResources.h>
// 自作ヘッダーファイル
#include "Game/Player/Player.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
// クラスの前方宣言
class Player;
class IEnemy;
class CommonResources;
class DamageEffect
{
public:// 列挙型
	// エフェクトの種類
	enum class EffectType
	{
		DAMAGE = 0,	// ダメージを受けた時
		INCOMINGENEMY,// 敵が攻撃しようとしている時		
	};
public:// 構造体
	// シェーダーに渡す定数バッファの構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;      // 時間
		DirectX::SimpleMath::Vector4 uv;		// uv座標
		DirectX::SimpleMath::Vector4 alpha;		// アルファ値
	};
public:// アクセサ
	// プレイヤーのポインタ取得
	Player* GetPlayer()const { return m_pPlayer; }
	// プレイヤーのポインタ設定
	void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }
	// 敵のポインタ取得
	IEnemy* GetEnemy()const { return m_pEnemy; }
	// 敵のポインタ設定
	void SetEnemy(IEnemy* pEnemy) { m_pEnemy = pEnemy; }
	// エフェクト再生フラグ取得
	bool GetPlayEffect()const { return m_playEffect; }
	// 攻撃してきた敵の向き取得
	DirectX::SimpleMath::Vector3 GetEnemyDirection()const { return m_enemyDirection; }
	// エフェクトタイプ設定
	void SetEffectType(EffectType type) { m_effectType = type; }

public:	// public関数
	// コンストラクタ
	DamageEffect(CommonResources* resources);
	// デストラクタ
	~DamageEffect();
	// 初期化
	void Initialize();
	//// テクスチャリソース読み込み関数
	//void LoadTexture(const wchar_t* path);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// エフェクトの破棄判定
	bool Destroy()const;
private:// private関数
	// シェーダー作成
	void MakeShader();
	// 角度計算
	float CalculateAngle()const;
	// 角度からUV座標を求める
	DirectX::SimpleMath::Vector4 GetUVFromAngle(float angle)const;
public:// public定数
	// 頂点情報
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// private定数
	// 板ポリゴンのXサイズ
	const float SIZE_X = 0.0625;
	// 板ポリゴンのYサイズ
	const float SIZE_Y = 0.11;
	// 拡大率
	const float SCALE = 10.0f;
	// UV座標:左
	const float UV_X = 0.1f;
	// UV座標:上
	const float UV_Y = 0.1f;
	// UV座標:右
	const float UV_W = 0.9f;
	// UV座標:下
	const float UV_H = 0.9f;
	// UV座標:中央
	const float UV_C = 0.5f;
	// 再生時間
	const float PLAY_TIME = 1.75f;
	// 破棄時間
	const float DESTROY_TIME = 2.0f;
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// プレイヤーのポインター
	Player* m_pPlayer;
	// 敵のポインター
	IEnemy* m_pEnemy;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// シェーダーの構造体
	ConstBuffer m_constBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_pStates;
	// テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;
	// 攻撃してきた敵のいる向き
	DirectX::SimpleMath::Vector3 m_enemyDirection;
	// 攻撃してくる敵の座標
	DirectX::SimpleMath::Vector3 m_enemyPosition;
	// プレイヤーの向き
	DirectX::SimpleMath::Vector3 m_playerDirection;
	// エフェクト再生フラグ
	bool m_playEffect;
	// エフェクトタイプ
	EffectType m_effectType;
};