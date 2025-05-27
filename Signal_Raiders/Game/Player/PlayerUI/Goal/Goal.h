/*
*	@file	Goal.h
*	@brief	�ڕW��\������N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
 // �O���錾
class CommonResources;
class Goal
{
public:// �\����
	// �V�F�[�_�[�ɓn���f�[�^�\����
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;		// ����
	};
public:// public�֐�
	Goal(CommonResources* resources);// �R���X�g���N�^
	~Goal();// �f�X�g���N�^
	void LoadTexture(const wchar_t* path);// �e�N�X�`�����\�[�X�ǂݍ���
	void Create(DX::DeviceResources* pDR);// ����
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
private:// private�֐�
	void CreateShaders();// �V�F�[�_�[�̍쐬
private:// �萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// �C���v�b�g���C�A�E�g
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �R���X�^���g�o�b�t�@(�V�F�[�_�[�ɑ���f�[�^)
	ConstBuffer m_constBuffer;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// ����
	float m_time;

};