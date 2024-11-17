#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED
#include <SimpleMath.h>
// IStateインタフェースを定義する(GOF Stateパターン)
class EnemyAI;

class IState
{
public:
	enum class EnemyState
	{
		IDLING = 0,
		ATTACK,
		ANGRY,
		DAMAGE
	};
public:
	virtual ~IState() = default;
	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer) = 0;
};
#endif		// ISTATE_DEFINED
