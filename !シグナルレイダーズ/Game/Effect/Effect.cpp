/*
	@file	Particle.cpp
	@brief	�p�[�e�B�N���N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Effect.h"



using namespace DirectX;


Effect::Effect(CommonResources* resources, ParticleType type, DirectX::SimpleMath::Vector3 PlayPos, DirectX::SimpleMath::Matrix world)
	: m_position{ PlayPos }
	, m_type{ type }
	, m_scale{ 3.0f }
	, m_commonResources{ resources }
	, m_world{ world }
	, m_isPlaying{ true }
	, m_anim{ 0 }
	, m_animSpeed{ 30 }
	, m_elapsedTime{ 0.0f }
	, m_frameRows{}
	, m_frameCols{}
	, m_vertices{}
	, m_offSetY{ 0.0f }

{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	const wchar_t* texturePath = L"Resources/Textures/effect.png";// �e�N�X�`���p�X�����ݒ�
	switch (m_type)
	{
		case ParticleType::ENEMY_DEAD:// �G���S�G�t�F�N�g
			texturePath = L"Resources/Textures/effect.png";
			m_frameRows = 4; // �摜�̍s��
			m_frameCols = 5; // �摜�̗�
			m_offSetY = 2.0f;// ������ς���
			m_vertexMinX = -2.0f;
			m_vertexMaxX = 2.0f;
			m_vertexMinY = -2.0f;
			m_vertexMaxY = 2.0f;
			break;
		case ParticleType::ENEMY_HIT:// �G�q�b�g�G�t�F�N�g
			texturePath = L"Resources/Textures/hit.png";
			m_frameRows = 2; // �摜�̍s��
			m_frameCols = 4; // �摜�̗�
			m_offSetY = 2.0f;// ������ς���
			m_vertexMinX = -1.0f;
			m_vertexMaxX = 1.0f;
			m_vertexMinY = -1.0f;
			m_vertexMaxY = 1.0f;
			break;
		case ParticleType::ENEMY_SPAWN:// �G�X�|�[���G�t�F�N�g
			texturePath = L"Resources/Textures/Born.png";
			m_frameRows = 4; // �摜�̍s��
			m_frameCols = 4; // �摜�̗�
			m_offSetY = 2.0f;// ������ς���
			m_vertexMinX = -1.0f;
			m_vertexMaxX = 1.0f;
			m_vertexMinY = -1.0f;
			m_vertexMaxY = 1.0f;
			break;

	}




	// �G�t�F�N�g�̍쐬 
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_BatchEffect->SetReferenceAlpha(1); // 0�ł͂Ȃ�1�ɐݒ肵�Ċ��S�ȍ������O����

	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	// ���ʃX�e�[�g����
	m_States = std::make_unique<CommonStates>(device);

	// �e�N�X�`�����[�h
	CreateWICTextureFromFile(
		device,
		texturePath,
		nullptr,
		m_Texture.GetAddressOf()
	);

	// �A�j���[�V�����Đ���
	m_isPlaying = true;
}
Effect::~Effect() {}

void Effect::Update(float elapsedTime)
{
	m_elapsedTime += elapsedTime * m_animSpeed;
	if (m_elapsedTime >= 1.0f)
	{
		m_anim++;
		m_elapsedTime = 0.0f;
	}
	if (m_anim == m_frameRows * m_frameCols)
	{

		m_isPlaying = false;// �Đ��I��
	}
}

void Effect::Render(ID3D11DeviceContext1* context, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	int currentRow = m_anim / m_frameCols;
	int currentCol = m_anim % m_frameCols;



	// �e�N�X�`�����W�̌v�Z
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMin)) };
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX,m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMin)) };
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX,m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMax)) };

	// �v���~�e�B�u�o�b�`�쐬
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	VertexPositionTexture billboardVertex[4]{};
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = m_vertices[i];

		// �傫����ς���ꍇ�͍ŏ��ɂ�����
		billboardVertex[i].position.x *= m_scale;
		billboardVertex[i].position.y *= m_scale;

		// ������������Ɖ�����
		billboardVertex[i].position.y -= m_offSetY;

	}

	// �r���{�[�h�s����쐬
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;


	// �r���{�[�h���A�t�B���ϊ�
	Matrix worldBillboard = m_world * billboardMatrix;
	worldBillboard *= Matrix::CreateTranslation(m_position);

	// �[�x�X�e���V����Ԃ�ݒ�i�[�x�o�b�t�@��L���ɂ���j

	context->OMSetDepthStencilState(m_States->DepthDefault(), 0);
	context->OMSetBlendState(m_States->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	// �J�����O�͍�����i�����v���j
	context->RSSetState(m_States->CullCounterClockwise());

	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_States->AnisotropicWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	�s�����̂ݕ`�悷��ݒ� 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_BatchEffect->SetReferenceAlpha(0);


	m_BatchEffect->SetWorld(worldBillboard);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_Texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	������������`�� 
	m_Batch->Begin();
	m_Batch->DrawQuad(billboardVertex[0], billboardVertex[1], billboardVertex[2], billboardVertex[3]);
	m_Batch->End();


}

void Effect::Finalize()
{

}
