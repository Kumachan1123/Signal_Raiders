/*
	@file	DrawPolygon.h
	@brief	�����蔻��`��p�N���X
*/
#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
//�O���錾
class CommonResources;

class DrawPolygon
{
public:
	// �O�̃V�F�[�_�[���ЂƂ܂Ƃ߂ɂ��đ��邽�߂̍\����
	struct Shaders
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> gs;
	};
public:
	// �V���O���g���C���X�^���X���擾
	static DrawPolygon* const GetInstance();

public:

	// �f�X�g���N�^
	~DrawPolygon();

	// �������i���_�A�e�N�X�`���j
	void InitializePositionTexture(DX::DeviceResources* pDR);
	// �������i���_�A�F�A�e�N�X�`���j
	void InitializePositionColorTexture(DX::DeviceResources* pDR);
	// �`��J�n
	void DrawStart(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures);
	// �T�u���\�[�X�̍X�V
	void UpdateSubResources(ID3D11DeviceContext1* context, ID3D11Resource* resource, const void* pSrcData);
	// �|���S���`��i���_�A�e�N�X�`���j
	void DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices);
	// �|���S���`��i���_�A�F�A�e�N�X�`���j
	void DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count);
	// ����i���_�A�e�N�X�`���j
	void ReleasePositionTexture();
	// ����i���_�A�F�A�e�N�X�`���j
	void ReleasePositionColorTexture();
	// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
	void SetShaderBuffer(ID3D11DeviceContext1* context, UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer);
	// �V�F�[�_�[���Z�b�g����
	void SetShader(ID3D11DeviceContext1* context, const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances);
	// �V�F�[�_�[���������
	void ReleaseShader(ID3D11DeviceContext1* context);
private:
	// �R���X�g���N�^
	DrawPolygon();
	// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
	DrawPolygon(const DrawPolygon&) = delete;
	DrawPolygon& operator=(const DrawPolygon&) = delete;

private:
	// �v���~�e�B�u�o�b�`�i���_�A�e�N�X�`���j
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatchTexture;
	// �v���~�e�B�u�o�b�`�i���_�A�F�A�e�N�X�`���j
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatchColorTexture;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �V���O���g���C���X�^���X
	static std::unique_ptr<DrawPolygon> m_instance;
};