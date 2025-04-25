//--------------------------------------------------------------------------------------
// File: BulletGauge.h
//
// 弾ゲージクラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IUI.h"
class BulletGauge : public IUI
{

	//変数
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	std::unique_ptr<PlayerUI> m_gauge;


	const wchar_t* m_baseTexturePath;



	int m_windowWidth, m_windowHeight;

	//関数
public:
	BulletGauge();
	~BulletGauge();

	void Initialize(CommonResources* resources, int width, int height)override;
	void Update(const UpdateContext& context)override
	{
		Update(context.bulletPoint);

	};
	void Render()override;

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
private:
	void Update(float bulletPoint);

};
