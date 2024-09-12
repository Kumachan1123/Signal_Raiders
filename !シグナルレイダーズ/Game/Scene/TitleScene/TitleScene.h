/*
	@file	TitleScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
#include "Game/Scene/IScene.h"
#include <DeviceResources.h>
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/TitleLogo/TitleLogo.h"
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Scene/PressKey/PressKey.h"
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
	std::unique_ptr<Fade> m_fade;
	// �t�F�[�h�̏��
	Fade::FadeState m_fadeState;
	// �w�i
	std::unique_ptr<BackGround> m_backGround;
	// �^�C�g�����S
	std::unique_ptr<TitleLogo> m_titleLogo;
	// �w��
	std::unique_ptr<PressKey> m_pressKey;
	// �t�F�[�h�摜�ԍ�
	int m_fadeTexNum;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;

	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;

	// FPS�J����
	std::unique_ptr<FPS_Camera> m_camera;
	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O
	float m_volume;		// �{�����[��
	int m_counter;		// �t�F�[�h�J�E���^


public:
	TitleScene();
	~TitleScene() override;


	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
private:


	void DrawSpace();// �X�y�[�X�L�[�����Ă��Ă�`��

};
