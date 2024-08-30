/*
	@file	ClearScene.h
	@brief	�N���A�V�[���N���X
*/
#pragma once
#include "IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"

// �O���錾
class CommonResources;
class Fade;
class BackGround;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}
class ClearScene final :
	public IScene
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// �^�C�g���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTexture;
	// �w��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pressKeyTexture;
	// �e�N�X�`���̔����̑傫��
	DirectX::SimpleMath::Vector2 m_clearTexCenter;
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// FMOD�Ŏg�p����ϐ��i�|�C���^�j
	FMOD::System* m_system;	// FMOD�̃V�X�e��
	FMOD::Sound* m_soundSE;	// SE�p�̉����f�[�^
	FMOD::Sound* m_soundBGM;	// BGM�p�̉����f�[�^
	FMOD::Channel* m_channelSE;	// SE���Đ�����`�����l��
	FMOD::Channel* m_channelBGM;// BGM���Đ�����`�����l��

	// ��̍s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O
	float m_volume;		// �{�����[��
	int m_counter;		// �t�F�[�h�J�E���^
	float m_time = 0.0f;// �g�k�Ɏg������
	float m_size = 0.0f;// �摜�T�C�Y
	float m_pressKeySize = 0.0f;

	// ��ʑJ�ڃt�F�[�h
	std::unique_ptr<Fade> m_fade;
	// �w�i
	std::unique_ptr<BackGround> m_backGround;

public:
	ClearScene();
	~ClearScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
private:
	void DrawSpace();// �X�y�[�X�L�[�����Ă��Ă�`��
};
