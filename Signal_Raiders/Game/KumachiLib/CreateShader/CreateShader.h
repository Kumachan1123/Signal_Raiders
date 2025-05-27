/*
*	@file CreateShaders.h
*	@brief �V�F�[�_�[���쐬����V���O���g���N���X
*	@details �|���S����`�悷��ۂɎg�p����V�F�[�_�[���쐬����
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <utility>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

class CreateShader
{
public:
	static CreateShader* const GetInstance();	// �V���O���g���C���X�^���X
public:
	// public�֐�
	~CreateShader();	// �f�X�g���N�^
	void Initialize(ID3D11Device1* device, // ������
		const D3D11_INPUT_ELEMENT_DESC* pIDE,
		UINT NumElements,
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout);
	void CreateVertexShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs);// ���_�V�F�[�_���쐬
	void CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);// �s�N�Z���V�F�[�_�[���쐬
	void CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);// �W�I���g���V�F�[�_�[���쐬
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout() { return m_pInputLayout; }// ������C���v�b�g���C�A�E�g��Ԃ�
	void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize);// �R���X�^���g�o�b�t�@���쐬
private:
	// private�֐�
	CreateShader();// �R���X�g���N�^
	CreateShader(const CreateShader&) = delete;// �R�s�[�R���X�g���N�^
	CreateShader& operator=(const CreateShader&) = delete;// ������Z�q�̋֎~
private:
	// �V���O���g���C���X�^���X
	static std::unique_ptr<CreateShader> m_pInstance;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// ���̓��C�A�E�g
	const D3D11_INPUT_ELEMENT_DESC* m_pIDE;
	// �v�f��
	UINT m_NumElements;
	// ���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};



