/*
*	@file	EnemyShadowStep.h
*	@brief	敵シャドウステップクラス
*/
#pragma once
#ifndef ENEMY_SHADOWSTEP_DEFINED
#define ENEMY_SHADOWSTEP_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>  
// DirectX
#include <SimpleMath.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ライブラリ
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"

//前方宣言
class EnemyAI;

// 敵シャドウステップクラス
class EnemyShadowStep : public IState
{
public:
	// アクセサ
	// 現在の位置を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 位置を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 現在の回転を取得する
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// 回転を設定する
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// モデルのスケールを設定する
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
public:
	// publicメンバ関数
	// コンストラクタ
	EnemyShadowStep(EnemyAI* enemyAI);
	// デストラクタ
	~EnemyShadowStep();
	// 初期化
	void Initialize() override;
	// 更新
	void Update(float elapsedTime) override;
private:
	// privateメンバ変数
	// 敵AI
	EnemyAI* m_pEnemyAI;
	// 現在の位置
	DirectX::SimpleMath::Vector3 m_position;
	// 初期位置
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// スケール
	DirectX::SimpleMath::Vector3 m_scale;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// ノックバック開始位置
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// ノックバック終了位置
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// ノックバックの初期速度
	DirectX::SimpleMath::Vector3 m_initialVelocity;
	// 回転角度
	float m_angle;
	// 経過時間
	float m_time;
	// ターゲット位置
	DirectX::SimpleMath::Vector3 m_targetPos;
	// プレイヤーの初期位置
	DirectX::SimpleMath::Vector3 m_centerPos;
	// 初期化フラグ
	bool m_initialized = false;
	// 累積角度
	float m_elapsedAngle = 0.0f;
	// +1 なら反時計回り, -1 なら時計回り
	int m_rotateDirection = 1;
	// ランダムな半径
	float m_randomRadius = 0.0f;
	// 回り込みの半径
	float m_evadeRadius = 0.0f;
};
#endif //ENEMY_SHADOWSTEP_DEFINED