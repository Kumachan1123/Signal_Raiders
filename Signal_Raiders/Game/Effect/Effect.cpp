/*
	@file	Effect.cpp
	@brief	�G�t�F�N�g�N���X
*/
#include "pch.h"
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Effect.h"



using namespace DirectX;
using namespace DirectX::SimpleMath;

const std::vector<D3D11_INPUT_ELEMENT_DESC>  Effect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


Effect::Effect(CommonResources* resources, ParticleType type, DirectX::SimpleMath::Vector3 playPos, float scale, DirectX::SimpleMath::Matrix world)
	: m_position{ playPos }
	, m_type{ type }
	, m_scale{ scale }
	, m_commonResources{ resources }
	, m_world{ world }
	, m_isPlaying{ true }
	, m_anim{ 0 }
	, m_animSpeed{ 30.0f }
	, m_elapsedTime{ 0.0f }
	, m_frameRows{}
	, m_frameCols{}
	, m_vertices{}
	, m_offSetY{ 0.0f }
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }

{
	switch (m_type)// �G�t�F�N�g�̎�ނɂ���ĉ摜��ǂݍ���
	{
	case ParticleType::ENEMY_DEAD:// �G���S�G�t�F�N�g
		LoadTexture(L"Resources/Textures/effect.png");
		m_frameRows = 4; // �摜�̍s��
		m_frameCols = 5; // �摜�̗�
		m_vertexMinX = -1.0f;
		m_vertexMaxX = 1.0f;
		m_vertexMinY = -1.0f;
		m_vertexMaxY = 1.0f;
		break;
	case ParticleType::ENEMY_HIT:// �G�q�b�g�G�t�F�N�g
		LoadTexture(L"Resources/Textures/hit.png");
		m_frameRows = 2; // �摜�̍s��
		m_frameCols = 4; // �摜�̗�
		m_vertexMinX = -1.0f;
		m_vertexMaxX = 1.0f;
		m_vertexMinY = -1.0f;
		m_vertexMaxY = 1.0f;
		break;

	}
	// �|���S���`��p�N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃZ�b�g
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
	// �A�j���[�V�����Đ���
	m_isPlaying = true;
}

//---------------------------------------------------------
// �摜��ǂݍ���
//---------------------------------------------------------
void Effect::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	m_textures.push_back(texture);
}
Effect::~Effect() {}

void Effect::Update(float elapsedTime)
{
	m_elapsedTime += elapsedTime * m_animSpeed;
	if (m_elapsedTime >= 1.0f)// 1�b�o��
	{
		m_anim++;// �A�j���̃R�}��E�߂�
		m_elapsedTime = 0.0f;// �^�C�}�[�����Z�b�g
	}
	if (m_anim == m_frameRows * m_frameCols)// �A�j���[�V�����I��
	{
		m_isPlaying = false;// �Đ��I��
	}
}

void Effect::Render(ID3D11DeviceContext1* context, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ���_���
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(0, 1)) };
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(1, 1)) };
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX,m_vertexMinY, 0), SimpleMath::Vector2(1, 0)) };
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX,m_vertexMinY, 0), SimpleMath::Vector2(0, 0)) };
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
	m_constBuffer.matWorld = worldBillboard.Transpose();
	m_constBuffer.matView = view.Transpose();
	m_constBuffer.matProj = proj.Transpose();
	m_constBuffer.count = Vector4(m_anim);
	m_constBuffer.height = Vector4(m_frameRows);
	m_constBuffer.width = Vector4(m_frameCols);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::ANISOTROPIC_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(billboardVertex);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

void Effect::Finalize()
{

}
