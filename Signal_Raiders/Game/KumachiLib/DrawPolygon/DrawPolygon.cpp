/*
	@file	DrawPolygon.cpp
	@brief	�|���S���`��p�N���X
	@details �|���S����`�悷�邽�߂̃N���X
*/
#include "pch.h"
#include "DrawPolygon.h"

// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<DrawPolygon> DrawPolygon::m_instance = nullptr;
/*
*	@brief �V���O���g���C���X�^���X���擾
*	@details �V���O���g���C���X�^���X���擾����
*	@param �Ȃ�
*	@return �V���O���g���C���X�^���X
*/
DrawPolygon* const DrawPolygon::GetInstance()
{
	if (m_instance == nullptr)// �C���X�^���X���Ȃ��ꍇ
	{
		m_instance.reset(new DrawPolygon());// �C���X�^���X�𐶐�
	}
	return m_instance.get();// �C���X�^���X��Ԃ�
}

/*
*	@brief �R���X�g���N�^
*	@details �R���X�g���N�^
*	@param �Ȃ�
*/
DrawPolygon::DrawPolygon()
	: m_primitiveBatchTexture(nullptr)// �v���~�e�B�u�o�b�`(���_�A�e�N�X�`��)
	, m_primitiveBatchColorTexture(nullptr)// �v���~�e�B�u�o�b�`(���_�A�F�A�e�N�X�`��)
	, m_states(nullptr)// �R�����X�e�[�g
	, m_context(nullptr)	// �f�o�C�X�R���e�L�X�g
	, m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_device(nullptr)// �f�o�C�X
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
	m_device = pDR->GetD3DDevice();// �f�o�C�X���擾
	m_context = pDR->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	m_primitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());// �v���~�e�B�u�o�b�`���쐬����
	m_states = std::make_unique<CommonStates>(m_device);//	�R�����X�e�[�g���쐬����
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
	m_device = pDR->GetD3DDevice();// �f�o�C�X���擾
	m_context = pDR->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	m_primitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());// �v���~�e�B�u�o�b�`�̍쐬
	m_states = std::make_unique<CommonStates>(m_device);//	�R�����X�e�[�g���쐬����
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
		m_context->PSSetShaderResources(i, 1, textures[i].GetAddressOf());// for���ň�C�ɐݒ肷��
	m_context->IASetInputLayout(pInputLayout);// �C���v�b�g���C�A�E�g�̓o�^
}

// �`��O�ݒ�
void DrawPolygon::DrawSetting(SamplerStates ss, BlendStates bs, RasterizerStates rs, DepthStencilStates dss)
{
	ID3D11SamplerState* sampler[1];//	�摜�p�T���v���[�̓o�^
	switch (ss)
	{
	case DrawPolygon::SamplerStates::ANISOTROPIC_CLAMP:
		sampler[0] = m_states->AnisotropicClamp();
		break;
	case DrawPolygon::SamplerStates::ANISOTROPIC_WRAP:
		sampler[0] = m_states->AnisotropicWrap();
		break;
	case DrawPolygon::SamplerStates::LINEAR_CLAMP:
		sampler[0] = m_states->LinearClamp();
		break;
	case DrawPolygon::SamplerStates::LINEAR_WRAP:
		sampler[0] = m_states->LinearWrap();
		break;
	case DrawPolygon::SamplerStates::POINT_CLAMP:
		sampler[0] = m_states->PointClamp();
		break;
	case DrawPolygon::SamplerStates::POINT_WRAP:
		sampler[0] = m_states->PointWrap();
		break;
	default:
		sampler[0] = nullptr;
		break;
	}
	m_context->PSSetSamplers(0, 1, sampler);

	//	�������菈��
	ID3D11BlendState* blendState;
	switch (bs)
	{
	case DrawPolygon::BlendStates::ALPHA:
		blendState = m_states->AlphaBlend();
		break;
	case DrawPolygon::BlendStates::ADDITIVE:
		blendState = m_states->Additive();
		break;
	case DrawPolygon::BlendStates::OPAQUE:
		blendState = m_states->Opaque();
		break;
	case DrawPolygon::BlendStates::NONPREMULTIPLIED:
		blendState = m_states->NonPremultiplied();
		break;
	default:
		blendState = nullptr;
		break;
	}
	m_context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	ID3D11DepthStencilState* depthStencilState;
	switch (dss)
	{
	case DrawPolygon::DepthStencilStates::DEPTH_DEFAULT:
		depthStencilState = m_states->DepthDefault();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_NONE:
		depthStencilState = m_states->DepthNone();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ:
		depthStencilState = m_states->DepthRead();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ_REVERSE_Z:
		depthStencilState = m_states->DepthReadReverseZ();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_REVERSE_Z:
		depthStencilState = m_states->DepthReverseZ();
		break;
	default:
		depthStencilState = nullptr;
		break;
	}
	m_context->OMSetDepthStencilState(depthStencilState, 0);

	// �J�����O�ݒ�
	ID3D11RasterizerState* rasterizerState;
	switch (rs)
	{
	case DrawPolygon::RasterizerStates::CULL_CLOCKWISE:
		rasterizerState = m_states->CullClockwise();
		break;
	case DrawPolygon::RasterizerStates::CULL_COUNTERCLOCKWISE:
		rasterizerState = m_states->CullCounterClockwise();
		break;
	case DrawPolygon::RasterizerStates::CULL_NONE:
		rasterizerState = m_states->CullNone();
		break;
	case DrawPolygon::RasterizerStates::WIREFRAME:
		rasterizerState = m_states->Wireframe();
		break;
	default:
		rasterizerState = nullptr;
		break;
	}
	m_context->RSSetState(rasterizerState);

}

// �T�u���\�[�X�̍X�V
void DrawPolygon::UpdateSubResources(ID3D11Resource* resource, const void* pSrcData)
{
	m_context->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);
}

// �|���S���`��i���_�A�e�N�X�`���j
void DrawPolygon::DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices)
{
	m_primitiveBatchTexture->Begin();
	m_primitiveBatchTexture->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	m_primitiveBatchTexture->End();

}

// �|���S���`��i���_�A�F�A�e�N�X�`���j
void DrawPolygon::DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count)
{
	m_primitiveBatchColorTexture->Begin();
	m_primitiveBatchColorTexture->Draw(topology, vertices, count);
	m_primitiveBatchColorTexture->End();
}

void DrawPolygon::ReleasePositionTexture()
{
	// �S�|�C���^�[�̉��
	m_primitiveBatchTexture.reset();
	m_states.reset();
	m_context = nullptr;
	m_pDR = nullptr;
	m_device = nullptr;
}


// ���
void DrawPolygon::ReleasePositionColorTexture()
{
	m_primitiveBatchColorTexture.reset();
	m_states.reset();
	m_context = nullptr;
	m_pDR = nullptr;
	m_device = nullptr;
}

void DrawPolygon::SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	m_context->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	m_context->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	m_context->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
}

void DrawPolygon::SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	if (shaders.vs.Get() != nullptr) m_context->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.gs.Get() != nullptr) m_context->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.ps.Get() != nullptr) m_context->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances);
}

void DrawPolygon::ReleaseShader()
{
	m_context->VSSetShader(nullptr, nullptr, 0);
	m_context->GSSetShader(nullptr, nullptr, 0);
	m_context->PSSetShader(nullptr, nullptr, 0);
}
