/*
	@file	TitleLogo.h
	@brief	�^�C�g�����S�N���X
*/
#pragma once
#include <DeviceResources.h>
// �O���錾
class CommonResources;

class TitleLogo final
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
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �ϐ�
	DX::DeviceResources* m_pDR;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@
	ConstBuffer m_ConstBuffer;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �^�C�g���摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_titleTexture;
	// ����
	float m_time;
	// ��̍s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	TitleLogo(CommonResources* resources);
	~TitleLogo();
	void LoadTexture(const wchar_t* path);

	//	������
	void Create(DX::DeviceResources* pDR);

	//	�X�V
	void Update(float elapsedTime);
	//	�`��
	void Render();
private:

	void CreateShader();
};