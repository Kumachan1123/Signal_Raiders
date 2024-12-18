/*
	@file	EnemyHPBar.h
	@brief	敵HPBarクラス
*/
#pragma once
//前方宣言
class CommonResources;
class PlayScene;
class Enemy;



class EnemyHPBar
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	/*
		ビルボードの描画に関連するオブジェクト
	*/
	// FPSカメラ

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;	// 四角形で使用する 頂点情報

	DirectX::VertexPositionTexture	m_hpbarVert[4];
	DirectX::VertexPositionTexture	m_hpbarBackVert[4];

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gaugeTexture;
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::SimpleMath::Vector3 m_accele;		// 加速度
	DirectX::SimpleMath::Vector3 m_scale;		// スケール

	// 砲塔境界球
	DirectX::BoundingSphere m_enemyBoundingSphere;
	DirectX::BoundingSphere m_enemyWBoundingSphere;


	int m_maxHP;// 最大HP(生成された瞬間の敵のHP)
	int m_currentHP;// 現在の敵のHP
	float m_displayedHP = 100.0f; // 表示されるHPを追跡する新しいメンバー
	float m_lerpSpeed = 2.0f; // 補間速度
	bool m_isDead = false;//敵のHPが0になったらTrue


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

};

