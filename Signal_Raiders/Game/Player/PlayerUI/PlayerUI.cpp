//--------------------------------------------------------------------------------------
//@file    PlayerUI.cpp
//@brief  �v���C���[UI�N���X
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "PlayerUI.h"

using namespace DirectX;

// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
// �R���X�g���N�^
PlayerUI::PlayerUI()
	: m_pDR(nullptr)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_frameRows(0)
	, m_frameCols(0)
	, m_anim(0)
	, m_texture(nullptr)
	, m_res(nullptr)
	, m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_anchor(KumachiLib::ANCHOR::TOP_LEFT)
	, m_shaderType(ShaderType::OTHER)
	, m_renderRatio(1.0f)
	, m_renderRatioOffset(0.0f)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
}
PlayerUI::~PlayerUI() {  }

// �e�N�X�`����ǂݍ���
void PlayerUI::LoadTexture(const wchar_t* path)
{

	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, m_res.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	m_textures.push_back(m_texture);
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;

}

// �����֐�
void PlayerUI::Create(DX::DeviceResources* pDR
	, const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor)
{
	m_pDR = pDR;
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);

	//	�V�F�[�_�[�̍쐬
	CreateShader();

	//	�摜�̓ǂݍ���
	LoadTexture(path);
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);

}




// �V�F�[�_�[�̍쐬
void PlayerUI::CreateShader()
{
	// �V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/PlayerUI/VS_PlayerUI.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerUI.cso", m_pixelShader);
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/PlayerUI/GS_PlayerUI.cso", m_geometryShader);

	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();

	// HP�p�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerHP.cso", m_hpPixelShader);
	// �~�`�Q�[�W�p�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_CircleGauge.cso", m_circlePixelShader);
	// UV�A�j���[�V�����p�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_Loading.cso", m_animPixelShader);

	// HP�p�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	// HP�ȊO�̃V�F�[�_�[
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = m_geometryShader.Get();
	// HP�p�V�F�[�_�[
	m_hpShaders.vs = m_vertexShader.Get();
	m_hpShaders.ps = m_hpPixelShader.Get();
	m_hpShaders.gs = m_geometryShader.Get();
	// �~�`�Q�[�W�p�V�F�[�_�[
	m_circleShaders.vs = m_vertexShader.Get();
	m_circleShaders.ps = m_circlePixelShader.Get();
	m_circleShaders.gs = m_geometryShader.Get();
	// UV�A�j���[�V�����p�V�F�[�_�[
	m_animShaders.vs = m_vertexShader.Get();
	m_animShaders.ps = m_animPixelShader.Get();
	m_animShaders.gs = m_geometryShader.Get();

}


/// <summary>
/// �`��֐�
/// </summary>
void PlayerUI::Render()
{
	//	���_���
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(
			 SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))//	Position.xy	:�g�k�p�X�P�[��	Position.z	:�A���J�[�^�C�v(0�`8)�̐����Ŏw��
			,SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))	//	Color.xy�@	:�A���J�[���W(�s�N�Z���w��:1280 �~720) Color.zw	:�摜�T�C�Y
			,SimpleMath::Vector2(m_renderRatio - m_renderRatioOffset,0))//	Tex.xy		:x = �Q�[�W�摜�̕`��͈�(0�`1), y = 0
	};

	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	m_constBuffer.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);

	m_constBuffer.renderRatio = m_renderRatio - m_renderRatioOffset;
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constBuffer);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);

	//	�V�F�[�_���Z�b�g����
	if (m_shaderType == ShaderType::HP)
	{
		// �`�揀��
		m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
		m_pDrawPolygon->SetShader(m_hpShaders, nullptr, 0);
	}
	else if (m_shaderType == ShaderType::CIRCLE)
	{
		// �`�揀��
		m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
		m_pDrawPolygon->SetShader(m_circleShaders, nullptr, 0);
	}
	else if (m_shaderType == ShaderType::ANIM)
	{
		// �`�揀��
		m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
		m_pDrawPolygon->SetShader(m_animShaders, nullptr, 0);
	}
	else
	{
		m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);
		m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	}
	//	�|���S����`��
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();

}

void PlayerUI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
