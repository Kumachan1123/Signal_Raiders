/*
	@file	SettingMenu.cpp
	@brief	�Z�b�e�B���O���j���[�N���X
*/
#include "pch.h"
#include "SettingMenu.h"
#include "Game/Screen.h"
#include "Game/Scene/TitleScene/TitleUI/TitleUI.h"
#include "Game/KumachiLib/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Libraries/MyLib/InputManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SettingMenu::SettingMenu()
	: m_menuIndex{ 0 }
	, m_pDR{ nullptr }
	, m_commonResources{ nullptr }
	, m_pUI{}
	, m_pSelect{}
	, m_pSelectTexturePath{ nullptr }
	, m_pSelectWindow{ nullptr }
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_tracker{}
	, m_num{ StateID::BGM }
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
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �uSE�v��ǂݍ���
	Add(L"Resources/Textures/SE.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y - 150)
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �u�}�E�X����ǁv��ǂݍ���
	Add(L"Resources/Textures/Mouse.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y)
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �u�ւ񂱂��v��ǂݍ���
	Add(L"Resources/Textures/Apply.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y + 150)
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �u�����v��ǂݍ���
	Add(L"Resources/Textures/end.png"
		, SimpleMath::Vector2(Screen::LEFT + 400, Screen::CENTER_Y + 300)
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);

}

void SettingMenu::Update(float elapsedTime)
{
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	m_time += elapsedTime;
	//  �L�[�{�[�h�̓��͂��擾
	if (kbTracker->pressed.S)
	{
		//  ���L�[����������A�I����1�i�߂�
		m_menuIndex += 1;
		//  ���j���[�A�C�e�����̍ő�l�𒴂��Ȃ��悤�ɐ���
		m_menuIndex %= m_pUI.size();
	}
	if (kbTracker->pressed.W)
	{
		//  ���L�[����������A�I����1�߂�
		m_menuIndex += static_cast<unsigned int>(m_pUI.size()) - 1;
		//  ���j���[�A�C�e�����̍ő�l�𒴂��Ȃ��悤�ɐ���
		m_menuIndex %= m_pUI.size();
	}
	if (kbTracker->pressed.Space)
	{
		m_num = static_cast<StateID>(m_menuIndex);
	}
	//  ���j���[�A�C�e���̑I�����X�V
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  �A�C�e���̑I����Ԃ��X�V
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// �I�𒆂̏����T�C�Y���擾����
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	//  �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
	SimpleMath::Vector2 selectScale = SimpleMath::Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), SimpleMath::Vector2::One, 1);
	//  �I����Ԃ͏�����ԁ{50���̑傫���Ƃ���
	select = SimpleMath::Vector2((sin(m_time) * 0.1f) + 1.0f);
	//  �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
	m_pUI[m_menuIndex]->SetScale(select);
	//  �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);
}

void SettingMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		//  �A�C�e���p�E�B���h�E�w�i��\��
		if (i == m_menuIndex) m_pSelect[i]->Render();
		//  ���ۂɕ\���������A�C�e���摜��\��
		m_pUI[i]->Render();
	}
}

void SettingMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, kumachi::ANCHOR anchor)
{
	//  ���j���[�Ƃ��ăA�C�e����ǉ�����
	std::unique_ptr<SettingUI> userInterface = std::make_unique<SettingUI>();
	//  �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	userInterface->Create(m_pDR
						  , path
						  , position
						  , scale
						  , anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  �A�C�e����V�����ǉ�
	m_pUI.push_back(std::move(userInterface));

	//  �w�i�p�̃E�B���h�E�摜���ǉ�����
	std::unique_ptr<SettingUI> base = std::make_unique<SettingUI>();
	base->Create(m_pDR
				 , m_pSelectTexturePath
				 , position
				 , scale
				 , anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

	//  �w�i�p�̃A�C�e�����V�����ǉ�����
	m_pSelect.push_back(std::move(base));

}
