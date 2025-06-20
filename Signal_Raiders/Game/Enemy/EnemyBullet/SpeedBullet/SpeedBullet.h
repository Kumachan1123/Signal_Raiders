/*
*	@file SpeedBullet.h
*	@brief 敵弾クラス(速い弾)
*/
#pragma once
#ifndef SPEED_BULLET_DEFINED
#define SPEED_BULLET_DEFINED
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"

// 敵弾クラス(速い弾)
class SpeedBullet : public IEnemyBullet
{
public:
	// public関数
	// コンストラクタ
	SpeedBullet();
	// デストラクタ
	~SpeedBullet();
	// 初期化関数
	void Initialize() override;
	// 更新関数
	void Update(float elapsedTime) override;
	// 敵弾ポインターをセットする関数
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
	// 共通リソースをセットする関数
	void SetCommonResources(CommonResources* commonResources) override { m_pCommonResources = commonResources; }
	// SE音量をセットする関数
	void SetSEVolume(float volume) override { m_seVolume = volume; }
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 敵弾ポインター
	EnemyBullet* m_pEnemyBullet;
	// 弾の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 弾の速さ
	DirectX::SimpleMath::Vector3 m_velocity;
	// 弾が飛ぶ方向
	DirectX::SimpleMath::Vector3 m_direction;
	// 弾発射時のターゲットの位置
	DirectX::SimpleMath::Vector3 m_target;
	// 弾の境界球
	DirectX::BoundingSphere m_boundingSphere;
	// SE音量
	float m_seVolume;
};
#endif // SPEED_BULLET_DEFINED