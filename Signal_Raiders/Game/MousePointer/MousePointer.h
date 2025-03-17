/*
	@file	TitleMenu.h
	@brief	���j���[�N���X
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


	//	�֐�
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
	//	�ϐ�

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::unique_ptr<UI> m_pMousePointer;// �}�E�X�|�C���^�[

	const wchar_t* m_pTexturePath;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

};