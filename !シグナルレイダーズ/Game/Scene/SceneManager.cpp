/*
	@file	SceneManager.cpp
	@brief	シーンマネージャクラス
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
// コンストラクタ
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
// デストラクタ
//---------------------------------------------------------
SceneManager::~SceneManager()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void SceneManager::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	ChangeScene(IScene::SceneID::TITLE);
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void SceneManager::Update(float elapsedTime)
{
	m_currentScene->Update(elapsedTime);

	// 説明用変数：次のシーン
	const IScene::SceneID nextSceneID = m_currentScene->GetNextSceneID();

	// シーンを変更しないとき
	if (nextSceneID == IScene::SceneID::NONE) return;

	// シーンを変更するとき
	ChangeScene(nextSceneID);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void SceneManager::Render()
{
	m_currentScene->Render();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void SceneManager::Finalize()
{
	DeleteScene();
}

//---------------------------------------------------------
// シーンを変更する
//---------------------------------------------------------
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	DeleteScene();
	CreateScene(sceneID);
}

//---------------------------------------------------------
// シーンを作成する
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
		assert(!"SceneManager::CreateScene::シーン名が存在しません！");
		// no break
	}

	assert(m_currentScene && "SceneManager::CreateScene::次のシーンが生成されませんでした！");
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
// シーンを削除する
//---------------------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		//現在のシーンが「セレクトシーン」の場合、ステージ番号を取得する
		if (GetSceneID() == IScene::SceneID::STAGESELECT)
		{
			//ステージ番号を取得する
			auto stageSelectScene = dynamic_cast<StageSelectScene*>(m_currentScene.get());
			assert(stageSelectScene);
			//ステージ番号を取得する
			m_stageNumber = stageSelectScene->GetStageNumber();

		}
		m_currentScene.reset();
		ShowCursor(TRUE);//カーソルを見えるようにする
	}
}
