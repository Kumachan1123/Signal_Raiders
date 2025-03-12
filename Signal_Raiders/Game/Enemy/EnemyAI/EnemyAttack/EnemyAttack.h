/*
	@file	EnemyAttack.h
	@brief	敵Attackクラス
*/

#pragma once
#ifndef ENEMY_ATTACK_DEFINED
#define ENEMY_ATTACK_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <cassert>
#include <random>  
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// 自作ヘッダーファイル
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
public:
	// アクセサ
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 位置を取得
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// 位置を設定
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// 回転を取得
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// 回転を設定
	float GetCoolTime() const { return m_attackCooldown; }// クールタイムを取得
	void SetCoolTime(float coolTime) { m_attackCooldown = coolTime; }// クールタイムを設定
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }// サイズを設定
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }// 回転速度を設定
public:
	// publicメンバ関数
	EnemyAttack(EnemyAI* enemyAI);// コンストラクタ
	~EnemyAttack();	// デストラクタ
	void Initialize() override;// 初期化する
	void Update(float elapsedTime) override;// 更新する
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// 敵AI
	EnemyAI* m_enemyAI;
	// 移動
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// サイズ
	DirectX::SimpleMath::Vector3 m_scale;
	// 回転速度
	float m_rotationSpeed;
	// 攻撃のクールダウンタイム
	float m_attackCooldown;
};
#endif //ENEMY_ATTACK_DEFINED
