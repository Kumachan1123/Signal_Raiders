/*
*	@file SpeedBullet.h
*	@brief 敵弾クラス(速い弾)
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"

class SpeedBullet : public IEnemyBullet
{
public:
	// public関数
	SpeedBullet();// コンストラクタ
	~SpeedBullet();// デストラクタ
	void Initialize() override;// 初期化関数
	void Update(float elapsedTime) override;// 更新関数
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }// 敵弾ポインターをセットする関数
	void SetCommonResources(CommonResources* commonResources) override { m_pCommonResources = commonResources; }// 共通リソースをセットする関数
	void SetSEVolume(float volume) override { m_seVolume = volume; } // SE音量をセットする関数
private:
	// privateメンバ変数
	CommonResources* m_pCommonResources;				// 共通リソース
	EnemyBullet* m_pEnemyBullet;					// 敵弾ポインター
	DirectX::SimpleMath::Vector3 m_position;		// 弾の座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 弾の速さ
	DirectX::SimpleMath::Vector3 m_direction;		// 弾が飛ぶ方向
	DirectX::SimpleMath::Vector3 m_target;			// 弾発射時のターゲットの位置
	DirectX::BoundingSphere m_boundingSphere;		// 「弾」境界球
	float m_seVolume;								// SE音量

};