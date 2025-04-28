/*
	@file	TitleMenu.h
	@brief	���j���[�N���X
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
	//	// �I���\UI
	//	SELECT,
	//	// �I��s�\UI
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
	//	�֐�
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
	//	�ϐ�

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;
	CommonResources* m_commonResources;
	std::vector<std::unique_ptr<UI>> m_pUI;// �I���\UI
	std::vector<std::unique_ptr<UI>> m_pGuide;// �I��s�\UI
	std::vector<std::unique_ptr<UI>> m_pSelect;// �I�𒆂�UI�ɕ\������摜

	const wchar_t* m_pSelectTexturePath;

	std::unique_ptr<UI> m_pSelectWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
	std::vector<UITransForm> m_transforms;
	SceneID m_num;// �V�[���ԍ�
	float m_time = 0;// ����
	bool m_hit;// �����������ǂ���

};