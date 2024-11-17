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
	, m_setting{ 4,4,5 }
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
	m_pSettingData = std::make_unique<SettingData>();

	//  �uBGM�v�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y - 300)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �uSE�v�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y - 150)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  �u�}�E�X����ǁv�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);

	// Json�t�@�C������ݒ��ǂݍ���
	m_pSettingData->Load();
	m_setting[0] = m_pSettingData->GetBGMVolume();
	m_setting[1] = m_pSettingData->GetSEVolume();
	m_setting[2] = m_pSettingData->GetMouseSensitivity();

	//  �ݒ�o�[�̋ʂ�ǂݍ��񂾐ݒ�ɍ��킹�Ĉړ�
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// �ݒ�o�[�̋ʂ�ǂݍ��񂾐ݒ�ɍ��킹�Ĉړ�
		m_pBarPointer[i]->SetPosition(Vector2(static_cast<float>(Screen::CENTER_X + 60 + m_setting[i] * 60 - 60), m_pBarPointer[i]->GetPosition().y));
	}
}

void SettingBar::Update(float elapsedTime)
{
	//  �L�[�{�[�h�̓��͂��擾
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	m_time += elapsedTime;
	if (0 <= m_num && m_num <= 2)//  �Z�b�e�B���O�V�[���Łu�ւ񂱂��v�Ɓu�����v�ȊO���I�΂�Ă���ꍇ
	{


		//  �L�[�{�[�h�̓��͂��擾
		if (kbTracker->pressed.A)
		{
			// �|�C���^�[�̍��W�����Ɉړ�
			// ���Ɉړ��ł���̂͒��S�ɂ���Ƃ�����T��܂�
			if (m_pBarPointer[m_num]->GetPosition().x > Screen::CENTER_X + 300 - 300)
			{
				m_pBarPointer[m_num]->SetPosition(Vector2(m_pBarPointer[m_num]->GetPosition().x - 60, m_pBarPointer[m_num]->GetPosition().y));
				m_setting[m_num] -= 1;

			}

		}
		if (kbTracker->pressed.D)
		{
			// �|�C���^�[�̍��W���E�Ɉړ�
			// �E�Ɉړ��ł���̂͒��S�ɂ���Ƃ�����T��܂�
			if (m_pBarPointer[m_num]->GetPosition().x < Screen::CENTER_X + 300 + 300)
			{
				m_pBarPointer[m_num]->SetPosition(Vector2(m_pBarPointer[m_num]->GetPosition().x + 60, m_pBarPointer[m_num]->GetPosition().y));
				m_setting[m_num] += 1;


			}

		}

	}
	else if (m_num == 3)
	{
		//  �u�ւ񂱂��v���I�΂�Ă���ꍇ,�ݒ��ۑ�����
		if (kbTracker->pressed.Space)m_pSettingData->Save(m_setting[0], m_setting[1], m_setting[2]);
	}

}

void SettingBar::Render()
{

	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
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
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();
	//  �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	userInterface->Create(m_pDR
						  , path
						  , position
						  , Vector2(scale.x / 1.45f, scale.y / 2.0f)
						  , anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  �A�C�e����V�����ǉ�
	m_pBar.push_back(std::move(userInterface));


	//  �w�i�p�̃E�B���h�E�摜���ǉ�����
	std::unique_ptr<UI> base = std::make_unique<UI>();
	base->Create(m_pDR
				 , m_pSettingBarPointerTexturePath
				 , position
				 , scale
				 , anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

	//  �w�i�p�̃A�C�e�����V�����ǉ�����
	m_pBarPointer.push_back(std::move(base));

}