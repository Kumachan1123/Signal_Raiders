/*
	@file	StageSelectScene.h
	@brief	�X�e�[�W�Z���N�g�V�[���N���X
*/
#pragma once
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/StageSelectScene/Menu/StageSelectMenu.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/StageSelectScene/Stage/StageSelect.h"
#include "Game/MousePointer/MousePointer.h"
// �O���錾
class CommonResources;
class Fade;
class BackGround;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class StageSelectScene final :
	public IScene
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// ���U���g���j���[
	std::unique_ptr<StageSelectMenu> m_pStageSelectMenu;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// ���ʃN���X
	std::unique_ptr<StageSelect> m_pStageSelect;
	// ���ʃN���X�ɓn���e�N�X�`���p�X
	const wchar_t* m_pTexturePath;
	// �}�E�X�|�C���^�[
	std::unique_ptr<MousePointer> m_pMousePointer;

	// ��̍s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O
	float m_volume;		// �{�����[��
	int m_counter;		// �t�F�[�h�J�E���^
	// ���ʂ̊
	const float VOLUME = 0.5f;
	float m_BGMvolume;	// �{�����[��
	float m_SEvolume;	// �{�����[��
	float m_time = 0.0f;// �g�k�Ɏg������
	float m_size = 0.0f;// �摜�T�C�Y
	float m_pressKeySize = 0.0f;
	// ��ʑJ�ڃt�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// �w�i
	std::unique_ptr<BackGround> m_pBackGround;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
public:
	StageSelectScene(IScene::SceneID sceneID);
	~StageSelectScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	int GetStageNumber() const { return m_pStageSelectMenu->GetMenuIndex(); }
};
