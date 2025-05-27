/*
*	@file	DrawPolygon.cpp
*	@brief	�|���S���`��p�N���X
*	@details �|���S����`�悷�邽�߂̃N���X
*/
#include <pch.h>
#include "DrawPolygon.h"

// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<DrawPolygon> DrawPolygon::m_pInstance = nullptr;
/*
*	@brief �V���O���g���C���X�^���X���擾
*	@details �V���O���g���C���X�^���X���擾����
*	@param �Ȃ�
*	@return �V���O���g���C���X�^���X
*/
DrawPolygon* const DrawPolygon::GetInstance()
{
	if (m_pInstance == nullptr)// �C���X�^���X���Ȃ��ꍇ
	{
		m_pInstance.reset(new DrawPolygon());// �C���X�^���X�𐶐�
	}
	return m_pInstance.get();// �C���X�^���X��Ԃ�
}

/*
*	@brief �R���X�g���N�^
*	@details �R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
DrawPolygon::DrawPolygon()
	: m_pPrimitiveBatchTexture(nullptr)// �v���~�e�B�u�o�b�`(���_�A�e�N�X�`��)
	, m_pPrimitiveBatchColorTexture(nullptr)// �v���~�e�B�u�o�b�`(���_�A�F�A�e�N�X�`��)
	, m_pStates(nullptr)// �R�����X�e�[�g
	, m_pDeviceContext(nullptr)	// �f�o�C�X�R���e�L�X�g
	, m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_pDevice(nullptr)// �f�o�C�X
{
}
/*
*	@brief �f�X�g���N�^
*	@details �f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
DrawPolygon::~DrawPolygon()
{
	ReleasePositionTexture();	// �v���~�e�B�u�o�b�`�̉���i���_�A�e�N�X�`���j
	ReleasePositionColorTexture();	// �v���~�e�B�u�o�b�`�̉���i���_�A�F�A�e�N�X�`���j
}
/*
*	@brief ������
*	@details �������i���_�A�e�N�X�`���j���s��
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void DrawPolygon::InitializePositionTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pDevice = pDR->GetD3DDevice();// �f�o�C�X���擾
	m_pDeviceContext = pDR->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	m_pPrimitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());// �v���~�e�B�u�o�b�`���쐬����
	m_pStates = std::make_unique<CommonStates>(m_pDevice);//	�R�����X�e�[�g���쐬����
}

/*
*	@brief ������
*	@details �������i���_�A�F�A�e�N�X�`���j���s��
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void DrawPolygon::InitializePositionColorTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pDevice = pDR->GetD3DDevice();// �f�o�C�X���擾
	m_pDeviceContext = pDR->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	m_pPrimitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());// �v���~�e�B�u�o�b�`�̍쐬
	m_pStates = std::make_unique<CommonStates>(m_pDevice);//	�R�����X�e�[�g���쐬����
}
/*
*	@brief �`��J�n
*	@details �`��J�n���ɌĂяo���֐�
*	@param pInputLayout ���̓��C�A�E�g
*	@param textures �e�N�X�`���z��
*	@return �Ȃ�
*/
void DrawPolygon::DrawStart(ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures)
{
	for (int i = 0; i < textures.size(); i++)// �s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
		m_pDeviceContext->PSSetShaderResources(i, 1, textures[i].GetAddressOf());// for���ň�C�ɐݒ肷��
	m_pDeviceContext->IASetInputLayout(pInputLayout);// �C���v�b�g���C�A�E�g�̓o�^
}
/*
*	@brief �`��O�ݒ�
*	@details �`��O�Ɋe��X�e�[�g��ݒ肷��
*	@param ss �T���v���[�X�e�[�g
*	@param bs �u�����h�X�e�[�g
*	@param rs ���X�^���C�U�[�X�e�[�g
*	@param dss �[�x�X�e���V���X�e�[�g
*	@return �Ȃ�
*/
void DrawPolygon::DrawSetting(SamplerStates ss, BlendStates bs, RasterizerStates rs, DepthStencilStates dss)
{
	ID3D11SamplerState* sampler[1];//	�摜�p�T���v���[�̓o�^
	switch (ss)// �T���v���[�X�e�[�g�̎�ނ𔻕ʂ��āA�T���v���[��ݒ肷��
	{
	case DrawPolygon::SamplerStates::ANISOTROPIC_CLAMP:// �A�j�\�g���s�b�N�N�����v
		sampler[0] = m_pStates->AnisotropicClamp();// �A�j�\�g���s�b�N�N�����v�̃T���v���[���擾
		break;
	case DrawPolygon::SamplerStates::ANISOTROPIC_WRAP:// �A�j�\�g���s�b�N���b�v
		sampler[0] = m_pStates->AnisotropicWrap(); // �A�j�\�g���s�b�N���b�v�̃T���v���[���擾
		break;
	case DrawPolygon::SamplerStates::LINEAR_CLAMP: // ���j�A�N�����v
		sampler[0] = m_pStates->LinearClamp(); // ���j�A�N�����v�̃T���v���[���擾
		break;
	case DrawPolygon::SamplerStates::LINEAR_WRAP: // ���j�A���b�v
		sampler[0] = m_pStates->LinearWrap(); // ���j�A���b�v�̃T���v���[���擾
		break;
	case DrawPolygon::SamplerStates::POINT_CLAMP: // �|�C���g�N�����v
		sampler[0] = m_pStates->PointClamp(); // �|�C���g�N�����v�̃T���v���[���擾
		break;
	case DrawPolygon::SamplerStates::POINT_WRAP: // �|�C���g���b�v
		sampler[0] = m_pStates->PointWrap(); // �|�C���g���b�v�̃T���v���[���擾
		break;
	default: // �f�t�H���g��nullptr
		sampler[0] = nullptr; // �T���v���[��nullptr�ɐݒ�
		break;
	}
	m_pDeviceContext->PSSetSamplers(0, 1, sampler);// �s�N�Z���V�F�[�_�ɃT���v���[��o�^����
	ID3D11BlendState* blendState;// �u�����h�X�e�[�g�̓o�^
	switch (bs)// �u�����h�X�e�[�g�̎�ނ𔻕ʂ��āA�u�����h�X�e�[�g��ݒ肷��
	{
	case DrawPolygon::BlendStates::ALPHA:// �A���t�@�u�����h
		blendState = m_pStates->AlphaBlend(); // �A���t�@�u�����h�̃u�����h�X�e�[�g���擾
		break;
	case DrawPolygon::BlendStates::ADDITIVE: // ���Z�u�����h
		blendState = m_pStates->Additive(); // ���Z�u�����h�̃u�����h�X�e�[�g���擾
		break;
	case DrawPolygon::BlendStates::OPAQUE: // �s����
		blendState = m_pStates->Opaque(); // �s�����̃u�����h�X�e�[�g���擾
		break;
	case DrawPolygon::BlendStates::NONPREMULTIPLIED: // ��v���}���`�v���C�h
		blendState = m_pStates->NonPremultiplied(); // ��v���}���`�v���C�h�̃u�����h�X�e�[�g���擾
		break;
	default: // �f�t�H���g��nullptr
		blendState = nullptr; // �u�����h�X�e�[�g��nullptr�ɐݒ�
		break;
	}
	m_pDeviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);// �u�����h�X�e�[�g��ݒ肷��

	ID3D11DepthStencilState* depthStencilState;// �[�x�X�e���V���X�e�[�g�̓o�^
	switch (dss)// �[�x�X�e���V���X�e�[�g�̎�ނ𔻕ʂ��āA�[�x�X�e���V���X�e�[�g��ݒ肷��
	{
	case DrawPolygon::DepthStencilStates::DEPTH_DEFAULT:// �f�t�H���g
		depthStencilState = m_pStates->DepthDefault(); // �f�t�H���g�̐[�x�X�e���V���X�e�[�g���擾
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_NONE: // �[�x�Ȃ�
		depthStencilState = m_pStates->DepthNone(); // �[�x�Ȃ��̐[�x�X�e���V���X�e�[�g���擾
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ: // �[�x�ǂݎ��
		depthStencilState = m_pStates->DepthRead(); // �[�x�ǂݎ��̐[�x�X�e���V���X�e�[�g���擾
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ_REVERSE_Z: // �[�x�ǂݎ��i�tZ�j
		depthStencilState = m_pStates->DepthReadReverseZ(); // �[�x�ǂݎ��i�tZ�j�̐[�x�X�e���V���X�e�[�g���擾
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_REVERSE_Z: // �tZ
		depthStencilState = m_pStates->DepthReverseZ(); // �tZ�̐[�x�X�e���V���X�e�[�g���擾
		break;
	default: // �f�t�H���g��nullptr
		depthStencilState = nullptr; // �[�x�X�e���V���X�e�[�g��nullptr�ɐݒ�
		break;
	}
	m_pDeviceContext->OMSetDepthStencilState(depthStencilState, 0);// �[�x�X�e���V���X�e�[�g��ݒ肷��

	ID3D11RasterizerState* rasterizerState;// ���X�^���C�U�[�X�e�[�g�̓o�^
	switch (rs)// ���X�^���C�U�[�X�e�[�g�̎�ނ𔻕ʂ��āA���X�^���C�U�[�X�e�[�g��ݒ肷��
	{
	case DrawPolygon::RasterizerStates::CULL_CLOCKWISE: // ���v���
		rasterizerState = m_pStates->CullClockwise(); // ���v���̃��X�^���C�U�[�X�e�[�g���擾
		break;
	case DrawPolygon::RasterizerStates::CULL_COUNTERCLOCKWISE: // �����v���
		rasterizerState = m_pStates->CullCounterClockwise(); // �����v���̃��X�^���C�U�[�X�e�[�g���擾
		break;
	case DrawPolygon::RasterizerStates::CULL_NONE: // �J�����O�Ȃ�
		rasterizerState = m_pStates->CullNone(); // �J�����O�Ȃ��̃��X�^���C�U�[�X�e�[�g���擾
		break;
	case DrawPolygon::RasterizerStates::WIREFRAME: // ���C���[�t���[��
		rasterizerState = m_pStates->Wireframe(); // ���C���[�t���[���̃��X�^���C�U�[�X�e�[�g���擾
		break;
	default: // �f�t�H���g��nullptr
		rasterizerState = nullptr; // ���X�^���C�U�[�X�e�[�g��nullptr�ɐݒ�
		break;
	}
	m_pDeviceContext->RSSetState(rasterizerState); // ���X�^���C�U�[�X�e�[�g��ݒ肷��

}

