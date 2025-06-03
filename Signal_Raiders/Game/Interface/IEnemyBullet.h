#pragma once
#ifndef IENEMYBULLET_DEFINED
#define IENEMYBULLET_DEFINED
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
// 前方宣言
class EnemyBullet;
class IEnemyBullet
{
public:// public関数
	virtual ~IEnemyBullet() = default;// デストラクタ
	virtual void Initialize() = 0;// 初期化
	virtual void Update(float elapsedTime) = 0;//	更新
	virtual void SetEnemyBullet(EnemyBullet* pEnemyBullet) = 0;// 敵弾設定
	virtual void SetCommonResources(CommonResources* commonResources) = 0;// 共通リソース設定
	virtual void SetSEVolume(float volume) = 0;// SE音量設定
};
#endif		// IENEMYBULLET_DEFINED
