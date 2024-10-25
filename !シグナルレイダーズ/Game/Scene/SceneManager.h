/*
	@file	SceneManager.h
	@brief	�V�[���}�l�[�W���N���X
*/
#pragma once
#include "IScene.h"


// �O���錾
class CommonResources;

class SceneManager
{
private:
	std::unique_ptr<IScene> m_currentScene;
	CommonResources* m_commonResources;
	IScene::SceneID m_nowSceneID;
public:
	// �v���C�V�[���ɃX�e�[�W�ԍ���n�����߂̕ϐ�
	int m_stageNumber;
public:
	SceneManager();
	~SceneManager();

	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	void ChangeScene(IScene::SceneID sceneID);
	void CreateScene(IScene::SceneID sceneID);
	void DeleteScene();
	IScene::SceneID GetSceneID() const { return m_nowSceneID; }
	void SetSceneID(IScene::SceneID sceneID) { m_nowSceneID = sceneID; }
};