/*
*	@brief ���\�[�X�̍X�V
*	@details ���\�[�X�̍X�V���s��
*	@param resource �X�V���郊�\�[�X
*	@param pSrcData �X�V����f�[�^
*	@return �Ȃ�
*/
void DrawPolygon::UpdateSubResources(ID3D11Resource* resource, const void* pSrcData)
{
	m_pDeviceContext->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);// �T�u���\�[�X�̍X�V���s��
}

/*
*	@brief �|���S���`��
*	@details �|���S����`�悷��(VertexPositionTexture�p)
*	@param vertices ���_���(VertexPositionTexture)
*	@return �Ȃ�
*/
void DrawPolygon::DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices)
{
	m_pPrimitiveBatchTexture->Begin();// �`��J�n
	m_pPrimitiveBatchTexture->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);// �l�p�`��`��
	m_pPrimitiveBatchTexture->End();// �`��I��

}
/*
*	@brief �|���S���`��
*	@details �|���S����`�悷��(VertexPositionColorTexture�p)
*	@param topology �v���~�e�B�u�g�|���W�[
*	@param vertices ���_���(VertexPositionColorTexture)
*	@param count ���_��
*	@return �Ȃ�
*/
void DrawPolygon::DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count)
{
	m_pPrimitiveBatchColorTexture->Begin();// �`��J�n
	m_pPrimitiveBatchColorTexture->Draw(topology, vertices, count); // ���_��`��
	m_pPrimitiveBatchColorTexture->End(); // �`��I��
}
/*
*	@brief ���
*	@details ����i���_�A�e�N�X�`���j���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawPolygon::ReleasePositionTexture()
{
	m_pPrimitiveBatchTexture.reset();// �v���~�e�B�u�o�b�`(���_�A�e�N�X�`��)�̉��
	m_pStates.reset(); // �R�����X�e�[�g�̉��
	m_pDeviceContext = nullptr; // �f�o�C�X�R���e�L�X�g�̉��
	m_pDR = nullptr; // �f�o�C�X���\�[�X�̉��
	m_pDevice = nullptr; // �f�o�C�X�̉��
}
/*
*	@brief ���
*	@details ����i���_�A�F�A�e�N�X�`���j���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawPolygon::ReleasePositionColorTexture()
{
	m_pPrimitiveBatchColorTexture.reset();// �v���~�e�B�u�o�b�`(���_�A�F�A�e�N�X�`��)�̉��
	m_pStates.reset(); // �R�����X�e�[�g�̉��
	m_pDeviceContext = nullptr; // �f�o�C�X�R���e�L�X�g�̉��
	m_pDR = nullptr; // �f�o�C�X���\�[�X�̉��
	m_pDevice = nullptr; // �f�o�C�X�̉��
}
/*
*	@brief �V�F�[�_�[�Ƀo�b�t�@�𑗂�
*	@details �V�F�[�_�[�Ƀo�b�t�@�𑗂�
*	@param startSlot �V�F�[�_�[�̊J�n�X���b�g
*	@param numBuffers ����o�b�t�@�̐�
*	@param ppBuffer ����o�b�t�@�̔z��
*	@return �Ȃ�
*/
void DrawPolygon::SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	m_pDeviceContext->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);// ���_�V�F�[�_�[�Ƀo�b�t�@�𑗂�
	m_pDeviceContext->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer); // �W�I���g���V�F�[�_�[�Ƀo�b�t�@�𑗂�
	m_pDeviceContext->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer); // �s�N�Z���V�F�[�_�[�Ƀo�b�t�@�𑗂�
}
/*
*	@brief �V�F�[�_�[���Z�b�g����
*	@details �V�F�[�_�[���Z�b�g����
*	@param shaders �V�F�[�_�[�̍\����
*	@param ppClassInstances �N���X�C���X�^���X�̔z��
*	@param nubClassInstances �N���X�C���X�^���X�̐�
*	@return �Ȃ�
*/
void DrawPolygon::SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	if (shaders.vs.Get() != nullptr) m_pDeviceContext->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);// ���_�V�F�[�_�[���Z�b�g����
	if (shaders.gs.Get() != nullptr) m_pDeviceContext->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances); // �W�I���g���V�F�[�_�[���Z�b�g����
	if (shaders.ps.Get() != nullptr) m_pDeviceContext->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances); // �s�N�Z���V�F�[�_�[���Z�b�g����
}
/*
*	@brief �V�F�[�_�[���������
*	@details �V�F�[�_�[���������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawPolygon::ReleaseShader()
{
	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);// ���_�V�F�[�_�[�����
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0); // �W�I���g���V�F�[�_�[�����
	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0);// �s�N�Z���V�F�[�_�[�����
}
