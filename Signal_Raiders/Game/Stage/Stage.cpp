#include "pch.h"

#include "Game/Stage/Stage.h"



Stage::Stage()
	: m_commonResources(nullptr)
	, m_pBatchEffect(nullptr)
	, m_pPrimitiveBatch(nullptr)
	, m_pInputLayout(nullptr)
	, m_pStates(nullptr)
	, m_pTexture(nullptr)
	, m_depthStencilState_Floor(nullptr)
{
}
Stage::~Stage() {}

void Stage::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	// �G�t�F�N�g�̍쐬 
	m_pBatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_pBatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_pBatchEffect->SetReferenceAlpha(255);
	// ���̓��C�A�E�g���� 
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_pBatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_pInputLayout.GetAddressOf()
	);
	// ���ʃX�e�[�g�̍쐬
	m_pStates = std::make_unique<CommonStates>(device);

	// ���e�N�X�`���̃��[�h 
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tile.png",
		nullptr,
		m_pTexture.GetAddressOf()
	);



}

void Stage::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	//	�v���~�e�B�u�o�b�`�̍쐬 
	m_pPrimitiveBatch =
		std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	���_���i���̒��_�j 
	VertexPositionTexture vertex[4] =
	{//												���W					UV���W�i�ӂ���0�`1�̊ԂŎw��B�������ꍇ�͌J��Ԃ�
		VertexPositionTexture(SimpleMath::Vector3(100.0f, 0.00f, 100.0f),	SimpleMath::Vector2(.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.0f, 100.0f),	SimpleMath::Vector2(25.0f, .0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.f, -100.0f),	SimpleMath::Vector2(25.0f,25.0f)),
		VertexPositionTexture(SimpleMath::Vector3(100.0f,0.f, -100.0f),	SimpleMath::Vector2(0.0f, 25.0f)),
	};

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ��� 
	context->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);
	//	�e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j 
	ID3D11SamplerState* samplers[1] = { m_pStates->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	�J�����O�͍�����i�����v���j 
	context->RSSetState(m_pStates->CullNone());

	//	�s�����̂ݕ`�悷��ݒ� 
	m_pBatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_pBatchEffect->SetReferenceAlpha(0);
	m_pBatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_pBatchEffect->SetView(view);
	m_pBatchEffect->SetProjection(proj);
	m_pBatchEffect->SetTexture(m_pTexture.Get());
	m_pBatchEffect->Apply(context);
	context->IASetInputLayout(m_pInputLayout.Get());


	//	������������`�� 
	m_pPrimitiveBatch->Begin();
	m_pPrimitiveBatch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_pPrimitiveBatch->End();


}
