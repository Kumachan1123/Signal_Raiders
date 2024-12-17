/*
	@file	StageSelect.h
	@brief	�X�e�[�W�I���N���X
*/
#pragma once
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Game/KumachiLib//BinaryFile.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
// �O���錾
class CommonResources;

class StageSelect final
{
public:
	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;//	���[���h�s��
		DirectX::SimpleMath::Matrix		matView;	//	�r���[�s��
		DirectX::SimpleMath::Matrix		matProj;	//	�v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4	color;
		DirectX::SimpleMath::Vector4	time;	//	����
	};
	ConstBuffer m_constBuffer;
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �ϐ�
	DX::DeviceResources* m_pDR;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@
	ConstBuffer m_ConstBuffer;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// �e�N�X�`���p�X
	wchar_t* m_texturePath;
	// ����
	float m_time;
	// ��̍s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// ���_���
	DirectX::VertexPositionTexture m_vertex[4];
public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	StageSelect(CommonResources* resources);
	~StageSelect();
	void LoadTexture(const wchar_t* path);

	//	������
	void Create(DX::DeviceResources* pDR);

	//	�X�V
	void Update(float elapsedTime);
	//	�`��
	void Render();
private:
	//	�V�F�[�_�[���쐬
	void CreateShader();
};