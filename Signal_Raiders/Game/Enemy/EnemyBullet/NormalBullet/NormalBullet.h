#pragma once
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
class NormalBullet : public IEnemyBullet
{
public:// ŠÖ”
	NormalBullet();
	~NormalBullet();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
private:// •Ï”
	EnemyBullet* m_pEnemyBullet;					// “G’eƒ|ƒCƒ“ƒ^[
	DirectX::SimpleMath::Vector3 m_position;		// ’e‚ÌÀ•W
	DirectX::SimpleMath::Vector3 m_velocity;		// ’e‚Ì‘¬‚³
	DirectX::SimpleMath::Vector3 m_direction;		// ’e‚ª”ò‚Ô•ûŒü
	DirectX::SimpleMath::Vector3 m_target;			// ’e”­Ë‚Ìƒ^[ƒQƒbƒg‚ÌˆÊ’u
	DirectX::BoundingSphere m_boundingSphere;		// u’ev‹«ŠE‹…
};