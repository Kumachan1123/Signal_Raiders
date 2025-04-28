/*
	@file	ResultMenu.h
	@brief	リザルトメニュークラス
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
#include <vector>
#include "Keyboard.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
class CommonResources;
class ResultMenu : public IMenuUI
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
	std::vector<std::unique_ptr<UI>> m_pGuide;
	std::vector<std::unique_ptr<UI>> m_pSelect;

	const wchar_t* m_pSelectTexturePath;

	std::unique_ptr<UI> m_pSelectWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	SceneID m_num = SceneID::REPLAY;
	float m_time = 0;
	bool m_hit;
	//	関数
public:
	ResultMenu();
	~ResultMenu();

	void Initialize(CommonResources* resources, int width, int height)override;
	void Update(const UpdateContext& context)override
	{
		Update(context.elapsedTime);
	};
	void Render()override;

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type)override;

public:
	SceneID GetSceneNum() const { return m_num; }
	void SetSceneNum(SceneID num) { m_num = num; }
	bool GetIsHit()const { return m_hit; }
	void SetIsHit(bool hit) { m_hit = hit; }
private:
	void Update(float elapsedTime);

};