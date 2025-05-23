/*
*	@file BackGround.h
*	@brief �w�i�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
#include <WICTextureLoader.h>
// �O�����C�u����
#include "Libraries/MyLib/DebugString.h"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
// �O���錾
class CommonResources;
class BackGround
{
public:
	// �V�F�[�_�[�ɓn���\����
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;		// ����
	};
public:// public�֐�
	BackGround(CommonResources* resources);// �R���X�g���N�^
	~BackGround();// �f�X�g���N�^
	void LoadTexture(const wchar_t* path);// �e�N�X�`�����\�[�X�ǂݍ���
	void Create(DX::DeviceResources* pDR);// ����
	void Update(float elapsedTime); // �X�V
	void Render();// �`��
private:// private�֐�
	void CreateShaders();// �V�F�[�_�[�쐬
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �f�o�C�X�R���e�L�X�g
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
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
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// ����
	float m_time;
	// �V�F�[�_�[�ɓn���F
	DirectX::SimpleMath::Vector4 m_color;
public:
	// �萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
};