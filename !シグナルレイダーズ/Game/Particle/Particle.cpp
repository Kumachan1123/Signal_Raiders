#include "pch.h"
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Particle.h"



using namespace DirectX;


Particle::Particle(CommonResources* resources, DirectX::SimpleMath::Vector3 PlayPos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Matrix world)
	:m_position{ PlayPos }
	, m_scale{ 1.0f }
	, m_commonResources{ resources }
	, m_Billboard{ DirectX::SimpleMath::Matrix::Identity }
	, m_rotation{ rot }
	, m_world{ world }
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	anim = 0;
	m_animSpeed = 25; // �t���[���؂�ւ����x
	m_elapsedTime = 0.0f;
	m_frameRows = 4; // �摜�̍s��
	m_frameCols = 5; // �摜�̗�
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
		L"Resources/Textures/effect.png",
		nullptr,
		m_Texture.GetAddressOf()
	);

	// �A�j���[�V�����Đ���
	m_isPlaying = true;
}
Particle::~Particle() {}

void Particle::Update(float elapsedTime)
{
	m_elapsedTime += elapsedTime * m_animSpeed;
	if (m_elapsedTime >= 1.0f)
	{
		anim++;
		m_elapsedTime = 0.0f;
	}
	if (anim >= m_frameRows * m_frameCols)
	{
		anim = 0;
		//	m_isPlaying = false;// �Đ��I��
	}
}

void Particle::Render(ID3D11DeviceContext1* context, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	int currentRow = anim / m_frameCols;
	int currentCol = anim % m_frameCols;

	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// �v���~�e�B�u�o�b�`�쐬
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	VertexPositionTexture Vertices[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(-1, 1,0), 	 SimpleMath::Vector2(uMax, vMin)),
		VertexPositionTexture(SimpleMath::Vector3(1 , 1,0), 	 SimpleMath::Vector2(uMax, vMax)),
		VertexPositionTexture(SimpleMath::Vector3(1 , -1,0),	 SimpleMath::Vector2(uMin, vMax)),
		VertexPositionTexture(SimpleMath::Vector3(-1, -1,0),	 SimpleMath::Vector2(uMin, vMin)),
	};
	VertexPositionTexture billboardVertex[4];
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = Vertices[i];

		// �傫����ς���ꍇ�͍ŏ��ɂ�����
		billboardVertex[i].position.x *= m_scale;
		billboardVertex[i].position.y *= m_scale;

		// �ݒ肳�ꂽ���W�𑫂�����
		billboardVertex[i].position.x += m_position.x;
		billboardVertex[i].position.y += m_position.y;
		billboardVertex[i].position.z += m_position.z;

	}
	//	�e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j 
	ID3D11SamplerState* samplers[1] = { m_States->AnisotropicWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ��� 
	context->OMSetDepthStencilState(m_States->DepthNone(), 0);
	context->OMSetBlendState(m_States->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	//	�J�����O�͍�����i�����v���j 
	context->RSSetState(m_States->CullNone());

	//	�s�����̂ݕ`�悷��ݒ� 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);




	// �r���{�[�h�s����쐬
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	billboardMatrix._44 = 1.0f;


	// �r���{�[�h���A�t�B���ϊ�
	Matrix worldBillboard{};
	worldBillboard = billboardMatrix;
	worldBillboard *= Matrix::CreateTranslation(m_position);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-m_rotation.y * 2.0f));	// �B

	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(m_rotation.y * 2.0f));	// �B



	m_BatchEffect->SetWorld(m_world);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_Texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	������������`�� 
	m_Batch->Begin();
	m_Batch->DrawQuad(billboardVertex[0], billboardVertex[1], billboardVertex[2], billboardVertex[3]);
	m_Batch->End();

	auto debugString = m_commonResources->GetDebugString();
	/*debugString->AddString("posX:%f", m_position.x);
	debugString->AddString("posY:%f", m_position.y);
	debugString->AddString("posZ:%f", m_position.z);*/
}

void Particle::Finalize()
{

}

void Particle::CreateBillboard(DirectX::SimpleMath::Vector3 target,
							   DirectX::SimpleMath::Vector3 eye,
							   DirectX::SimpleMath::Vector3 up)
{
	using namespace DirectX::SimpleMath;
	Vector3 billboardTranslationValue{ 0.0f, 0.0f, 0.0f };

	m_Billboard =
		SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;


	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;
	m_Billboard = rot * m_Billboard * Matrix::CreateTranslation(billboardTranslationValue);

}