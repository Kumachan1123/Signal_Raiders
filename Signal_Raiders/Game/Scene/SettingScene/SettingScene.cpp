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
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/FPS_Camera/FPS_Camera.h"
#include <Mouse.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
SettingScene::SettingScene(IScene::SceneID sceneID)
	:
	m_commonResources{},
	m_isChangeScene{ false },
	m_isFade{ false },
	m_BGMvolume{ VOLUME },
	m_SEvolume{ VOLUME },
	m_camera{},
	m_pDR{},
	m_pFade{},
	m_fadeState{ },
	m_fadeTexNum{ 0 },
	m_pBackGround{ nullptr },
	m_audioManager{ AudioManager::GetInstance() },
	m_nowSceneID{ sceneID },
	m_pSettingMenu{},
	m_pSettingBar{},
	m_pSettingData{}

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
	// �Z�b�e�B���O���j���[���쐬
	m_pSettingMenu = std::make_unique<SettingMenu>();
	m_pSettingMenu->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// �}�E�X�|�C���^�[���쐬
	m_pMousePointer = std::make_unique<MousePointer>();
	m_pMousePointer->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// �ݒ�o�[���쐬
	m_pSettingBar = std::make_unique<SettingBar>(m_pSettingMenu.get());
	m_pSettingBar->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// �ݒ�f�[�^�̓ǂݍ���
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * .1f;
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * .1f;
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

	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();

	// ���j���[�ł̑I���������s��ꂽ��
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		if (mtracker->GetLastState().leftButton)
		{
			if (m_pSettingMenu->GetSelectIDNum() == SettingMenu::SelectID::END ||
				m_pSettingMenu->GetSelectIDNum() == SettingMenu::SelectID::APPLY)
			{

				m_pFade->SetState(Fade::FadeState::FadeOut);// �t�F�[�h�A�E�g�Ɉڍs
				m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// �t�F�[�h�̃e�N�X�`����ύX
			}
			else
			{
				// �}�E�X�|�C���^�[�̍X�V
				m_pMousePointer->Update(elapsedTime);

			}

		}
		else
		{
			// �}�E�X�|�C���^�[�̍X�V
			m_pMousePointer->Update(elapsedTime);
		}


	}
	// �t�F�[�h�A�E�g���I��������
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
	// BGM�̍Đ�
	m_audioManager->PlaySound("BGM", m_BGMvolume);

	// �w�i�̍X�V
	m_pBackGround->Update(elapsedTime);
	// �t�F�[�h�̍X�V
	m_pFade->Update(elapsedTime);


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
		m_pMousePointer->Render();
	}
	// �t�F�[�h�̕`��
	m_pFade->Render();
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void SettingScene::Finalize()
{
	SetVolume();// ���ʂ̐ݒ�
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
		m_audioManager->StopSound("Select");// Select�̒�~
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
	m_audioManager->LoadSound("Resources/Sounds/click.mp3", "Select");
}

// ���ʂ�ݒ肷��
void SettingScene::SetVolume()
{
	// ���ʂ̎擾(10����1�Ŋ������擾)
	float BGMvolume = static_cast<float>(m_pSettingBar->GetSetting(0)) * 0.1f;
	float SEvolume = static_cast<float>(m_pSettingBar->GetSetting(1)) * 0.1f;
	// �ݒ�̕ύX
	m_pSettingData->SetBGMVolume(m_pSettingBar->GetSetting(0));
	m_pSettingData->SetSEVolume(m_pSettingBar->GetSetting(1));
	// ���ʂ̐ݒ�
	m_BGMvolume = VOLUME * BGMvolume;
	m_SEvolume = VOLUME * SEvolume;
}