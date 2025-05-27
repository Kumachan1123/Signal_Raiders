/*
	@file	TitleScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/DebugString.h"
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
class TitleScene final :public IScene
{
public:// public�֐�
	TitleScene(IScene::SceneID sceneID);// �R���X�g���N�^
	~TitleScene() override;// �f�X�g���N�^
	void Initialize(CommonResources* resources) override;// ������
	void Update(float elapsedTime)override;// �X�V
	void Render() override;// �`��
	void Finalize() override;// �I��
	SceneID GetNextSceneID() const override;// ���̃V�[��ID���擾
private:
	// ���ʂ̊
	static	const float VOLUME;
private:
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
