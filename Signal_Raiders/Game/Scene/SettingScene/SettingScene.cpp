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
	m_pFade->Create();
	m_pFade->SetState(Fade::FadeState::FadeIn);
	//m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// �w�i�̏�����
	m_pBackGround = std::make_unique<BackGround>(m_commonResources);
	m_pBackGround->Create(DR);
	// FPS�J�������쐬����
	m_camera = std::make_unique<FPS_Camera>();
	// �Z�b�e�B���O���j���[���쐬
	m_pSettingMenu = std::make_unique<SettingMenu>();
	//// �}�E�X�|�C���^�[���쐬
	m_pMousePointer = std::make_unique<MousePointer>();
	m_pSettingBar = std::make_unique<SettingBar>();
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());
	m_pUI.push_back(std::move(m_pSettingMenu));
	m_pUI.push_back(std::move(m_pSettingBar));
	m_pUI.push_back(std::move(m_pMousePointer));
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);

	// �ݒ�f�[�^�̓ǂݍ���
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * .1f;
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * .1f;
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void SettingScene::Update(float elapsedTime)
{
	UpdateContext ctx;
	ctx.bulletPoint = 0;// �g��Ȃ�
	ctx.dashStamina = 0;//�g��Ȃ�
	ctx.elapsedTime = elapsedTime;//�t���[������
	ctx.playerHP = 0;//�g��Ȃ�
	UpdateSettingBars(ctx);	// ���j���[�̍X�V
	UpdateFadeAndMouse(ctx);// �t�F�[�h��Ԃƃ}�E�X�̍X�V
	UpdateBackgroundAndFade(elapsedTime);// �w�i�ƃt�F�[�h�̍X�V
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
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();
	// �t�F�[�h�̕`��
	m_pFade->Render();
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void SettingScene::Finalize()
{
	SetVolume();// ���ʂ̐ݒ�
}

//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID SettingScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		m_commonResources->GetAudioManager()->StopSound("TitleBGM");// BGM�̒�~
		m_commonResources->GetAudioManager()->StopSound("SE");// SE�̒�~
		m_commonResources->GetAudioManager()->StopSound("Select");// Select�̒�~
		return IScene::SceneID::TITLE;
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}



// ���ʂ�ݒ肷��
void SettingScene::SetVolume()
{
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(m_pUI[it].get()))
		{
			// ���ʂ̎擾(10����1�Ŋ������擾)
			float BGMvolume = static_cast<float>(pSettingBar->GetSetting(0)) * 0.1f;
			float SEvolume = static_cast<float>(pSettingBar->GetSetting(1)) * 0.1f;
			// �ݒ�̕ύX
			m_pSettingData->SetBGMVolume(pSettingBar->GetSetting(0));
			m_pSettingData->SetSEVolume(pSettingBar->GetSetting(1));
			// ���ʂ̐ݒ�
			m_BGMvolume = VOLUME * BGMvolume;
			m_SEvolume = VOLUME * SEvolume;
			return;
		}
	}

}

void SettingScene::UpdateSettingBars(const UpdateContext& ctx)
{
	// SettingMenu�����X�g�A�b�v
	std::vector<SettingMenu*> settingMenus;
	for (auto& settingMenu : m_pUI)
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(settingMenu.get()))
			settingMenus.push_back(pSettingMenu);
	// SettingBar�����X�g�A�b�v���A���j���[�����Z�b�g
	for (auto& settingBar : m_pUI)
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(settingBar.get()))
			for (auto& menu : settingMenus)
				pSettingBar->SetStateIDNum(static_cast<SettingMenu::StateID>(menu->GetMenuIndex()));
		settingBar->Update(ctx);
	}
}

void SettingScene::UpdateFadeAndMouse(const UpdateContext& ctx)
{
	// �I�[�f�B�I�}�l�[�W���[�̍X�V����
	m_commonResources->GetAudioManager()->Update();
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// ���j���[�ł̑I���������s��ꂽ��
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;
	if (mtracker->GetLastState().leftButton) HandleMenuSelection(ctx);
	else UpdateMousePointers(ctx);
}

void SettingScene::UpdateBackgroundAndFade(float elapsedTime)
{
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;// �V�[���ύX�t���O�𗧂Ă�
	m_commonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);
	m_pBackGround->Update(elapsedTime);
	m_pFade->Update(elapsedTime);
}

void SettingScene::UpdateMousePointers(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)
		if (auto pMousePointer = dynamic_cast<MousePointer*>(m_pUI[it].get()))
			pMousePointer->Update(ctx);
}

void SettingScene::HandleMenuSelection(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(m_pUI[it].get()))
		{
			auto selectID = pSettingMenu->GetSelectIDNum();// �I�����ꂽ���j���[��ID���擾
			if (selectID == SettingMenu::SelectID::END ||
				selectID == SettingMenu::SelectID::APPLY)
			{
				m_commonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
				m_pFade->SetState(Fade::FadeState::FadeOut);// �t�F�[�h�A�E�g�Ɉڍs
				//m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// �t�F�[�h�̃e�N�X�`����ύX
				return;
			}
			UpdateMousePointers(ctx);// �}�E�X�|�C���^�[�̍X�V
		}
	}
}
