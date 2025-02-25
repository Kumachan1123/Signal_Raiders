#pragma once
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
#include "Game/KumachiLib/KumachiLib.h"
class SpecialBullet : public IEnemyBullet
{
public:// ŠÖ”
	SpecialBullet();
	~SpecialBullet();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
private:
	// ‰ñ“]’e‚ğ“WŠJ‚·‚é
	void Expand();
	// ‰ñ“]’e‚ğ”­Ë‚·‚é
	void Shot();
	// ‰ñ“]’e‚Ì“WŠJ‚ğ‚â‚ß‚é
	void StopExpand();
	// ‰ñ“]’e‚ğ©•ª‚Ìü‚è‚É–ß‚·
	void ComeBack();
private:// •Ï”
	EnemyBullet* m_pEnemyBullet;					// “G’eƒ|ƒCƒ“ƒ^[
	DirectX::SimpleMath::Vector3 m_position;		// ’e‚ÌÀ•W
	DirectX::SimpleMath::Vector3 m_velocity;		// ’e‚Ì‘¬‚³
	DirectX::SimpleMath::Vector3 m_direction;		// ’e‚ª”ò‚Ô•ûŒü
	DirectX::SimpleMath::Vector3 m_target;			// ’e”­Ë‚Ìƒ^[ƒQƒbƒg‚ÌˆÊ’u
	DirectX::SimpleMath::Vector3 m_currentTarget;	// Œ»İ‚Ìƒ^[ƒQƒbƒg‚ÌˆÊ’u
	DirectX::BoundingSphere m_boundingSphere;		// u’ev‹«ŠE‹…
	float m_spiralAngle;							// ‰ñ“]’e‚ÌŠp“x
	float m_elapsedTime;							// Œo‰ßŠÔ
	float m_time;									// ŠÔ
	DirectX::SimpleMath::Vector3 m_basePos;			// ’e‚ÌŠî€ˆÊ’u
	float m_rotationSpeed;							// ’e‚Ì‰ñ“]‘¬“x
	float m_distance;								// ’e‚Ì“G‚Æ‚Ì‹——£
	float m_height;									// ’e‚Ì‚‚³
	DirectX::SimpleMath::Vector3 m_positionOffSet;	// ’e‚ÌˆÊ’uƒIƒtƒZƒbƒg

};