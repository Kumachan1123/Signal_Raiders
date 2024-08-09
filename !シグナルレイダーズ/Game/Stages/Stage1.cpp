#include "pch.h"

#include "Game//Stages/Stage1.h"


Stage1::Stage1()
	: m_commonResources(nullptr)
	, m_BatchEffect(nullptr)
	, m_Batch(nullptr)
	, m_InputLayout(nullptr)
	, m_States(nullptr)
	, m_Texture(nullptr)
	, m_depthStencilState_Floor(nullptr)
{

}
Stage1::~Stage1() {}

void Stage1::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	//auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	�G�t�F�N�g�̍쐬 
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);
	//	���̓��C�A�E�g���� 
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	//	���ʃX�e�[�g�̍쐬
	m_States = std::make_unique<CommonStates>(device);

	//	�e�N�X�`���̃��[�h 
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tile.png",
		nullptr,
		m_Texture.GetAddressOf()
	);
	// �[�x�X�e���V���o�b�t�@������������
	this->InitializeDepthStencilState(device);
}
void Stage1::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

}
void Stage1::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	//	�v���~�e�B�u�o�b�`�̍쐬 
	m_Batch =
		std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	���_���i�|���S���̒��_�j 
	VertexPositionTexture vertex[4] =
	{//												���W					UV���W�i�ӂ���0�`1�̊ԂŎw��B�������ꍇ�͌J��Ԃ�
		VertexPositionTexture(SimpleMath::Vector3(100.0f, 0.00f, 100.0f),	SimpleMath::Vector2(.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.0f, 100.0f),	SimpleMath::Vector2(50.0f, .0f)),
		VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.f, -100.0f),	SimpleMath::Vector2(50.0f,50.0f)),
		VertexPositionTexture(SimpleMath::Vector3(100.0f,0.f, -100.0f),	SimpleMath::Vector2(0.0f, 50.0f)),
	};


	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ��� 
	context->OMSetDepthStencilState(m_depthStencilState_Floor.Get(), 0);
	//	�e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j 
	ID3D11SamplerState* samplers[1] = { m_States->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	�J�����O�͍�����i�����v���j 
	context->RSSetState(m_States->CullCounterClockwise());

	//	�s�����̂ݕ`�悷��ݒ� 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_Texture.Get());

	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	������������`�� 
	m_Batch->Begin();
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}
//---------------------------------------------------------
// �[�x�X�e���V���X�e�[�g������������
//---------------------------------------------------------
void Stage1::InitializeDepthStencilState(ID3D11Device* device)
{
	assert(device);

	// �[�x�X�e���V���o�b�t�@��ݒ肷��
	D3D11_DEPTH_STENCIL_DESC desc{};

	/*
		���̐ݒ�
	*/
	// ���i�`�掞�O���P�ɂ���j
	desc.DepthEnable = TRUE;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �e�X�g�̏����F�[�x�l�ȉ��Ȃ灨��O�Ȃ�

	desc.StencilEnable = TRUE;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// �\��
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// �Q�ƒl���X�e���V���l�Ɠ����Ȃ�F�[���Ȃ�
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK�@�X�e���V���l���C���N�������g����
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG�@�������Ȃ�

	// ���ʂ������ݒ�
	desc.BackFace = desc.FrontFace;

	// �[�x�X�e���V���X�e�[�g���쐬����
	DX::ThrowIfFailed(
		device->CreateDepthStencilState(&desc, m_depthStencilState_Floor.ReleaseAndGetAddressOf())
	);


}
