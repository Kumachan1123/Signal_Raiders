/*
	@file	TitleMenu.h
	@brief	メニュークラス
*/
#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <Mouse.h>
#include <vector>
#include "Keyboard.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Libraries/MyLib/InputManager.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"



class CommonResources;
class MousePointer : public IMenuUI
{
public:


	//	関数
public:
	MousePointer();
	~MousePointer();

	void Initialize(CommonResources* resources, int width, int height)override;
	void Update(const UpdateContext& context)override
	{
		Update(context.elapsedTime);

	}
	void Render()override;

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type)override;
private:
	void Update(float elapsedTime);
	//	変数
private:
	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::unique_ptr<UI> m_pMousePointer;// マウスポインター

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

};