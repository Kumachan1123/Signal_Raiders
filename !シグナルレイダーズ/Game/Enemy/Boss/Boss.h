/*
	@file	Boss.h
	@brief	ボスクラス
	作成者：くまち
*/
#pragma once
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
// 前方宣言
class CommonResources;
class Player;
class Effect;
class EnemyBullets;
class EnemyModel;

class Boss
{
private:
	// コモンリソース
	CommonResources* m_commonResources;
	// プレイヤー
	Player* m_pPlayer;
	// エフェクト
	std::vector<std::unique_ptr<Effect>> m_effect;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// SEの音量
	float m_SEVolume = 0.0f;
	// 敵生成フラグ
	bool m_isBossBorn = false;
	// 敵生成済み
	bool m_isBorned = false;
	// プレイヤーと敵の当たり判定
	bool m_isHitPlayerToBoss = false;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	// 砲塔境界球
	DirectX::BoundingSphere m_bossBS;	//敵の境界球
	DirectX::BoundingSphere m_bossBSToPlayerArea;// 敵とPlayerとの一定範囲の当たり判定に使う
	DirectX::BoundingSphere m_bossBulletBS;// 敵の弾の境界球
	DirectX::BoundingSphere m_playerBS;// プレイヤーの境界球
	DirectX::SimpleMath::Matrix m_matrix;// マトリクス
	std::unique_ptr<EnemyModel>		m_enemyModel;// 敵のモデル
	std::unique_ptr<EnemyHPBar>		m_HPBar;// 敵のHPバー
	std::unique_ptr<EnemyBullets>	m_bossBullets;// 敵の弾
	int m_currentHP;//敵の体力
	bool m_isDead = false;//敵のHPが0になったらTrue
	bool m_isHit = false;// プレイヤーとの判定
	bool m_isHitToOtherBoss = false;// その他の敵との判定
	bool m_isHitToPlayerBullet = false;// 敵がプレイヤーの弾に当たったか
	bool m_isBullethit = false;// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown;  // 攻撃のクールダウンタイ
	// プレイヤーに与えるダメージ
	const float PLAYER_DAMAGE = 5.0f;

public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_bossBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_bossBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	DirectX::SimpleMath::Matrix GetMatrix() const { return m_matrix; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }
	Player* GetPlayer()const { return m_pPlayer; }
	int GetHP() const { return m_currentHP; }
	bool GetEnemyIsDead() const { return m_isDead; }
	bool GetHitToPlayer()const { return m_isHit; }
	bool GetHitToOtherEnemy() const { return m_isHitToOtherBoss; }
	bool GetBulletHitToPlayer() const { return m_isBullethit; }// 敵の弾がプレイヤーに当たったか
	bool GetHitToPlayerBullet()const { return m_isHitToPlayerBullet; }
	float GetToPlayerDamage() const { return PLAYER_DAMAGE; }
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetEnemyHP(int hp) { m_currentHP = hp; }
	void SetHitToPlayer(bool isHitToPlayer) { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherBoss) { m_isHitToOtherBoss = isHitToOtherBoss; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_bossBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) { m_isBullethit = hit; }// 敵の弾がプレイヤーに当たったか
	void SetHitToPlayerBullet(bool hit) { m_isHitToPlayerBullet = hit; }
public:
	// 初期ステータスを設定
	Boss(Player* pPlayer);
	~Boss();
	void Initialize(CommonResources* resources, int hp);
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B);


};