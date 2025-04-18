#pragma once
#ifndef IBOSS_DEFINED
#define IBOSS_DEFINED
// 自作ヘッダーファイル
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/Boss/BossSheild/BossSheild.h"
class IBoss : public IEnemy
{
public:
	virtual ~IBoss() = default;
	// アクセサ
	virtual void SetBulletCooldown(float cooldown) = 0;// シールドを展開した後に実行する
	virtual void SetInitSpecialAttacCooldown(float cooldown) = 0;// 特殊攻撃のクールダウン
	virtual void PlayBarrierSE() = 0;// バリアSE再生
	virtual DirectX::SimpleMath::Vector3 GetCameraEye()const = 0;// カメラの位置
	virtual DirectX::SimpleMath::Vector3 GetCameraTarget()const = 0;// カメラのターゲット
	virtual DirectX::SimpleMath::Vector3 GetCameraUp()const = 0;// カメラの上ベクトル
};
#endif //IBOSS_DEFINED