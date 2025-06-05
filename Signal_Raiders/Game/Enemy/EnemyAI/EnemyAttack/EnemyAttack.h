/*
*	@file	EnemyAttack.h
*	@brief	敵Attackクラス
*/
#pragma once
#ifndef ENEMY_ATTACK_DEFINED
#define ENEMY_ATTACK_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <cassert>
#include <random>  
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
//前方宣言
class CommonResources;
class EnemyAI;
class EnemyAttack : public IState
{
public:	// アクセサ
	// 現在の位置を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 位置を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 現在の回転を取得する
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// 回転を設定する
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// 攻撃のクールタイムを取得する
	float GetCoolTime() const { return m_attackCooldown; }
	// 攻撃のクールタイムを設定する
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }
	// モデルのスケールを設定する
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// 回転スピードを設定する
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
public:// publicメンバ関数
	// コンストラクタ
	EnemyAttack(EnemyAI* enemyAI);
	// デストラクタ
	~EnemyAttack();
	// 初期化する
	void Initialize() override;
	// 更新する
	void Update(float elapsedTime) override;
private:
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 敵AI
	EnemyAI* m_pEnemyAI;
	// 移動
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// 初期位置
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// サイズ
	DirectX::SimpleMath::Vector3 m_scale;
	// 回転速度
	float m_rotationSpeed;
	// 攻撃のクールダウンタイム
	float m_attackCooldown;
	// 時間
	float m_time;
};
#endif //ENEMY_ATTACK_DEFINED
