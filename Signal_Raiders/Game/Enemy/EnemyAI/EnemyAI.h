/*
	@file	EnemyAI.h
	@brief	敵AIクラス
*/
#pragma once
#ifndef ENEMY_AI_DEFINED
#define ENEMY_AI_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>
#include <type_traits>
#include <SimpleMath.h>
// 外部ライブラリ
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// 自作ライブラリ
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAttack/EnemyAttack.h"
#include "Game/Enemy/EnemyAI/EnemyIdling/EnemyIdling.h"
#include "Game/Enemy/EnemyAI/EnemySpin/EnemySpin.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Player/Player.h"
//前方宣言
class CommonResources;
class EnemyAttack;
class EnemySpin;
class EnemyIdling;
class IEnemy;
class EnemyAI
{
public:
	// アクセサ
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 位置取得
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; } // 位置設定
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; } // 初期位置取得
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; } // 回転取得
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; } // 回転設定
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; } // 速度取得
	void SetVelocity(DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; } // 速度設定
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; } // スケール取得
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; } // スケール設定
	IState::EnemyState GetState()const { return m_enemyState; } // 状態取得
	void SetState(IState::EnemyState state) { m_enemyState = state; }// 状態設定
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; } // 攻撃可能か
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; } // プレイヤーの弾に当たったか
	EnemyAttack* GetEnemyAttack()const { return m_pEnemyAttack.get(); }// 攻撃クラス取得
	EnemyIdling* GetEnemyIdling()const { return m_pEnemyIdling.get(); } // 徘徊クラス取得
	EnemySpin* GetEnemySpin()const { return m_pEnemySpin.get(); } // 逃避クラス取得
	IEnemy* GetEnemy()const { return m_pEnemy; } // 敵取得
	IState* GetNowState()const { return m_pCurrentState; } // 現在の状態取得
	bool GetisAttack()const { return m_pEnemy->GetIsAttack(); } // 攻撃中か
	void SetIsAttack(bool isAttack) { m_pEnemy->SetIsAttack(isAttack); } // 攻撃中にする
public:
	// publicメンバ関数
	EnemyAI(IEnemy* pEnemy);// コンストラクタ
	~EnemyAI();// デストラクタ
	void Initialize();// 初期化
	void Update(float elapsedTime); // 更新
	void ChangeState(IState* newState); // 状態変更
private:
	// privateメンバ関数
	void KnockBack(float elapsedTime); // ノックバック
private:
	// privateメンバ変数
	std::unique_ptr<EnemyIdling> m_pEnemyIdling;	// 平常時
	std::unique_ptr<EnemyAttack> m_pEnemyAttack;	// 攻撃時
	std::unique_ptr<EnemySpin> m_pEnemySpin;	// スピン
	IState* m_pCurrentState;	// 現在の状態
	IState::EnemyState m_enemyState;// 敵の状態
	IEnemy* m_pEnemy;	// 敵のポインター
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Vector3 m_initialPosition;// 座標初期値
	DirectX::SimpleMath::Vector3 m_bsPosition;//当たり判定用座標
	DirectX::SimpleMath::Vector3 m_scale;//サイズ
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	float m_knockTime;// ノックバックする時間
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	float  m_time;  // 時間の初期化
	float m_attackCooldown;  // 攻撃のクールダウンタイム
	bool m_isHitPlayerBullet;// プレイヤーの弾に当たったか
	bool m_canAttack;// 攻撃可能か
	bool m_isAttack;// 攻撃中か
};
#endif //ENEMY_AI_DEFINED
