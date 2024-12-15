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

	// �������i���_�A�e�N�X�`���j
	static void InitializePositionTexture(DX::DeviceResources* pDR);
	// �������i���_�A�F�A�e�N�X�`���j
	static void InitializePositionColorTexture(DX::DeviceResources* pDR);
	// �`��J�n�i���_�A�e�N�X�`���j
	static void DrawStartTexture(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures);
	// �`��J�n�i���_�A�F�A�e�N�X�`���j
	static void DrawStartColorTexture(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures);
	// �T�u���\�[�X�̍X�V
	static void UpdateSubResources(ID3D11DeviceContext1* context, ID3D11Resource* resource, const void* pSrcData);
	// �|���S���`��i���_�A�e�N�X�`���j
	static void DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices);
	// �|���S���`��i���_�A�F�A�e�N�X�`���j
	static void DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count);
	// ����i���_�A�e�N�X�`���j
	static void ReleasePositionTexture();
	// ����i���_�A�F�A�e�N�X�`���j
	static void ReleasePositionColorTexture();

private:
	// �x�[�V�b�N�G�t�F�N�g
	static std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���~�e�B�u�o�b�`�i���_�A�e�N�X�`���j
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatchTexture;
	// �v���~�e�B�u�o�b�`�i���_�A�F�A�e�N�X�`���j
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatchColorTexture;
	// ���ʃ��\�[�X
	static CommonResources* m_commonResources;
	//�R�����X�e�[�g
	static std::unique_ptr<DirectX::CommonStates> m_states;
};