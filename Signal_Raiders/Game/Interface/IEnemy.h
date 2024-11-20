#pragma once
#ifndef IENEMY_DEFINED
#define IENEMY_DEFINED
#include <SimpleMath.h>
class Player;
class FPS_Camera;
class CommonResources;
class IEnemy
{
protected:
	Player* m_pPlayer;
public:
	//	getter										  
	virtual DirectX::BoundingSphere& GetBoundingSphere() = 0;
	virtual DirectX::BoundingSphere& GetBulletBoundingSphere() = 0;
	virtual DirectX::BoundingSphere& GetPlayerBoundingSphere() = 0;
	virtual DirectX::SimpleMath::Matrix GetMatrix() const = 0;
	virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;
	virtual DirectX::SimpleMath::Vector3 GetVelocity() const = 0;
	virtual DirectX::SimpleMath::Vector3 GetRotate() const = 0;
	virtual Player* GetPlayer()const = 0;
	virtual FPS_Camera* GetCamera()const = 0;

	virtual int GetHP() const = 0;
	virtual bool GetEnemyIsDead() const = 0;
	virtual bool GetHitToPlayer()const = 0;
	virtual bool GetHitToOtherEnemy() const = 0;
	virtual bool GetBulletHitToPlayer() const = 0;
	virtual bool GetHitToPlayerBullet()const = 0;
	virtual float GetToPlayerDamage() const = 0;
	// setter
	virtual void SetPosition(DirectX::SimpleMath::Vector3& pos) = 0;
	virtual void SetEnemyHP(int hp) = 0;
	virtual void SetEnemyIsDead(bool isDead) = 0;
	virtual void SetHitToPlayer(bool isHitToPlayer) = 0;
	virtual void SetHitToOtherEnemy(bool isHitToOtherEnemy) = 0;
	virtual void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) = 0;
	virtual void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) = 0;
	virtual void SetPlayerHP(float& HP) const = 0;
	virtual void SetBulletHitToPlayer(bool hit) = 0;
	virtual void SetHitToPlayerBullet(bool hit) = 0;
	virtual void SetCameraEye(DirectX::SimpleMath::Vector3 eye) = 0;
	virtual void SetCameraTarget(DirectX::SimpleMath::Vector3 target) = 0;
	virtual void SetCameraUp(DirectX::SimpleMath::Vector3 up) = 0;
public:
	IEnemy(Player* pPlayer) : m_pPlayer(pPlayer) {}
	virtual ~IEnemy() = default;
	virtual void Initialize(CommonResources* resources, int hp) = 0;

	virtual void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos) = 0;
	virtual	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	virtual void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B) = 0;
	virtual void CheckHitWall(DirectX::BoundingSphere& A, DirectX::BoundingBox& B) = 0;


};
#endif		// IENEMY_DEFINED
