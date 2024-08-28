#pragma once
#include "StepTimer.h"
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
class CommonResources;
class Fade
{
public:
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		float power;                             // ���x
		float fadeAmount;                       // �t�F�[�h�̐i�s�x�i0.0�`1.0�j
		DirectX::SimpleMath::Vector2 padding;// �p�f�B���O
	};

	enum class FadeState
	{
		None,// �Ȃ�
		// �ȉ��A�t�F�[�h�C���E�A�E�g�̊J�n���
		FadeIn,// �t�F�[�h�C��
		FadeOut,// �t�F�[�h�A�E�g
		// �ȉ��A�t�F�[�h�C���E�A�E�g�̓r�����
		FadeInEnd,// �t�F�[�h�C���I��
		FadeOutEnd,// �t�F�[�h�A�E�g�I��

	};

private:
	//	�ϐ�
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DX::StepTimer                           m_timer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	//	�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	FadeState m_fadeState;
	// ����
	float m_time;
public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//	�R���X�g���N�^
	Fade(CommonResources* commonResources);
	//	�f�X�g���N�^
	~Fade() = default;
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);

	void Update(float elapsedTime, FadeState state);

	void Render();

	FadeState GetState() const { return m_fadeState; }
private:

	void CreateShader();
};