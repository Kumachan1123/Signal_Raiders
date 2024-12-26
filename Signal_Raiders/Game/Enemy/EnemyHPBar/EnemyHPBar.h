/*
	@file	EnemyHPBar.h
	@brief	敵HPBarクラス
*/
#pragma once
#include <SimpleMath.h>
#include "Game/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
//前方宣言
class CommonResources;
class PlayScene;
class Enemy;



class EnemyHPBar
{
private:
	// データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;//	ワールド行列
		DirectX::SimpleMath::Matrix		matView;	//	ビュー行列
		DirectX::SimpleMath::Matrix		matProj;	//	プロジェクション行列
		DirectX::SimpleMath::Vector4	colors;
		DirectX::SimpleMath::Vector4	hp;
	}m_constBuffer;
	// 共通リソース
	CommonResources* m_commonResources;
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

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;	// 四角形で使用する 頂点情報

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DirectX::VertexPositionTexture	m_hpbarVert[4];
	DirectX::VertexPositionTexture	m_hpbarBackVert[4];

	// テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_gaugeTexture;
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::SimpleMath::Vector3 m_accele;		// 加速度
	DirectX::SimpleMath::Vector3 m_scale;		// スケール

	// 砲塔境界球
	DirectX::BoundingSphere m_enemyBoundingSphere;
	DirectX::BoundingSphere m_enemyWBoundingSphere;

	float m_time;
	int m_maxHP;// 最大HP(生成された瞬間の敵のHP)
	int m_currentHP;// 現在の敵のHP
	float m_displayedHP = 100.0f; // 表示されるHPを追跡する新しいメンバー
	float m_lerpSpeed = 2.0f; // 補間速度
	bool m_isDead = false;//敵のHPが0になったらTrue
public:
	// 頂点レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_enemyBoundingSphere; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }

	int GetHP() const { return m_currentHP; }
	bool GetIsDead() const { return m_isDead; }
	// setter
	void SetBoundingSphereCenter(DirectX::SimpleMath::Vector3& cen) { m_enemyBoundingSphere.Center = cen; }
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetEnemyHP(int& hp) { m_maxHP = hp; }
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }
public:
	// 初期ステータスを設定
	EnemyHPBar();
	~EnemyHPBar();

	void Initialize(CommonResources* resources);
	void Update(float elapsedTime, int currentHP);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot);
private:
	void LoadTexture(const wchar_t* path);
	void CreateBuffer();
	void CreateShader();
};


