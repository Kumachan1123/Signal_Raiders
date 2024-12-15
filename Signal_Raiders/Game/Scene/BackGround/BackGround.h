//--------------------------------------------------------------------------------------
// File: Particle.h
//
// �p�[�e�B�N���N���X
//
//-------------------------------------------------------------------------------------

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
class BackGround
{
public:
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;	// ����
	}m_constBuffer;

private:
	//	�ϐ�
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;

	DX::StepTimer                           m_timer;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	//	���[���h�r���[�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// ����
	float m_time;
public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	BackGround(CommonResources* resources);
	~BackGround();

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);

	void Update(float elapsedTime);

	void Render();
private:

	void CreateShader();
};