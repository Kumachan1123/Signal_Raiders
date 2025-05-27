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
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IState.h"

//前方宣言
class CommonResources;
class BossAI;

class BossAttack : public IState
{
public:
	// アクセサ
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 座標取得
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// 座標設定
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// 回転取得
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// 回転設定
	float GetCoolTime() const { return m_attackCooldown; }// クールタイム取得
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }// クールタイム設定
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }// スケール設定
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }// 回転速度設定
public:
	// publicメンバ関数
	BossAttack(BossAI* pBoss);// コンストラクタ
	~BossAttack();// デストラクタ
	void Initialize() override;// 初期化
	void Update(float elapsedTime) override;// 更新
private:
	// privateメンバ関数
	void RotateTowardsPlayer(DirectX::SimpleMath::Vector3 playerPos);// プレイヤーの方向に回転
	void MoveTowardsPlayer(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos);// プレイヤーの方向に移動
	void ManageAttackCooldown(float elapsedTime);// 攻撃クールダウンの管理
private:
	// private定数
	const float ROTATION_SPEED = 5.5f;// 回転速度
	const float RANDOM_MAX = 2.0f;// ランダムの最大値
	const float RANDOM_MIN = 0.5f;// ランダムの最小値
	const float ATTACK_INTERVAL = 1.0f; // 攻撃間隔
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
