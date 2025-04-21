/*
	@file	TitleScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
#include "Game/Scene/IScene.h"
#include <DeviceResources.h>
#include "Game/Fade/Fade.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/TitleLogo/TitleLogo.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/TitleScene/TitleMenu/TitleMenu.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/MousePointer/MousePointer.h"
// �O���錾
class CommonResources;
class Fade;

class FPS_Camera;

class TitleScene final :
	public IScene
{

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//	�ϐ�
	DX::DeviceResources* m_pDR;
	// �t�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// �t�F�[�h�̏��
	Fade::FadeState m_fadeState;
	// �w�i
	std::unique_ptr<BackGround> m_pBackGround;
	// �^�C�g�����S
	std::unique_ptr<TitleLogo> m_pTitleLogo;
	// �}�E�X�|�C���^�[
	std::unique_ptr<MousePointer> m_pMousePointer;
	// ���j���[
	std::unique_ptr<TitleMenu> m_pMenu;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// �t�F�[�h�摜�ԍ�
	int m_fadeTexNum;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;

	// FPS�J����
	std::unique_ptr<FPS_Camera> m_camera;
	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O
	// ���ʂ̊
	const float VOLUME = 0.5f;
	float m_BGMvolume;	// �{�����[��
	float m_SEvolume;	// �{�����[��
	int m_counter;		// �t�F�[�h�J�E���^
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;

public:
	TitleScene(IScene::SceneID sceneID);
	~TitleScene() override;
	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;
	SceneID GetNextSceneID() const;
};
