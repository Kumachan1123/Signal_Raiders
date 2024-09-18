/*
	@file	Menu.h
	@brief	���j���[�N���X
*/
#pragma once

#include "StepTimer.h"
#include "Game/Scene/TitleScene/TitleUI/TitleUI.h"
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
class CommonResources;
class Menu
{
public:
	enum SceneID
	{
		PLAY = 0,
		SETTING,
		END
	};
	//	�ϐ�
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::vector<std::unique_ptr<TitleUI>> m_pUI;
	std::vector<std::unique_ptr<TitleUI>> m_pSelect;

	const wchar_t* m_pSelectTexturePath;

	std::unique_ptr<TitleUI> m_pSelectWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	SceneID m_num = SceneID::PLAY;
	float m_time = 0;
	//	�֐�
public:
	Menu();
	~Menu();

	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();

	void Add(const wchar_t* path
			 , DirectX::SimpleMath::Vector2 position
			 , DirectX::SimpleMath::Vector2 scale
			 , kumachi::ANCHOR anchor);

public:
	SceneID GetSceneNum() const { return m_num; }
	void SetSceneNum(SceneID num) { m_num = num; }
};