/*
	@file	SettingBar.cpp
	@brief	�ݒ�o�[�N���X
*/
#include "pch.h"
#include "SettingBar.h"
#include "Game/Screen.h"
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

SettingBar::SettingBar(SettingMenu* pSettingMenu)
	: m_commonResources{}
	, m_pDR{}
	, m_pSettingMenu{ pSettingMenu }
	, m_pBar{}
	, m_pSettingBarTexturePath{}
	, m_pSettingBarPointerTexturePath{}
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_tracker{}
	, m_num{ SettingMenu::StateID::BGM }
{
}

SettingBar::~SettingBar()
{
}

void SettingBar::Initialize(CommonResources* resources, int width, int height)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSettingBarTexturePath = L"Resources/Textures/SettingBar.png";
	m_pSettingBarPointerTexturePath = L"Resources/Textures/SettingBarPointer.png";
	//  �uBGM�v�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 250, Screen::CENTER_Y - 300)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �uSE�v�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 250, Screen::CENTER_Y - 150)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �u�}�E�X����ǁv�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 250, Screen::CENTER_Y)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);

}

void SettingBar::Update(float elapsedTime)
{
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	m_time += elapsedTime;
	if (m_num < 3)//  �Z�b�e�B���O�V�[���Łu�ւ񂱂��v�Ɓu�����v�ȊO���I�΂�Ă���ꍇ
	{
		//  �L�[�{�[�h�̓��͂��擾
		if (kbTracker->pressed.A)
		{
			// �|�C���^�[�̍��W�����Ɉړ�
			// ���Ɉړ��ł���̂͒��S�ɂ���Ƃ�����T��܂�
			if (m_pBarPointer[m_num]->GetPosition().x > Screen::CENTER_X + 250 - 300)
			{
				m_pBarPointer[m_num]->SetPosition(Vector2(m_pBarPointer[m_num]->GetPosition().x - 60, m_pBarPointer[m_num]->GetPosition().y));
			}
		}
		if (kbTracker->pressed.D)
		{
			// �|�C���^�[�̍��W���E�Ɉړ�
			// �E�Ɉړ��ł���̂͒��S�ɂ���Ƃ�����T��܂�
			if (m_pBarPointer[m_num]->GetPosition().x < Screen::CENTER_X + 250 + 300)
			{
				m_pBarPointer[m_num]->SetPosition(Vector2(m_pBarPointer[m_num]->GetPosition().x + 60, m_pBarPointer[m_num]->GetPosition().y));
			}
		}
	}

}

void SettingBar::Render()
{

	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		//  �A�C�e���p�E�B���h�E�w�i��\��
		//if (i == m_menuIndex) m_pSelect[i]->Render();
		//  ���ۂɕ\���������A�C�e���摜��\��
		m_pBar[i]->Render();
		//  �ݒ�o�[�̋ʂ�\��
		m_pBarPointer[i]->Render();
	}
}

void SettingBar::Add(const wchar_t* path
					 , DirectX::SimpleMath::Vector2 position
					 , DirectX::SimpleMath::Vector2 scale
					 , kumachi::ANCHOR anchor)
{
	//  ���j���[�Ƃ��ăA�C�e����ǉ�����
	std::unique_ptr<SettingUI> userInterface = std::make_unique<SettingUI>();
	//  �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	userInterface->Create(m_pDR
						  , path
						  , position
						  , Vector2(scale.x / 1.5f, scale.y / 2.0f)
						  , anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  �A�C�e����V�����ǉ�
	m_pBar.push_back(std::move(userInterface));


	//  �w�i�p�̃E�B���h�E�摜���ǉ�����
	std::unique_ptr<SettingUI> base = std::make_unique<SettingUI>();
	base->Create(m_pDR
				 , m_pSettingBarPointerTexturePath
				 , position
				 , scale
				 , anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

	//  �w�i�p�̃A�C�e�����V�����ǉ�����
	m_pBarPointer.push_back(std::move(base));

}