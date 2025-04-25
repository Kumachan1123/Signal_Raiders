/*
	@file	SettingBar.cpp
	@brief	�ݒ�o�[�N���X
*/
#include "pch.h"
#include "SettingBar.h"
#include "Game/Screen.h"
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
using namespace DirectX;
using namespace DirectX::SimpleMath;

SettingBar::SettingBar()
	: m_commonResources{}
	, m_pDR{}
	, m_pSettingMenu{   }
	, m_pBar{}
	, m_pSettingBarTexturePath{}
	, m_pSettingBarPointerTexturePath{}
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_tracker{}
	, m_num{ SettingMenu::StateID::BGM }
	, m_selectNum{ SettingMenu::SelectID::NONE }
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
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  �uSE�v�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y - 150)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  �u�}�E�X����ǁv�̏ꏊ�ɔz�u
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);

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
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �}�E�X�{�^����Ԃ��擾
	auto& mouseTracker = m_commonResources->GetInputManager()->GetMouseTracker();
	bool isDragging = false;// �h���b�O��Ԃ�ێ�
	int dragIndex = -1;// �h���b�O���̃A�C�e���̃C���f�b�N�X
	m_time += elapsedTime;
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// �|�C���^�[�̓����蔻��`�F�b�N�i�~�`�j

		if (m_pBarPointer[i]->IsHit(mousePos))
		{
			if (mouseTracker->GetLastState().leftButton) isDragging = true;
			dragIndex = i;
			break;
		}

	}
	// �h���b�O���Ȃ�}�E�X���W�ɒǏ]
	if (isDragging && dragIndex != -1)
	{
		float newX = mousePos.x;

		// �͈͐����i���[�`�E�[�j
		float minX = Screen::CENTER_X + 300 - 300;
		float maxX = Screen::CENTER_X + 300 + 300;
		newX = std::max(minX, std::min(maxX, newX));

		// �|�C���^�[�̈ʒu�X�V
		m_pBarPointer[dragIndex]->SetPosition(Vector2(newX, m_pBarPointer[dragIndex]->GetPosition().y));

		// �ݒ�l�̍X�V�i0�`5�ɕϊ��j
		m_setting[dragIndex] = static_cast<int>((newX - minX) / 60);
	}

	//  �u�ւ񂱂��v���I�΂�Ă���ꍇ,�ݒ��ۑ�����
	if (m_pSettingMenu->GetSelectIDNum() == SettingMenu::APPLY)
	{
		if (mouseTracker->GetLastState().leftButton)m_pSettingData->Save(m_setting[0], m_setting[1], m_setting[2]);
	}
#ifdef _DEBUG
	// �f�o�b�O����\������
	auto debugString = m_commonResources->GetDebugString();
	auto& mousestate = m_commonResources->GetInputManager()->GetMouseState();
	// �E�B���h�E��̃}�E�X���W���擾����
	Vector2 pos = Vector2(static_cast<float>(mousestate.x), static_cast<float>(mousestate.y));

	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
		debugString->AddString("SelectNum:%i Setting:%i", m_pBarPointer[i]->IsHit(mousePos), m_setting[i]);

	debugString->AddString("DragIndex:%i ", dragIndex);
	debugString->AddString("MousePos:%f %f", pos.x, pos.y);
	debugString->AddString("ClickLeft:%i", mouseTracker->GetLastState().leftButton);

#endif

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
	, KumachiLib::ANCHOR anchor
	, UIType type)
{
	UNREFERENCED_PARAMETER(type);// ���g�p�x����\��
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