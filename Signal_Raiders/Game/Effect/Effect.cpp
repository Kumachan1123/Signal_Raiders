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
// ---------------------------------------------------------
// �C���v�b�g���C�A�E�g
// ---------------------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Effect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// ---------------------------------------------------------
// �R���X�g���N�^
// ---------------------------------------------------------
Effect::Effect(CommonResources* resources, EffectType type, DirectX::SimpleMath::Vector3 playPos, float scale, DirectX::SimpleMath::Matrix world)
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
	case EffectType::ENEMY_DEAD:// �G���S�G�t�F�N�g
		LoadTexture(L"Resources/Textures/effect.png");
		m_frameRows = 4; // �摜�̍s��
		m_frameCols = 5; // �摜�̗�
		m_vertexMinX = -1.0f;
		m_vertexMaxX = 1.0f;
		m_vertexMinY = -1.0f;
		m_vertexMaxY = 1.0f;
		break;
	case EffectType::ENEMY_HIT:// �G�q�b�g�G�t�F�N�g
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
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_vertexShader);// ���_�V�F�[�_�[
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pixelShader);	// �s�N�Z���V�F�[�_�[
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃZ�b�g
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
	// �A�j���[�V�������Đ����ɂ���
	m_isPlaying = true;
}
//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Effect::~Effect() {}
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

// ---------------------------------------------------------
// �X�V����
// ---------------------------------------------------------
void Effect::Update(float elapsedTime)
{
	m_elapsedTime += elapsedTime * m_animSpeed;// �o�ߎ��Ԃ����Z
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

// ---------------------------------------------------------
// �`�揈��
// ---------------------------------------------------------
void Effect::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ���_���
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(0, 1)) };// ����
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(1, 1)) };// �E��
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX,m_vertexMinY, 0), SimpleMath::Vector2(1, 0)) };// �E��
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX,m_vertexMinY, 0), SimpleMath::Vector2(0, 0)) }; // ����
	VertexPositionTexture billboardVertex[4]{};// �r���{�[�h�p�̒��_���
	for (int i = 0; i < 4; i++)
	{
		// ���_�����R�s�[
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
	Matrix worldBillboard = m_world * billboardMatrix;// ���[���h�s��
	worldBillboard *= Matrix::CreateTranslation(m_position);// �ʒu
	// �萔�o�b�t�@�̓��e���X�V
	m_constBuffer.matWorld = worldBillboard.Transpose();// ���[���h�s��
	m_constBuffer.matView = view.Transpose();// �r���[�s��
	m_constBuffer.matProj = proj.Transpose();// �v���W�F�N�V�����s��
	m_constBuffer.count = Vector4((float)(m_anim));// �A�j���[�V�����J�E���g
	m_constBuffer.height = Vector4((float)(m_frameRows));// ����
	m_constBuffer.width = Vector4((float)(m_frameCols));// ��
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �e�N�X�`���̃T���v���[�X�e�[�g(���j�A�A���b�v)
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g(���Z)
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g(�J�����O�Ȃ�)
		DrawPolygon::DepthStencilStates::DEPTH_READ);// �[�x�X�e���V���X�e�[�g(�ǂݎ��̂�)
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(billboardVertex);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

// ---------------------------------------------------------
// �I������
// ---------------------------------------------------------
void Effect::Finalize()
{
	// �V�F�[�_�[�̉��
	m_vertexShader.Reset();
	m_pixelShader.Reset();
	// �C���v�b�g���C�A�E�g�̉��
	m_pInputLayout.Reset();
	// �R���X�^���g�o�b�t�@�̉��
	m_cBuffer.Reset();
	// �e�N�X�`���̉��
	m_textures.clear();
}
