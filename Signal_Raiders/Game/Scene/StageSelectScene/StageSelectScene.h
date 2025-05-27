/*
*	@file	StageSelectScene.h
*	@brief	�X�e�[�W�Z���N�g�V�[���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX
#include <DeviceResources.h>
#include <Mouse.h>
 // �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/StageSelectScene/Menu/StageSelectMenu.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/StageSelectScene/Stage/StageSelect.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Interface/IMenuUI.h"
// �O���錾
class CommonResources;
class Fade;
class BackGround;
class StageSelectScene final : public IScene
{
public:// �A�N�Z�T
	SceneID GetNextSceneID()const override;// ���̃V�[��ID���擾
	int GetStageNumber() const { return m_stageNumber; }// �X�e�[�W�ԍ����擾
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; } // �X�e�[�W�ԍ���ݒ�
public:
	StageSelectScene(IScene::SceneID sceneID);// �R���X�g���N�^
	~StageSelectScene() override;// �f�X�g���N�^
	void Initialize(CommonResources* resources) override;// ������
	void Update(float elapsedTime)override;// �X�V
	void Render() override;// �`��
	void Finalize() override;// �I��
private:
	// ���ʂ̊
	static const float VOLUME;
	// �^�C�g���ɖ߂�l
	static const int TITLE;
private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// �X�e�[�W�I�����j���[
	std::unique_ptr<StageSelectMenu> m_pStageSelectMenu;
	// �}�E�X�|�C���^�[
	std::unique_ptr<MousePointer> m_pMousePointer;
	// �X�e�[�W�I����ʂ�UI(���j���[�A�}�E�X�J�[�\���j
	std::vector<std::unique_ptr<IMenuUI>> m_pUI;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// ���ʃN���X
	std::unique_ptr<StageSelect> m_pStageSelect;
	// ���ʃN���X�ɓn���e�N�X�`���p�X
	const wchar_t* m_pTexturePath;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// BGM����
	float m_BGMvolume;
	// SE����
	float m_SEvolume;
	// ��ʑJ�ڃt�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// �w�i
	std::unique_ptr<BackGround> m_pBackGround;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
	// �X�e�[�W�ԍ�
	int m_stageNumber;

};
