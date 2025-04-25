//--------------------------------------------------------------------------------------
// File: PlayerHP.h
//
// HPゲージクラス
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

class PlayerHP : public IUI
{


	//変数
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	std::unique_ptr<PlayerUI> m_heart;
	std::unique_ptr<PlayerUI> m_gauge;
	std::unique_ptr<PlayerUI> m_frame;
	std::unique_ptr<PlayerUI> m_base;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<PlayerUI> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
	float m_maxHP;// 最大HP(途中で変動する）
	float m_currentHP;// 現在のHP

	//関数
public:
	PlayerHP();
	~PlayerHP();

	void Initialize(CommonResources* resources, int width, int height)override;
	void Update(const UpdateContext& context)override
	{
		Update(context.playerHP);
	};
	void Render()override;
	void SetMaxHP(float maxHP) { m_maxHP = maxHP; }
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
private:
	void Update(float playerHP);
};
