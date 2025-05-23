/*
	@file	ResultScene.h
	@brief	���U���g�V�[���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <unordered_map>
// DirectX
#include "DeviceResources.h"
#include <Mouse.h>
// �O�����C�u����
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/MyLib/InputManager.h"
// ����w�b�_�[�t�@�C��
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/ResultScene/ResultMenu/ResultMenu.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/ResultScene/Result/Result.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Interface/IMenuUI.h"
// �O���錾
class CommonResources;
class Fade;
class BackGround;

class ResultScene final : public IScene
{
public:// �A�N�Z�T
	int GetStageNumber() const { return m_stageNumber; }// �X�e�[�W�ԍ����擾
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// �X�e�[�W�ԍ���ݒ�
	SceneID GetNextSceneID() const;// ���̃V�[��ID���擾
public:// public�֐�
	ResultScene(IScene::SceneID sceneID);// �R���X�g���N�^
	~ResultScene() override;// �f�X�g���N�^
	void Initialize(CommonResources* resources) override;// ������
	void Update(float elapsedTime)override;// �X�V
	void Render() override;// �`��
	void Finalize() override;// �I��
public:
	// ���ʂ̊
	static const float VOLUME;
	// �X�e�[�W�I���Ɉړ�����l
	static const int STAGE_SELECT;
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// �}�E�X�|�C���^�[
	std::unique_ptr<MousePointer> m_pMousePointer;
	// ���U���g���j���[
	std::unique_ptr<ResultMenu> m_pResultMenu;
	// ���U���g�V�[����UI(���j���[�A�}�E�X�J�[�\���j
	std::vector<std::unique_ptr<IMenuUI>> m_pUI;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// ���ʃN���X
	std::unique_ptr<Result> m_pResult;
	// ���ʂɉ����ĕς��e�N�X�`���p�X�}�b�v
	std::unordered_map<IScene::SceneID, const wchar_t*> m_pResultTexturePathMap;
	// �X�e�[�W�ԍ�
	int m_stageNumber;
	// BGM�{�����[��
	float m_BGMvolume;
	// SE�{�����[��
	float m_SEvolume;
	// ��ʑJ�ڃt�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// �w�i
	std::unique_ptr<BackGround> m_pBackGround;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
};
