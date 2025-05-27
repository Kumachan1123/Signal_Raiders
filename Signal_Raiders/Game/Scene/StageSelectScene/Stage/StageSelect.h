/*
*	@file	StageSelect.h
*	@brief	�X�e�[�W�I���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
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
		DirectX::SimpleMath::Vector4	color;	// 	�F
		DirectX::SimpleMath::Vector4	time;	//	����
	};
public:// public�֐�
	StageSelect(CommonResources* resources);// �R���X�g���N�^
	~StageSelect();// �f�X�g���N�^
	void LoadTexture(const wchar_t* path);// �e�N�X�`���ǂݍ���
	void Create(DX::DeviceResources* pDR);// ������
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
private:// private�֐�
	void CreateShaders();// �V�F�[�_�[���쐬
public:
	// public�萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// private�萔
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬
	CreateShader* m_pCreateShader;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �ϐ�
	DX::DeviceResources* m_pDR;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@
	ConstBuffer m_ConstBuffer;
	// �摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// �e�N�X�`���p�X
	wchar_t* m_texturePath;
	// ����
	float m_time;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// ���_���
	DirectX::VertexPositionTexture m_vertex[4];
};