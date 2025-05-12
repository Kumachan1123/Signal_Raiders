/*
	@file	EnemyIdling.h
	@brief	敵Idlingクラス
*/
#pragma once
#ifndef ENEMY_IDLING_DEFINED
#define ENEMY_IDLING_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>  
#include <SimpleMath.h>
// 外部ライブラリ
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// 自作ヘッダーファイル
#include "Game/Interface/IState.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/KumachiLib.h"
//前方宣言
class EnemyAI;
class EnemyIdling : public IState
{
public:
	// アクセサ
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// 位置取得
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// 位置設定
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }// 回転取得
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_rotation = rot; }// 回転設定
	void SetScale(DirectX::SimpleMath::Vector3 sca) { m_scale = sca; }// サイズ設定
public:
	// publicメンバ関数
	EnemyIdling(EnemyAI* enemyAI);// コンストラクタ
	~EnemyIdling();// デストラクタ
	void Initialize() override;// 初期化
	void Update(float elapsedTime) override;// 更新
private:
	// privateメンバ関数
	EnemyAI* m_enemyAI;//敵AI
	DirectX::SimpleMath::Vector3 m_position;//移動
	DirectX::SimpleMath::Vector3 m_initialPosition;// 座標初期値
	DirectX::SimpleMath::Vector3 m_scale;//サイズ
	DirectX::SimpleMath::Quaternion m_rotation;//回転
	DirectX::SimpleMath::Vector3 m_velocity;// 移動速度
	float m_rotationSpeed;//回転速度
	float  m_time;  // 時間の初期化
};
#endif //ENEMY_IDLING_DEFINED