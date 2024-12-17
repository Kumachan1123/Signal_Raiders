/*
	@file	ResultMenu.h
	@brief	リザルトメニュークラス
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
#include <vector>
#include "Keyboard.h"
class CommonResources;
class ResultMenu
{
public:
	enum SceneID
	{
		REPLAY = 0,
		END
	};
	//	変数
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::vector<std::unique_ptr<UI>> m_pUI;
	std::vector<std::unique_ptr<UI>> m_pSelect;

	const wchar_t* m_pSelectTexturePath;

	std::unique_ptr<UI> m_pSelectWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	SceneID m_num = SceneID::REPLAY;
	float m_time = 0;
	//	関数
public:
	ResultMenu();
	~ResultMenu();

	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);

public:
	SceneID GetSceneNum() const { return m_num; }
	void SetSceneNum(SceneID num) { m_num = num; }
};