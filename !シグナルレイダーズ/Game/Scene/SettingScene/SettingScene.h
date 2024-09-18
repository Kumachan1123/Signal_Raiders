/*
	@file	SettingScene.h
	@brief	���j���[�N���X
*/
#pragma once
#include "Game/Scene/IScene.h"
#include <DeviceResources.h>
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Scene/PressKey/PressKey.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
// �O���錾
class CommonResources;
class Fade;

class FPS_Camera;

class SettingScene final :
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
	// �w��
	std::unique_ptr<PressKey> m_pPressKey;
	// ���j���[
	std::unique_ptr<SettingMenu> m_pSettingMenu;
	// �ݒ�o�[
	std::unique_ptr<SettingBar> m_pSettingBar;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// �t�F�[�h�摜�ԍ�
	int m_fadeTexNum;
	// ���ʂ̊
	const float VOLUME = 0.5f;
	float m_BGMvolume;	// �{�����[��
	float m_SEvolume;	// �{�����[��
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;

	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;

	// FPS�J����
	std::unique_ptr<FPS_Camera> m_camera;
	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O


public:
	SettingScene();
	~SettingScene() override;


	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();



private:

	void SetVolume();

};
