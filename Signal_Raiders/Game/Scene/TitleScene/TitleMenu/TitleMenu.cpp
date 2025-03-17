/*
	@file	TitleMenu.cpp
	@brief	�^�C�g�����j���[�N���X
*/
#include "pch.h"
#include "TitleMenu.h"
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

TitleMenu::TitleMenu()
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
	, m_time{ 0 }
	, m_num{ SceneID::STAGESELECT }
{
}


TitleMenu::~TitleMenu()
{
}

void TitleMenu::Initialize(CommonResources* resources, int width, int height)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSelectTexturePath = L"Resources/Textures/select.png";
	//  �u�v���C�v��ǂݍ���
	Add(L"Resources/Textures/play.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, SimpleMath::Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u�����Ă��v��ǂݍ���
	Add(L"Resources/Textures/setting.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y + 350)
		, SimpleMath::Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u�����v��ǂݍ���
	Add(L"Resources/Textures/end.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y + 450)
		, SimpleMath::Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u��������v��ǂݍ���
	Add(L"Resources/Textures/Guide.png"
		, SimpleMath::Vector2(Screen::RIGHT, Screen::BOTTOM)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);

}

void TitleMenu::Update(float elapsedTime)
{
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();

	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	//  ���j���[�A�C�e���̐������J��Ԃ�
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		if (m_pUI[i]->IsHit(mousePos))
		{
			//  �͈͓��ɂ���ꍇ�́A�I�𒆂̃A�C�e�����X�V
			m_menuIndex = i;
		}
	}

	m_time += elapsedTime;
	//  �L�[�{�[�h�̓��͂��擾
	//if (kbTracker->pressed.S)
	//{
	//	//  ���L�[����������A�I����1�i�߂�
	//	m_menuIndex += 1;
	//	//  ���j���[�A�C�e�����̍ő�l�𒴂��Ȃ��悤�ɐ���
	//	m_menuIndex %= m_pUI.size();
	//}
	if (kbTracker->pressed.W)
	{
		//  ���L�[����������A�I����1�߂�
		m_menuIndex += static_cast<unsigned int>(m_pUI.size()) - 1;
		//  ���j���[�A�C�e�����̍ő�l�𒴂��Ȃ��悤�ɐ���
		m_menuIndex %= m_pUI.size();
	}
	if (kbTracker->pressed.Space || mtracker->GetLastState().leftButton)
	{
		m_num = static_cast<SceneID>(m_menuIndex);
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
	// �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
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

void TitleMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		//  �A�C�e���p�E�B���h�E�w�i��\��
		if (i == m_menuIndex) m_pSelect[i]->Render();
		//  ���ۂɕ\���������A�C�e���摜��\��
		m_pUI[i]->Render();

	}
	// �I��s�\�ȃA�C�e����\��
	for (unsigned int i = 0; i < m_pGuide.size(); i++)
	{
		m_pGuide[i]->Render();
	}
#ifdef _DEBUG
	// �f�o�b�O����\������
	auto debugString = m_commonResources->GetDebugString();
	for (int i = 0; i < m_transforms.size(); i++)
	{
		debugString->AddString("Transform.Pos:%f,%f  Scale:%f,%f", m_transforms[i].position.x, m_transforms[i].position.y, m_transforms[i].scale.x, m_transforms[i].scale.y);
	}
#endif
}

void TitleMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
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
	if (type == UIType::SELECT)m_pUI.push_back(std::move(userInterface));
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

	//  �w�i�p�̃A�C�e�����V�����ǉ�����
	m_pSelect.push_back(std::move(base));

	// UI�̏���z��ɓo�^
	m_transforms.push_back({ position, scale });

}
