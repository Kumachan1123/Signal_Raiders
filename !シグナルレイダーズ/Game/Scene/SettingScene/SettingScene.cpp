/*
	@file	SettingScene.cpp
	@brief	���j���[�N���X
*/
#include "pch.h"
#include "SettingScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Game/KumachiLib//BinaryFile.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/FPS_Camera/FPS_Camera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
SettingScene::SettingScene()
	:
	m_commonResources{},
	m_isChangeScene{ false },
	m_isFade{ false },
	m_volume{ 1 },
	m_counter{ 0 },
	m_camera{},
	m_pDR{},
	m_pFade{},
	m_fadeState{ },
	m_fadeTexNum{ 0 },
	m_pBackGround{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
{}


//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
SettingScene::~SettingScene()
{
	// do nothing.
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void SettingScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	auto DR = m_commonResources->GetDeviceResources();
	// �t�F�[�h�̏�����
	m_pFade = std::make_unique<Fade>(m_commonResources);
	m_pFade->Create(DR);
	m_pFade->SetState(Fade::FadeState::FadeIn);
	m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// �w�i�̏�����
	m_pBackGround = std::make_unique<BackGround>(m_commonResources);
	m_pBackGround->Create(DR);
	// FPS�J�������쐬����
	m_camera = std::make_unique<FPS_Camera>();
	// �w���摜���쐬
	m_pPressKey = std::make_unique<PressKey>(m_commonResources);
	m_pPressKey->Initialize();
	// �Z�b�e�B���O���j���[���쐬
	m_pSettingMenu = std::make_unique<SettingMenu>();
	m_pSettingMenu->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// �ݒ�o�[���쐬
	m_pSettingBar = std::make_unique<SettingBar>(m_pSettingMenu.get());
	m_pSettingBar->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// ����������������
	InitializeFMOD();

}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void SettingScene::Update(float elapsedTime)
{
	// �Z�b�e�B���O���j���[�̍X�V����
	m_pSettingMenu->Update(elapsedTime);
	// ���I�΂�Ă��郁�j���[��ID��Bar�ɓn��
	m_pSettingBar->SetStateIDNum(static_cast<SettingMenu::StateID>(m_pSettingMenu->GetMenuIndex()));
	// �Z�b�e�B���O�o�[�̍X�V����
	m_pSettingBar->Update(elapsedTime);
	// �I�[�f�B�I�}�l�[�W���[�̍X�V����
	m_audioManager->Update();
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// ���j���[�ł̑I���������s��ꂽ��
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd && kbTracker->pressed.Space)
	{
		m_audioManager->PlaySound("SE", .3);// SE�̍Đ�
		if (m_pSettingMenu->GetStateIDNum() == SettingMenu::StateID::END ||
			m_pSettingMenu->GetStateIDNum() == SettingMenu::StateID::APPLY)
		{
			m_pFade->SetState(Fade::FadeState::FadeOut);// �t�F�[�h�A�E�g�Ɉڍs
			m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// �t�F�[�h�̃e�N�X�`����ύX
		}


	}
	// �t�F�[�h�A�E�g���I��������
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
	// BGM�̍Đ�
	m_audioManager->PlaySound("BGM", 0.3);
	// �w���摜�̍X�V
	m_pPressKey->Update(elapsedTime);
	// �w�i�̍X�V
	m_pBackGround->Update(elapsedTime);
	// �t�F�[�h�̍X�V
	m_pFade->Update(elapsedTime);
	// �w���摜�̍X�V
	m_pPressKey->Update(elapsedTime);

}
//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void SettingScene::Render()
{
	// �w�i�̕`��
	m_pBackGround->Render();

	// �X�y�[�X�L�[�����Ă��Ă�`��(��ʑJ�ڒ��͕`�悵�Ȃ�)
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		m_pSettingMenu->Render();
		m_pSettingBar->Render();
		//m_pPressKey->Render();
	}
	// �t�F�[�h�̕`��
	m_pFade->Render();
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void SettingScene::Finalize()
{
	// �I�[�f�B�I�}�l�[�W���[�̏I������
	m_audioManager->Shutdown();
}

//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID SettingScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		m_audioManager->StopSound("BGM");// BGM�̒�~
		m_audioManager->StopSound("SE");// SE�̒�~
		return IScene::SceneID::TITLE;
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}

//---------------------------------------------------------
// FMOD�̃V�X�e���̏������Ɖ����f�[�^�̃��[�h
//---------------------------------------------------------
void SettingScene::InitializeFMOD()
{
	// FMOD�V�X�e���̏�����
	m_audioManager->Initialize();
	// �����f�[�^�̃��[�h
	m_audioManager->LoadSound("Resources/Sounds/select.mp3", "SE");
	m_audioManager->LoadSound("Resources/Sounds/title.mp3", "BGM");
}
