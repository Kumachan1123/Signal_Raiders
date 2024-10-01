/*
	@file	Enemy.h
	@brief	敵クラス
	作成者：くまち
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Interface/IEnemy.h"
//前方宣言
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class EnemyBullets;
class Enemies;
class Enemy : public IEnemy
{
private:
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// デバッグ用
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	std::unique_ptr<DirectX::Model> m_model;          // 影用のモデル
	std::unique_ptr<EnemyModel> m_enemyModel;         // 敵のモデル
	std::unique_ptr<EnemyAI> m_enemyAI;               // 敵のAI
	std::unique_ptr<EnemyHPBar> m_HPBar;              // 敵のHPバー
	std::unique_ptr<EnemyBullets> m_enemyBullets;     // 敵の弾

	// 影関連
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	// 砲塔境界球
	DirectX::BoundingSphere m_enemyBS;	//敵の境界球
	DirectX::BoundingSphere m_enemyBSToPlayerArea;// 敵とPlayerとの一定範囲の当たり判定に使う
	DirectX::BoundingSphere m_enemyBulletBS;// 敵の弾の境界球
	DirectX::BoundingSphere m_playerBS;// プレイヤーの境界球
	DirectX::SimpleMath::Matrix m_matrix;// マトリクス

	int m_currentHP;//敵の体力
	bool m_isDead = false;//敵のHPが0になったらTrue
	bool m_isHit = false;// プレイヤーとの判定
	bool m_isHitToOtherEnemy = false;// その他の敵との判定
	bool m_isHitToPlayerBullet = false;// 敵がプレイヤーの弾に当たったか
	bool m_isBullethit = false;// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown;  // 攻撃のクールダウンタイ
	// プレイヤーに与えるダメージ
	const float PLAYER_DAMAGE = 1.0f;
	// オーディオマネージャー
	AudioManager* m_audioManager;
public:
	// コンストラクタ
	Enemy(Player* pPlayer);
	~Enemy();

	// インターフェースの実装
	void Initialize(CommonResources* resources, int hp) override;
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B) override;

};