/*
*	@file	EnemyAI.h
*	@brief	敵AIクラス
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
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
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
public:// アクセサ
	// 現在の位置を取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 現在の位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 初期位置を取得
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	// 回転情報を取得（クォータニオン）
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// 回転情報を設定（クォータニオン）
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// 現在の速度ベクトルを取得
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// 現在の速度ベクトルを設定
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
	// スケール情報を取得
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// スケール情報を設定
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// 現在の敵の状態を取得
	IState::EnemyState GetState() const { return m_enemyState; }
	// 敵の状態を設定
	void SetState(IState::EnemyState state) { m_enemyState = state; }
	// 攻撃可能フラグを設定
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; }
	// プレイヤーの弾に当たったかのフラグを取得
	bool GetHitPlayerBullet() const { return m_isHitPlayerBullet; }
	// プレイヤーの弾に当たったかのフラグを設定
	void SetHitPlayerBullet(bool hit) { m_isHitPlayerBullet = hit; }
	// 敵の攻撃クラスを取得
	EnemyAttack* GetEnemyAttack() const { return m_pEnemyAttack.get(); }
	// 敵の徘徊クラスを取得
	EnemyIdling* GetEnemyIdling() const { return m_pEnemyIdling.get(); }
	// 敵の回転クラス（スピン）を取得
	EnemySpin* GetEnemySpin() const { return m_pEnemySpin.get(); }
	// 実体となる敵キャラの情報を取得
	IEnemy* GetEnemy() const { return m_pEnemy; }
	// 現在のステート（状態クラス）を取得
	IState* GetNowState() const { return m_pCurrentState; }
	// 攻撃中かどうかを取得
	bool GetisAttack() const { return m_pEnemy->GetIsAttack(); }
	// 攻撃中フラグを設定
	void SetIsAttack(bool isAttack) { m_pEnemy->SetIsAttack(isAttack); }
public:// publicメンバ関数
	// コンストラクタ
	EnemyAI(IEnemy* pEnemy);
	// デストラクタ
	~EnemyAI();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 状態変更
	void ChangeState(IState* newState);
private:// privateメンバ変数
	// 徘徊状態のポインター
	std::unique_ptr<EnemyIdling> m_pEnemyIdling;
	// 攻撃状態のポインター
	std::unique_ptr<EnemyAttack> m_pEnemyAttack;
	// スピン状態のポインター
	std::unique_ptr<EnemySpin>	 m_pEnemySpin;
	// 現在の状態
	IState* m_pCurrentState;
	// 敵の状態
	IState::EnemyState m_enemyState;
	// 敵のポインター
	IEnemy* m_pEnemy;
	// 敵の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 敵の座標初期値
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// 敵の当たり判定用座標
	DirectX::SimpleMath::Vector3 m_bsPosition;
	// 敵のスケール
	DirectX::SimpleMath::Vector3 m_scale;
	// 敵の回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// 敵の移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// ノックバックする時間
	float m_knockTime;
	// ノックバックの開始位置
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// ノックバックの終了位置
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// ノックバックの初期速度
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	// 時間
	float  m_time;
	// 攻撃のクールダウンタイム
	float m_attackCooldown;
	// プレイヤーの弾に当たったか
	bool m_isHitPlayerBullet;
	// 攻撃可能か
	bool m_canAttack;
	// 攻撃中か
	bool m_isAttack;
};
#endif //ENEMY_AI_DEFINED
