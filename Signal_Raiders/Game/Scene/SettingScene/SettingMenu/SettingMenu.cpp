/*
	@file	SettingMenu.cpp
	@brief	�Z�b�e�B���O���j���[�N���X
*/
#include "pch.h"
#include "SettingMenu.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Libraries/MyLib/InputManager.h"
#include <Mouse.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

SettingMenu::SettingMenu()
	: m_menuIndex{ 0 }
	, m_time{ 0 }
	, m_pDR{ nullptr }
	, m_commonResources{ nullptr }
	, m_pUI{}
	, m_pSelect{}
	, m_pSelectTexturePath{ nullptr }
	, m_pSelectWindow{ nullptr }
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_tracker{}
	, m_selectNum{ SelectID::NONE }
{
}


SettingMenu::~SettingMenu()
{
}

void SettingMenu::Initialize(CommonResources* resources, int width, int height)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSelectTexturePath = L"Resources/Textures/select.png";

	//  �uBGM�v��ǂݍ���
	Add(L"Resources/Textures/BGM.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y - 300)
		, SimpleMath::Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  �uSE�v��ǂݍ���
	Add(L"Resources/Textures/SE.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y - 150)
		, SimpleMath::Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  �u�}�E�X����ǁv��ǂݍ���
	Add(L"Resources/Textures/Mouse.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y)
		, SimpleMath::Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  �u�ւ񂱂��v��ǂݍ���
	Add(L"Resources/Textures/Apply.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y + 150)
		, SimpleMath::Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u�����v��ǂݍ���
	Add(L"Resources/Textures/end.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y + 300)
		, SimpleMath::Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// �u��������v��ǂݍ���
	Add(L"Resources/Textures/Guide.png"
		, SimpleMath::Vector2(Screen::RIGHT, Screen::BOTTOM)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);
}

void SettingMenu::Update(float elapsedTime)
{
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	m_time += elapsedTime;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();

	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	//  ���j���[�A�C�e���̐������J��Ԃ�
	m_menuIndex = -1;
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		if (m_pUI[i]->IsHit(mousePos))
		{
			//  �͈͓��ɂ���ꍇ�́A�I�𒆂̃A�C�e�����X�V
			m_menuIndex = i;
		}
	}
	if (mtracker->GetLastState().leftButton)
	{
		m_selectNum = static_cast<SelectID>(m_menuIndex);
	}
	//  ���j���[�A�C�e���̑I�����X�V
	for (int i = 0; i < m_pUI.size(); i++)
	{
		////  �A�C�e���̑I����Ԃ��X�V
		//m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		//m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	//  �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	if (m_menuIndex != SelectID::NONE)
	{
		// �I�𒆂̏����T�C�Y���擾����
		Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
		//  �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
		SimpleMath::Vector2 selectScale = SimpleMath::Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), SimpleMath::Vector2::One, 1);
		//  �I����Ԃ͏�����ԁ{50���̑傫���Ƃ���
		select = SimpleMath::Vector2((sin(m_time) * 0.1f) + 1.0f);
		//  �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
		m_pUI[m_menuIndex]->SetScale(select);

	}

}

void SettingMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		//  �A�C�e���p�E�B���h�E�w�i��\��
		//if (i == m_menuIndex) m_pSelect[i]->Render();
		//  ���ۂɕ\���������A�C�e���摜��\��
		m_pUI[i]->Render();
	}
	// �I��s�\�ȃA�C�e���̉摜��\��
	for (unsigned int i = 0; i < m_pGuide.size(); i++)
	{
		m_pGuide[i]->Render();
	}
}

void SettingMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
{
	//  ���j���[�Ƃ��ăA�C�e����ǉ�����
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();
	//  �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	userInterface->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  �A�C�e����V�����ǉ�
	if (type == UIType::SELECT) m_pUI.push_back(std::move(userInterface));
	else
	{
		m_pGuide.push_back(std::move(userInterface));
		return;
	}

	//  �w�i�p�̃E�B���h�E�摜���ǉ�����
	std::unique_ptr<UI> base = std::make_unique<UI>();
	base->Create(m_pDR
		, m_pSelectTexturePath
		, position
		, scale
		, anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

}
