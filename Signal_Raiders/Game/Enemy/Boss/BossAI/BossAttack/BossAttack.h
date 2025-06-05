/*
*	@file	BossAttack.h
*	@brief	ボス攻撃クラス
*/
#pragma once
#ifndef BOSS_ATTACK_DEFINED
#define BOSS_ATTACK_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>
#include <cmath>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"

//前方宣言
class CommonResources;
class BossAI;

// ボス攻撃クラス
class BossAttack : public IState
{
public:
	// アクセサ
	// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 座標設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 回転取得
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// 回転設定
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// クールタイム取得
	float GetCoolTime() const { return m_attackCooldown; }
	// クールタイム設定
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }
	// スケール設定
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
	// 回転速度設定
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
public:
	// publicメンバ関数
	// コンストラクタ
	BossAttack(BossAI* pBoss);
	// デストラクタ
	~BossAttack();
	// 初期化
	void Initialize() override;
	// 更新
	void Update(float elapsedTime) override;
private:
	// privateメンバ関数
	// プレイヤーの方向に回転
	void RotateTowardsPlayer(const DirectX::SimpleMath::Vector3& playerPos);
	// プレイヤーの方向に移動
	void MoveTowardsPlayer(float elapsedTime, const DirectX::SimpleMath::Vector3& playerPos);
	// 攻撃クールダウンの管理
	void ManageAttackCooldown(float elapsedTime);
private:
	// private定数
	// 回転速度
	const float ROTATION_SPEED = 5.5f;
	// ランダムの最大値
	const float RANDOM_MAX = 2.0f;
	// ランダムの最小値
	const float RANDOM_MIN = 0.5f;
	// 攻撃間隔
	const float ATTACK_INTERVAL = 1.0f;
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 敵AI
	BossAI* m_pBoss;
	// 移動
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// サイズ
	DirectX::SimpleMath::Vector3 m_scale;
	// 攻撃のクールダウンタイム
	float m_attackCooldown;
	// 回転速度
	float m_rotationSpeed;
};
#endif //BOSS_ATTACK_DEFINED
