#pragma once
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
class NormalBullet : public IEnemyBullet
{
public:// 関数
	NormalBullet();
	~NormalBullet();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
private:// 変数
	EnemyBullet* m_pEnemyBullet;					// 敵弾ポインター
	DirectX::SimpleMath::Vector3 m_position;		// 弾の座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 弾の速さ
	DirectX::SimpleMath::Vector3 m_direction;		// 弾が飛ぶ方向
	DirectX::SimpleMath::Vector3 m_target;			// 弾発射時のターゲットの位置
	DirectX::SimpleMath::Vector3 m_toPlayer;		// プレイヤーの方向ベクトル
	DirectX::BoundingSphere m_boundingSphere;		// 「弾」境界球
};