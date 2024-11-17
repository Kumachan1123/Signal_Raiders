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
		MOUSE = 2,
		APPLY = 3,
		END = 4
	};
	//	�ϐ�
private:

	unsigned int m_menuIndex;// ���j���[�̃C���f�b�N�X
	DX::DeviceResources* m_pDR;// �f�o�C�X���\�[�X
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	std::vector<std::unique_ptr<UI>> m_pUI;// UI
	std::vector<std::unique_ptr<UI>> m_pSelect;// �I��

	const wchar_t* m_pSelectTexturePath;// �I���e�N�X�`���p�X

	std::unique_ptr<UI> m_pSelectWindow;// �I���E�B���h�E

	int m_windowWidth, m_windowHeight;// �E�B���h�E�̕��ƍ���

	DirectX::Keyboard::KeyboardStateTracker m_tracker;// �g���b�J�[


	StateID m_num;// ���ID
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
		, kumachi::ANCHOR anchor);

public:
	StateID GetStateIDNum() const { return m_num; }
	void SetStateIDNum(StateID num) { m_num = num; }
	unsigned int GetMenuIndex() const { return m_menuIndex; }
};