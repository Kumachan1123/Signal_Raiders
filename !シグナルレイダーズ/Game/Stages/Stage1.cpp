#include "pch.h"

#include "Game//Stages/Stage1.h"


Stage1::Stage1()
{

}
Stage1::~Stage1() {}

void Stage1::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	�G�t�F�N�g�̍쐬 
	m_BatchEffect = std::make_unique<NormalMapEffect>(device);
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
	// �m�[�}���}�b�v�̃��[�h
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tile_normal.png",
		nullptr,
		m_NormalMap.GetAddressOf()
	);
	m_BatchEffect->SetNormalTexture(*m_NormalMap.GetAddressOf());

}
void Stage1::Update(float elapsedTime)
{

}
void Stage1::Render(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	//	�v���~�e�B�u�o�b�`�̍쐬 
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	���_���i�|���S���̒��_�j 
	VertexPositionTexture vertex[4] =
	{//												���W					UV���W�i�ӂ���0�`1�̊ԂŎw��B�������ꍇ�͌J��Ԃ�
		VertexPositionTexture(SimpleMath::Vector3(50.0f, 0.00f, 50.0f),	SimpleMath::Vector2(.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-50.0f, 0.0f, 50.0f),	SimpleMath::Vector2(50.0f, .0f)),
		VertexPositionTexture(SimpleMath::Vector3(-50.0f, 0.f, -50.0f),	SimpleMath::Vector2(50.0f,50.0f)),
		VertexPositionTexture(SimpleMath::Vector3(50.0f,0.f, -50.0f),	SimpleMath::Vector2(0.0f, 50.0f)),
	};

	//	�e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j 
	ID3D11SamplerState* samplers[1] = { m_States->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ��� 
	context->OMSetDepthStencilState(m_States->DepthDefault(), 0);

	//	�J�����O�͍�����i�����v���j 
	context->RSSetState(m_States->CullCounterClockwise());

	//	�s�����̂ݕ`�悷��ݒ� 

	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_Texture.Get());
	// �m�[�}���}�b�v�̃Z�b�g
	context->PSSetShaderResources(1, 1, m_NormalMap.GetAddressOf());

	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	������������`�� 
	m_Batch->Begin();
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}
