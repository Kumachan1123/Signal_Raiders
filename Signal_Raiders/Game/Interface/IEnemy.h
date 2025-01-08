#pragma once
#ifndef IENEMY_DEFINED
#define IENEMY_DEFINED
#include <SimpleMath.h>
#include "Game/KumachiLib/AudioManager/AudioManager.h"
class Player;
class FPS_Camera;
class CommonResources;
class IEnemy
{
protected:
	Player* m_pPlayer;
public:
	//	getter										  
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() = 0;
	virtual const  DirectX::SimpleMath::Vector3& GetPosition() = 0;
	virtual void SetPosition(DirectX::SimpleMath::Vector3& pos) = 0;
	virtual const DirectX::SimpleMath::Vector3& GetVelocity() = 0;
	virtual const DirectX::SimpleMath::Vector3& GetRotate() = 0;
	virtual int GetEnemyHP() const = 0;
	virtual void SetEnemyHP(int hp) = 0;
	virtual bool GetEnemyIsDead() const = 0;
	virtual void SetEnemyIsDead(bool isDead) = 0;
	virtual bool GetHitToPlayer()const = 0;
	virtual void SetHitToPlayer(bool isHitToPlayer) = 0;
	virtual bool GetHitToOtherEnemy() const = 0;
	virtual void SetHitToOtherEnemy(bool isHitToOtherEnemy) = 0;
	virtual void SetPlayerHP(float& HP) const = 0;
	virtual bool GetPlayerHitByEnemyBullet() const = 0;
	virtual void SetPlayerHitByEnemyBullet(bool hit) = 0;
	virtual bool GetEnemyHitByPlayerBullet()const = 0;
	virtual void SetEnemyHitByPlayerBullet(bool hit) = 0;
	virtual float GetToPlayerDamage() const = 0;
	virtual bool GetCanAttack() const = 0;
	virtual void SetCanAttack(bool canAttack) = 0;
	virtual void SetCameraEye(DirectX::SimpleMath::Vector3 eye) = 0;
	virtual void SetCameraTarget(DirectX::SimpleMath::Vector3 target) = 0;
	virtual void SetCameraUp(DirectX::SimpleMath::Vector3 up) = 0;
	virtual const DirectX::BoundingSphere& GetBoundingSphere() = 0;
	virtual void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) = 0;
	virtual const DirectX::BoundingSphere& GetBulletBoundingSphere() = 0;
	virtual void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) = 0;
	virtual const DirectX::BoundingSphere& GetPlayerBoundingSphere() = 0;
	virtual Player* GetPlayer()const = 0;
	virtual FPS_Camera* GetCamera()const = 0;
	virtual void SetAudioManager(AudioManager* audioManager) = 0;
public:
	IEnemy(Player* pPlayer) : m_pPlayer(pPlayer) {}
	virtual ~IEnemy() = default;
	virtual void Initialize(CommonResources* resources, int hp) = 0;

	virtual void Update(float elapsedTime) = 0;
	virtual	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	virtual void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;


};
#endif		// IENEMY_DEFINED
