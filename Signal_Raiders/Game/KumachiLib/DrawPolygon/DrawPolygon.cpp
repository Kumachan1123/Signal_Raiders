#include "pch.h"
#include "DrawPolygon.h"
//�V���O���g���ɂ���

std::unique_ptr<DrawPolygon> DrawPolygon::m_instance = nullptr;

DrawPolygon* const DrawPolygon::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new DrawPolygon());
	}
	return m_instance.get();
}

// �R���X�g���N�^
DrawPolygon::DrawPolygon()
	:
	m_primitiveBatchTexture(nullptr),
	m_primitiveBatchColorTexture(nullptr)
{
}

// �f�X�g���N�^
DrawPolygon::~DrawPolygon()
{
	// �v���~�e�B�u�o�b�`�̉��
	ReleasePositionTexture();
	ReleasePositionColorTexture();
}

// �������i���_�A�e�N�X�`���j
void DrawPolygon::InitializePositionTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = pDR->GetD3DDevice();

	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	m_states = std::make_unique<CommonStates>(device);
}

// �������i���_�A�F�A�e�N�X�`���j
void DrawPolygon::InitializePositionColorTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = pDR->GetD3DDevice();
	//	�v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);
}

// �`��J�n�i���_�A�e�N�X�`���j
void DrawPolygon::DrawStart(ID3D11DeviceContext1* context, ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures)
{


	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	//	�������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//	�J�����O�͍�����
	context->RSSetState(m_states->CullNone());
	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	for (int i = 0; i < textures.size(); i++)
	{
		//	for���ň�C�ɐݒ肷��
		context->PSSetShaderResources(i, 1, textures[i].GetAddressOf());
	}
	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(pInputLayout);
}

// �T�u���\�[�X�̍X�V
void DrawPolygon::UpdateSubResources(ID3D11DeviceContext1* context, ID3D11Resource* resource, const void* pSrcData)
{
	context->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);
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
	m_primitiveBatchTexture.reset();
}


// ���
void DrawPolygon::ReleasePositionColorTexture()
{
	m_primitiveBatchColorTexture.reset();
}

void DrawPolygon::SetShaderBuffer(ID3D11DeviceContext1* context, UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	context->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	context->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	context->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
}

void DrawPolygon::SetShader(ID3D11DeviceContext1* context, const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	if (shaders.vs.Get() != nullptr) context->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.gs.Get() != nullptr) context->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances);
	if (shaders.ps.Get() != nullptr) context->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances);
}

void DrawPolygon::ReleaseShader(ID3D11DeviceContext1* context)
{
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
