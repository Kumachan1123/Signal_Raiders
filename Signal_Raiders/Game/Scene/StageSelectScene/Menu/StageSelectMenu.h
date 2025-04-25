/*
	@file	StageSelectMenu.h
	@brief	���U���g���j���[�N���X
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
class StageSelectMenu : public IMenuUI
{
public:
	enum SceneID
	{
		PLAY = 0,
		END
	};
	//enum class UIType
	//{
	//	// �I���\UI
	//	SELECT,
	//	// �I��s�\UI
	//	NON_SELECT
	//};
	//	�ϐ�
public:
	unsigned int m_menuIndex;
private:


	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::vector<std::unique_ptr<UI>> m_pUI;// �I���\UI
	std::vector<std::unique_ptr<UI>> m_pGuide;// �I��s�\UI
	std::vector<std::unique_ptr<UI>> m_pSelect;// �I��UI

	const wchar_t* m_pSelectTexturePath;

	std::unique_ptr<UI> m_pSelectWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	SceneID m_num = SceneID::PLAY;
	float m_time = 0;
	//	�֐�
public:
	StageSelectMenu();
	~StageSelectMenu();

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
		, UIType type);

public:
	SceneID GetSceneNum() const { return m_num; }
	void SetSceneNum(SceneID num) { m_num = num; }
	int GetMenuIndex() const { return m_menuIndex; }
	void SetMenuIndex(int index) { m_menuIndex = index; }
private:
	void Update(float elapsedTime);
};