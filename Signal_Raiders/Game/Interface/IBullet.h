#pragma once
#ifndef IBULLET_DEFINED
#define IBULLET_DEFINED
#include <SimpleMath.h>

class CommonResources;
class IBullet
{
public:// ŠÖ”
	virtual ~IBullet() = default;
	virtual void Initialize(CommonResources* resources) = 0;// ‰Šú‰»
	virtual void Update(float elapsedTime) = 0;// XV
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// •`‰æ
	virtual void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// ‰e‚Ì•`‰æ
};
#endif		// IBULLET_DEFINED
