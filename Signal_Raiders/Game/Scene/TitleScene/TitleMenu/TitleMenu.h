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
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"


class CommonResources;
class TitleMenu : public IMenuUI
{
public:
	enum SceneID
	{
		STAGESELECT = 0,
		SETTING,
		END
	};

	//enum class UIType
	//{
	//	// 選択可能UI
	//	SELECT,
	//	// 選択不可能UI
	//	NON_SELECT
	//};

	struct UITransForm
	{
		DirectX::SimpleMath::Vector2 position;
		DirectX::SimpleMath::Vector2 scale;
	};


public:
	SceneID GetSceneNum() const { return m_num; }
	void SetSceneNum(SceneID num) { m_num = num; }
	bool GetIsHit()const { return m_hit; }
	void SetIsHit(bool hit) { m_hit = hit; }
	//	関数
public:
	TitleMenu();
	~TitleMenu();

	void Initialize(CommonResources* resources, int width, int height);

	void Update(const UpdateContext& context)override
	{
		Update(context.elapsedTime);

	}
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type);
private:
	void Update(float elapsedTime);
private:
	//	変数

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::vector<std::unique_ptr<UI>> m_pUI;// 選択可能UI
	std::vector<std::unique_ptr<UI>> m_pGuide;// 選択不可能UI
	std::vector<std::unique_ptr<UI>> m_pSelect;// 選択中のUIに表示する画像

	const wchar_t* m_pSelectTexturePath;

	std::unique_ptr<UI> m_pSelectWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
	std::vector<UITransForm> m_transforms;
	SceneID m_num;// シーン番号
	float m_time = 0;// 時間
	bool m_hit;// 当たったかどうか

};