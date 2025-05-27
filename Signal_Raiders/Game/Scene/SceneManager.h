/*
	@file	SceneManager.h
	@brief	�V�[���}�l�[�W���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "IScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game/Scene/SettingScene/SettingScene.h"
#include "Game/Scene/PlayScene/PlayScene.h"
#include "Game/Scene/ResultScene/ResultScene.h"
#include "Game/Scene/StageSelectScene/StageSelectScene.h"
// �O���錾
class CommonResources;
class SceneManager
{
public:// public�����o�֐�
	SceneManager();// �R���X�g���N�^
	~SceneManager();// �f�X�g���N�^
	void Initialize(CommonResources* resources);// ������
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
	void Finalize();// �I��
private:// private�����o�֐�
	void ChangeScene(IScene::SceneID sceneID);// �V�[���ύX
	void CreateScene(IScene::SceneID sceneID);// �V�[���쐬
	void DeleteScene();// �V�[���폜
	IScene::SceneID GetSceneID() const { return m_nowSceneID; }// �V�[��ID�擾
	void SetSceneID(IScene::SceneID sceneID) { m_nowSceneID = sceneID; }// �V�[��ID�ݒ�
private:// private�萔
	static const int STAGE_MAX;// �X�e�[�W��
private:// private�����o�ϐ�
	std::unique_ptr<IScene> m_currentScene;// ���݂̃V�[��
	CommonResources* m_pCommonResources;// ���ʃ��\�[�X
	IScene::SceneID m_nowSceneID;// ���݂̃V�[��ID
	int m_stageNumber;// �v���C�V�[���ɃX�e�[�W�ԍ���n�����߂̕ϐ�

};
