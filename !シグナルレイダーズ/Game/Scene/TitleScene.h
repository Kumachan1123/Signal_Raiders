/*
	@file	TitleScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
#include "IScene.h"
#include <DeviceResources.h>

// �O���錾
class CommonResources;
class Fade;
namespace mylib
{

	class GridFloor;
}
class FPS_Camera;
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}
class TitleScene final :
	public IScene
{
public:
	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;//	���[���h�s��
		DirectX::SimpleMath::Matrix		matView;	//	�r���[�s��
		DirectX::SimpleMath::Matrix		matProj;	//	�v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4	Colors;
		float time;
		DirectX::SimpleMath::Vector3 padding;
	};
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�ϐ�
	DX::DeviceResources* m_pDR;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �^�C�g���摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_titleTexture;
	// �w��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pressKeyTexture;
	// �w�i�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;


	// �e�N�X�`���̔����̑傫��
	DirectX::SimpleMath::Vector2 m_titleTexCenter;
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// �t�F�[�h
	std::unique_ptr<Fade> m_fade;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// FMOD�Ŏg�p����ϐ��i�|�C���^�j
	FMOD::System* m_system;	// FMOD�̃V�X�e��
	FMOD::Sound* m_soundSE;	// SE�p�̉����f�[�^
	FMOD::Sound* m_soundBGM;	// BGM�p�̉����f�[�^
	FMOD::Channel* m_channelSE;	// SE���Đ�����`�����l��
	FMOD::Channel* m_channelBGM;// BGM���Đ�����`�����l��
	// FPS�J����
	std::unique_ptr<FPS_Camera> m_camera;
	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O
	float m_volume;		// �{�����[��
	int m_counter;		// �t�F�[�h�J�E���^

	float m_time = 0.0f;// �g�k�Ɏg������
	float m_size = 0.0f;// �摜�T�C�Y
public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	TitleScene();
	~TitleScene() override;

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
private:

	void CreateShader();
	void DrawBackground();// �w�i�`��
	void DrawSpace();// �X�y�[�X�L�[�����Ă��Ă�`��
	void DrawTitle();// �^�C�g���`��
};
