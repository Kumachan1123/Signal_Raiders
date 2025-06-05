/*
	@file	TitleScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED
// �W�����C�u����
#include <cassert>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include "Libraries/Microsoft/DebugDraw.h"
#include <Libraries/MyLib/DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/Scene/IScene.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/TitleLogo/TitleLogo.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/TitleScene/TitleMenu/TitleMenu.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"

// �O���錾
class CommonResources;
class Fade;

// �^�C�g���V�[���N���X
class TitleScene final :public IScene
{
public:
	// public�֐�
	// �R���X�g���N�^
	TitleScene(IScene::SceneID sceneID);
	// �f�X�g���N�^
	~TitleScene() override;
	// ������
	void Initialize(CommonResources* resources) override;
	// �X�V
	void Update(float elapsedTime)override;
	// �`��
	void Render() override;
	// �I��
	void Finalize() override;
	// ���̃V�[��ID���擾
	SceneID GetNextSceneID() const override;
private:
	// �萔
	// ���ʂ̊
	static	const float VOLUME;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
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
	// �^�C�g�����j���[
	std::unique_ptr<TitleMenu> m_pTitleMenu;
	// �^�C�g����ʂ�UI(���j���[�A�}�E�X�J�[�\���j
	std::vector<std::unique_ptr<IMenuUI>> m_pUI;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// BGM����
	float m_BGMvolume;
	// SE����
	float m_SEvolume;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
};
#endif // TITLE_SCENE_DEFINED
