/*
*	@file	SettingScene.h
*	@brief	���j���[�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <vector>
#include <memory>
// DirectX
#include <Mouse.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include "Libraries/Microsoft/DebugDraw.h"
// ����w�b�_�[�t�@�C��
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
// �O���錾
class CommonResources;
class Fade;
class SettingScene final : public IScene
{
public:// public�֐�
	// �R���X�g���N�^
	SettingScene(IScene::SceneID sceneID);
	// �f�X�g���N�^
	~SettingScene() override;
	// ������
	void Initialize(CommonResources* resources) override;
	// �X�V
	void Update(float elapsedTime)override;
	// �`��
	void Render() override;
	// �I������
	void Finalize() override;
	// ���̃V�[��ID���擾
	SceneID GetNextSceneID() const;
private:// private�֐�
	// ���ʐݒ�
	void SetVolume();
	// �ݒ�o�[�̍X�V
	void UpdateSettingBars(const UpdateContext& ctx);
	// �t�F�[�h�ƃ}�E�X�̍X�V
	void UpdateFadeAndMouse(const UpdateContext& ctx);
	// �w�i�ƃt�F�[�h�̍X�V
	void UpdateBackgroundAndFade(float elapsedTime);
	// �}�E�X�|�C���^�[�̍X�V
	void UpdateMousePointers(const UpdateContext& ctx);
	// ���j���[�I���̏���
	void HandleMenuSelection(const UpdateContext& ctx);
private:// private�萔
	// ���ʂ̊
	static const float VOLUME;
private:// private�ϐ�
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
	// ���j���[
	std::unique_ptr<SettingMenu> m_pSettingMenu;
	// �ݒ�o�[
	std::unique_ptr<SettingBar> m_pSettingBar;
	// �}�E�X�|�C���^�[
	std::unique_ptr<MousePointer> m_pMousePointer;
	// �ݒ��ʂ�UI
	std::vector < std::unique_ptr<IMenuUI> >m_pUI;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// BGM�{�����[��
	float m_BGMvolume;
	// SE�{�����[��
	float m_SEvolume;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �t�F�[�h�t���O
	bool m_isFade;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
};
