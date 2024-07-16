/*
	@file	Enemy.h
	@brief	敵クラス
	作成者：くまち
*/
#pragma once
//前方宣言
class CommonResources;
class PlayScene;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class Enemy
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;
	std::unique_ptr<EnemyModel>		m_enemyModel;
	std::unique_ptr<EnemyAI>		m_enemyAI;
	std::unique_ptr<EnemyHPBar>		m_HPBar;
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // 弾のリスト
	// モデルの影
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>m_depthStencilState;
	// 輪郭（仮）
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;	// ブレンドステート
	// 追加：アウトライン用のシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_outlinePS;
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::SimpleMath::Vector3 m_accele;		// 加速度
	DirectX::SimpleMath::Vector3 m_nowScale;		// 現在スケール
	DirectX::SimpleMath::Vector3 m_startScale;		// 初期スケール
	DirectX::SimpleMath::Vector3 m_endScale;		// 最終スケール
	DirectX::SimpleMath::Quaternion m_rotation; // クォータニオン (追加)
	// 砲塔境界球
	DirectX::BoundingSphere m_enemyBoundingSphere;	//敵の境界球
	DirectX::BoundingSphere m_enemyBoundingSphereToPlayer;// 敵とPlayerとの一定範囲との当たり判定に使う
	DirectX::BoundingSphere m_enemyWBoundingSphere;
	DirectX::BoundingSphere m_enemyBulletBS;
	DirectX::BoundingSphere m_playerBS;
	DirectX::SimpleMath::Matrix m_matrix;				// マトリクス

	int m_currentHP;//敵の体力
	bool m_isDead = false;//敵のHPが0になったらTrue
	bool m_isHit = false;// プレイヤーとの判定
	bool m_isHitToOtherEnemy = false;// その他の敵との判定
	bool m_isBullethit = false;// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown;  // 攻撃のクールダウンタイム

	// Player
	const float PLAYER_DAMAGE = 0.1f;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_enemyBoundingSphere; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }
	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }
	int GetHP() const { return m_currentHP; }
	bool GetEnemyIsDead() const { return m_isDead; }
	bool GetHitToPlayer()const { return m_isHit; }
	bool GetHitToOtherEnemy() const { return m_isHitToOtherEnemy; }
	bool GetBulletHitToPlayer() const { return m_isBullethit; }
	// setter
	void SetBoundingSphereCenter(DirectX::SimpleMath::Vector3& cen) { m_enemyBoundingSphere.Center = cen; }
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetEnemyHP(int hp) { m_currentHP = hp; }
	void SetHitToPlayer(bool isHitToPlayer) { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere& playerBS) { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) { m_isBullethit = hit; }
public:
	// 初期ステータスを設定
	Enemy();
	~Enemy();
	void Initialize(CommonResources* resources, int hp);
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void CheckHitOtherEnemy(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B);
public:
	void AddBullet(std::unique_ptr<EnemyBullet> bullet);
	void UpdateBullets(float elapsedTime);

};