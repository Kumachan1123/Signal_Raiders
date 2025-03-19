/*
	@file	SettingMenu.h
	@brief	�Z�b�e�B���O���j���[�N���X
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
class SettingMenu
{
public:
	enum StateID
	{
		BGM = 0,
		SE = 1,
		MOUSE = 2

	};
	enum SelectID
	{
		NONE = -1,
		APPLY = 0,
		END = 1
	};
	enum class UIType
	{
		// �I���\UI
		SELECT,
		// �I��s�\UI
		NON_SELECT
	};
	//	�ϐ�
private:

	int m_menuIndex;// ���j���[�̃C���f�b�N�X
	DX::DeviceResources* m_pDR;// �f�o�C�X���\�[�X
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	std::vector<std::unique_ptr<UI>> m_pUI;// UI
	std::vector<std::unique_ptr<UI>> m_pGuide;// �I��s�\UI
	std::vector<std::unique_ptr<UI>> m_pSelect;// �I��

	const wchar_t* m_pSelectTexturePath;// �I���e�N�X�`���p�X

	std::unique_ptr<UI> m_pSelectWindow;// �I���E�B���h�E

	int m_windowWidth, m_windowHeight;// �E�B���h�E�̕��ƍ���

	DirectX::Keyboard::KeyboardStateTracker m_tracker;// �g���b�J�[


	SelectID m_selectNum;// �I��ID
	float m_time = 0;// ����
	//	�֐�
public:
	SettingMenu();
	~SettingMenu();

	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type);

public:
	SelectID GetSelectIDNum() const { return m_selectNum; }
	void SetSelectIDNum(SelectID num) { m_selectNum = num; }
	unsigned int GetMenuIndex() const { return m_menuIndex; }
};