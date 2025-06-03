/*
*	@file	BossKnockBacking.h
*	@brief	ボスのノックバック中の処理クラス
*/
#pragma once
#ifndef BOSS_KNOCKBACKING_DEFINED
#define BOSS_KNOCKBACKING_DEFINED
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
class BossAI;
class BossKnockBacking : public IState
{
public:	// アクセサ
	// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 座標設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 回転取得
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
	// 回転設定
	void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	// スケール設定
	void SetScale(const DirectX::SimpleMath::Vector3& sca) { m_scale = sca; }
public:	// publicメンバ関数
	// コンストラクタ
	BossKnockBacking(BossAI* pBoss);
	// デストラクタ
	virtual ~BossKnockBacking();
	// 初期化
	void Initialize() override;
	// 更新
	void Update(float elapsedTime) override;
private:
	// 敵AI
	BossAI* m_pBoss;
	// 移動
	DirectX::SimpleMath::Vector3 m_position;
	// 座標初期値
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// サイズ
	DirectX::SimpleMath::Vector3 m_scale;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転速度
	float m_rotationSpeed;
	// 時間の初期化
	float  m_time;
	// 角度
	float m_angle;
	// ノックバック時間
	float m_knockTime;
	// ノックバックの開始位置 
	DirectX::SimpleMath::Vector3 m_knockStartPosition;
	// ノックバックの終了位置
	DirectX::SimpleMath::Vector3 m_knockEndPosition;
	// ノックバックの初速度
	DirectX::SimpleMath::Vector3 m_initialVelocity;
};
#endif //BOSS_KNOCKBACKING_DEFINED