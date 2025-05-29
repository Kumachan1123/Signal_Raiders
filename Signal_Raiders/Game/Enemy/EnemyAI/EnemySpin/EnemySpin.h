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
class EnemySpin : public IState
{
public:
	// アクセサ
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 位置取得
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// 位置設定
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// 回転取得
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// 回転設定
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; } // スケール設定
public:
	// publicメンバ関数
	EnemySpin(EnemyAI* enemyAI);// コンストラクタ
	~EnemySpin();// デストラクタ
	void Initialize() override;// 初期化
	void Update(float elapsedTime) override;// 更新
private:
	void UpdateSpin(float elapsedTime);// スピンの更新
	void UpdateKnockBack(float elapsedTime);// ノックバックの更新
private:
	EnemyAI* m_pEnemyAI;//敵AI
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Vector3 m_initialPosition;// 座標初期値
	DirectX::SimpleMath::Vector3 m_scale;//サイズ
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	DirectX::SimpleMath::Vector3 m_knockStartPosition;// ノックバック開始位置
	DirectX::SimpleMath::Vector3 m_knockEndPosition;// ノックバック終了位置
	DirectX::SimpleMath::Vector3 m_initialVelocity;// ノックバックの初期速度
	float m_angle;//角度
	float  m_time;  // 時間の初期化
	float m_knockTime;// ノックバックする時間
};
#endif //ENEMY_SPIN_DEFINED