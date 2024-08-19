/*
	@file	GameOverScene.h
	@brief	�Q�[���I�[�o�[�N���X
*/
#pragma once
#include "IScene.h"

// �O���錾
class CommonResources;

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
class GameOverScene final :
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
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gameoverTexture;
	// �w��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTexture;
	// �e�N�X�`���̔����̑傫��
	DirectX::SimpleMath::Vector2 m_titleTexCenter;
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// FMOD�Ŏg�p����ϐ��i�|�C���^�j
	FMOD::System* m_system;	// FMOD�̃V�X�e��
	FMOD::Sound* m_soundSE;	// SE�p�̉����f�[�^
	FMOD::Sound* m_soundBGM;	// BGM�p�̉����f�[�^
	FMOD::Channel* m_channelSE;	// SE���Đ�����`�����l��
	FMOD::Channel* m_channelBGM;// BGM���Đ�����`�����l��

	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O
	float m_volume;		// �{�����[��
	int m_counter;		// �t�F�[�h�J�E���^
	float m_time = 0.0f;// �g�k�Ɏg������
	float m_size = 0.0f;// �摜�T�C�Y
	float m_cleatrSize = 0.0f;
public:
	GameOverScene();
	~GameOverScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
};
