/*
*	@file	BossAI.h
*	@brief	ボスAIクラス
*/
#pragma once
#ifndef BOSS_AI_DEFINED
#define BOSS_AI_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>
#include <type_traits> // std::enable_if, std::is_integral
// DirectX
#include <SimpleMath.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/Boss/BossAI/BossAttack/BossAttack.h"
#include "Game/Enemy/Boss/BossAI/BossKnockBacking/BossKnockBacking.h"
#include "Game/Player/Player.h"
#include "Game/Interface/IState.h"
//前方宣言
class CommonResources;
class BossAttack;
class BossKnockBacking;
class BossAI
{
public:	// アクセサ
	// 初期位置を取得
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	// 位置を取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 回転を取得
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// 回転を設定
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// 速度を取得
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// 速度を設定
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	// スケールを取得
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// スケールを設定
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// 攻撃時の状態を取得
	BossAttack* GetBossAttack()const { return m_pBossAttack.get(); }
	// 敵を取得
	IEnemy* GetEnemy()const { return m_pBoss; }
	// 現在の状態を取得
	IState* GetNowState()const { return m_pCurrentState; }
	// 状態を取得
	IState::EnemyState GetState()const { return m_enemyState; }
	// 状態を設定
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	// プレイヤーの弾に当たったか設定
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
	// 攻撃中か取得
	bool GetIsAttack()const { return m_pBoss->GetIsAttack(); }
	// 攻撃中か設定
	void SetIsAttack(bool attack) { m_pBoss->SetIsAttack(attack); }
	// 攻撃時の表情差分を取得
	IState::EnemyState GetAttackState() const { return m_attackState; }
	// 攻撃時の表情差分を設定
	void SetAttackState(IState::EnemyState state) { m_attackState = state; }
	// ノックバックが完了したか取得
	bool GetIsKnockBack() const { return m_isKnockBack; }
	// ノックバックが完了したか設定
	void SetIsKnockBack(bool isKnockBack) { m_isKnockBack = isKnockBack; }
public:	// public関数
	// コンストラクタ
	BossAI(IEnemy* pBoss);
	// デストラクタ
	~BossAI();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 状態変更
	void ChangeState(IState* newState);
private:// private変数
	//攻撃時
	std::unique_ptr<BossAttack> m_pBossAttack;
	//ノックバック時
	std::unique_ptr<BossKnockBacking> m_pBossKnockBacking;
	// 現在の状態
	IState* m_pCurrentState;
	// 状態
	IState::EnemyState m_enemyState;
	// 攻撃時表情差分
	IState::EnemyState m_attackState;
	// ボス
	IEnemy* m_pBoss;
	//移動
	DirectX::SimpleMath::Vector3 m_position;
	// 座標初期値
	DirectX::SimpleMath::Vector3 m_initialPosition;
	//当たり判定用座標
	DirectX::SimpleMath::Vector3 m_bsPosition;
	//サイズ
	DirectX::SimpleMath::Vector3 m_scale;
	//回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// ノックバック時間
	float m_knockTime;
	// ノックバックの開始位置 
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// ノックバックの終了位置
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// ノックバックの初速度
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	// 時間
	float m_time;
	// 攻撃のクールダウンタイム
	float m_attackCooldown;
	// プレイヤーの弾に当たったか
	bool m_isHitPlayerBullet;
	// 一度でもノックバックが完了したらノックバック処理をしないためのフラグ
	bool m_isKnockBack;
	// 攻撃中か
	bool m_isAttack;
};
#endif //BOSS_AI_DEFINED
