/*
*	@file	EnemySpin.h
*	@brief	敵スピンクラス
*/
#pragma once
#ifndef ENEMY_SPIN_DEFINED
#define ENEMY_SPIN_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>  
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

// 敵スピンクラス
class EnemySpin : public IState
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
	EnemySpin(EnemyAI* enemyAI);
	// デストラクタ
	~EnemySpin();
	// 初期化
	void Initialize() override;
	// 更新
	void Update(float elapsedTime) override;
private:
	// privateメンバ関数
	// スピンの更新
	void UpdateSpin(float elapsedTime);
	// ノックバックの更新
	void UpdateKnockBack(float elapsedTime);
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
	// ノックバックの持続時間
	float m_knockTime;
};
#endif //ENEMY_SPIN_DEFINED