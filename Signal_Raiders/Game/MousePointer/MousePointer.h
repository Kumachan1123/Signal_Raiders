/*
	@file	TitleMenu.h
	@brief	メニュークラス
*/
#pragma once

#include "StepTimer.h"
#include "Game/UI/UI.h"
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



class CommonResources;
class MousePointer
{
public:


	//	関数
public:
	MousePointer();
	~MousePointer();

	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
private:
	//	変数

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::unique_ptr<UI> m_pMousePointer;// マウスポインター

	const wchar_t* m_pTexturePath;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

};