/*
*	@file	EnemyHPBar.h
*	@brief	敵HPBarクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <random>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include "Libraries/MyLib/GridFloor.h"
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/Enemy.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
//前方宣言
class CommonResources;
class PlayScene;
class Enemy;
class EnemyHPBar
{
private:// 構造体
	// シェーダーに渡す定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;//	ワールド行列
		DirectX::SimpleMath::Matrix		matView;	//	ビュー行列
		DirectX::SimpleMath::Matrix		matProj;	//	プロジェクション行列
		DirectX::SimpleMath::Vector4	colors;//		色
		DirectX::SimpleMath::Vector4	hp;//		HP
	};

public:// アクセサ
	DirectX::BoundingSphere& GetBoundingSphere() { return m_enemyBoundingSphere; }// 敵の境界球を取得
	void SetBoundingSphereCenter(const DirectX::SimpleMath::Vector3& cen) { m_enemyBoundingSphere.Center = cen; }// 敵の境界球の中心を設定
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 位置を取得
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }// 位置を設定
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }// 速度を取得
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }// 加速度を取得
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }// スケールを取得
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }// スケールを設定
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; } // 回転を取得
	int GetCurrentHP() const { return m_currentHP; } // 現在のHPを取得
	void SetCurrentHP(int& hp) { m_currentHP = hp; } // 現在のHPを設定
	void SetEnemyMaxHP(int& hp) { m_maxHP = hp; } // 敵の最大HPを設定
	bool GetIsDead() const { return m_isDead; } // 敵が死んでいるかどうかを取得
public:// public関数
	// コンストラクタ
	EnemyHPBar();
	// デストラクタ
	~EnemyHPBar();
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& rot);
private:// private関数
	// バッファ作成
	void CreateBuffer();
	// シェーダー作成
	void CreateShaders();
public:// public定数
	// 頂点レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 頂点数
	static const int VERTEX_COUNT = 4;
	// HPバーのX座標min
	const float HPBAR_X_MIN = -1.5f;
	// HPバーのX座標max
	const float HPBAR_X_MAX = 1.5f;
	// HPバーのY座標min
	const float HPBAR_Y_MAX = 3.25f;
	// HPバーのY座標max
	const float HPBAR_Y_MIN = 3.0f;
	// HPバーの背景のX座標min
	const float HPBARBACK_X_MIN = -1.53f;
	// HPバーの背景のX座標max
	const float HPBARBACK_X_MAX = 1.53f;
	// HPバーの背景のY座標max
	const float HPBARBACK_Y_MAX = 3.28f;
	// HPバーの背景のY座標min
	const float HPBARBACK_Y_MIN = 2.98f;
	// HPバーの幅
	const float BAR_LEFT = -1.50f;
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
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
	// コンスタントバッファ
	ConstBuffer m_constBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// HPバーの頂点情報
	DirectX::VertexPositionTexture	m_hpbarVert[VERTEX_COUNT];
	// HPバーの背景の頂点情報
	DirectX::VertexPositionTexture	m_hpbarBackVert[VERTEX_COUNT];
	// テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pGaugeTexture;
	// 敵の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 敵の速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 敵の回転
	DirectX::SimpleMath::Vector3 m_rotate;
	// 敵の加速度
	DirectX::SimpleMath::Vector3 m_accele;
	// 敵のスケール
	DirectX::SimpleMath::Vector3 m_scale;
	// 敵の境界球
	DirectX::BoundingSphere m_enemyBoundingSphere;
	// 敵のワールド境界球
	DirectX::BoundingSphere m_enemyWBoundingSphere;
	// 経過時間
	float m_time;
	// 最大HP(生成された瞬間の敵のHP)
	int m_maxHP;
	// 現在の敵のHP
	int m_currentHP;
	// 表示されるHP
	float m_displayedHP;
	// 補間速度
	float m_lerpSpeed;
	//敵のHPが0になったらTrue
	bool m_isDead;
};


