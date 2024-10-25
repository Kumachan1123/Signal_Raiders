/*
	@file	SceneManager.cpp
	@brief	�V�[���}�l�[�W���N���X
*/
#include "pch.h"
#include "SceneManager.h"
#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game/Scene/SettingScene/SettingScene.h"
#include "Game/Scene/PlayScene/PlayScene.h"
#include "Game/Scene/ResultScene/ResultScene.h"
#include "Game/Scene/StageSelectScene/StageSelectScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
SceneManager::SceneManager()
	:
	m_currentScene{},
	m_commonResources{},
	m_stageNumber{ 0 },
	m_nowSceneID{ IScene::SceneID::NONE }
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
SceneManager::~SceneManager()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void SceneManager::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	ChangeScene(IScene::SceneID::TITLE);
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void SceneManager::Update(float elapsedTime)
{
	m_currentScene->Update(elapsedTime);

	// �����p�ϐ��F���̃V�[��
	const IScene::SceneID nextSceneID = m_currentScene->GetNextSceneID();

	// �V�[����ύX���Ȃ��Ƃ�
	if (nextSceneID == IScene::SceneID::NONE) return;

	// �V�[����ύX����Ƃ�
	ChangeScene(nextSceneID);
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void SceneManager::Render()
{
	m_currentScene->Render();
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void SceneManager::Finalize()
{
	DeleteScene();
}

//---------------------------------------------------------
// �V�[����ύX����
//---------------------------------------------------------
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	DeleteScene();
	CreateScene(sceneID);
}

//---------------------------------------------------------
// �V�[�����쐬����
//---------------------------------------------------------
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_currentScene == nullptr);

	switch (sceneID)
	{
	case IScene::SceneID::TITLE:
		m_currentScene = std::make_unique<TitleScene>(sceneID);
		break;
	case IScene::SceneID::SETTING:
		m_currentScene = std::make_unique<SettingScene>(sceneID);
		break;
	case IScene::SceneID::STAGESELECT:
		m_currentScene = std::make_unique<StageSelectScene>(sceneID);
		break;
	case IScene::SceneID::PLAY:
		m_currentScene = std::make_unique<PlayScene>(sceneID);

		break;
	case IScene::SceneID::CLEAR:
	case IScene::SceneID::GAMEOVER:
		m_currentScene = std::make_unique<ResultScene>(sceneID);
		break;
	default:
		assert(!"SceneManager::CreateScene::�V�[���������݂��܂���I");
		// no break
	}

	assert(m_currentScene && "SceneManager::CreateScene::���̃V�[������������܂���ł����I");
	if (GetSceneID() == IScene::SceneID::STAGESELECT && m_stageNumber < 5)
	{
		auto playScene = dynamic_cast<PlayScene*>(m_currentScene.get());
		assert(playScene);
		playScene->SetStageNumber(m_stageNumber);
	}
	m_currentScene->Initialize(m_commonResources);
	SetSceneID(sceneID);
}

//---------------------------------------------------------
// �V�[�����폜����
//---------------------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		//���݂̃V�[�����u�Z���N�g�V�[���v�̏ꍇ�A�X�e�[�W�ԍ����擾����
		if (GetSceneID() == IScene::SceneID::STAGESELECT)
		{
			//�X�e�[�W�ԍ����擾����
			auto stageSelectScene = dynamic_cast<StageSelectScene*>(m_currentScene.get());
			assert(stageSelectScene);
			//�X�e�[�W�ԍ����擾����
			m_stageNumber = stageSelectScene->GetStageNumber();

		}
		m_currentScene.reset();
		ShowCursor(TRUE);//�J�[�\����������悤�ɂ���
	}
}
