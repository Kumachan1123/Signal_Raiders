/*
*	@file PlayerUI.cpp
*	@brief �v���C���[UI�N���X
*/
#include <pch.h>
#include "PlayerUI.h"
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details PlayerUI�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayerUI::PlayerUI(CommonResources* pCommonResources)
	: m_pDR{ nullptr }// �f�o�C�X���\�[�X
	, m_pCommonResources{ pCommonResources }// ���ʃ��\�[�X�ւ̃|�C���^
	, m_windowWidth{ 0 }// �E�B���h�E�̕�
	, m_windowHeight{ 0 }// �E�B���h�E�̍���
	, m_textureWidth{ 0 }// �e�N�X�`���̕�
	, m_textureHeight{ 0 }// �e�N�X�`���̍���
	, m_frameRows(0)// �t���[���̍s��
	, m_frameCols(0)// �t���[���̗�
	, m_anim(0)// �t���[����
	, m_pTexture(nullptr)// �e�N�X�`���n���h��
	, m_pTextureResource(nullptr)// �e�N�X�`�����\�[�X�n���h��
	, m_scale{ DirectX::SimpleMath::Vector2::One }// �X�P�[��
	, m_baseScale{ DirectX::SimpleMath::Vector2::One }// ��X�P�[��
	, m_position{ DirectX::SimpleMath::Vector2::Zero }// �ʒu
	, m_anchor(KumachiLib::ANCHOR::TOP_LEFT)// �A���J�[
	, m_shaderType(ShaderType::OTHER)// �V�F�[�_�[�̎��
	, m_renderRatio(1.0f)// �����_�����O�䗦
	, m_renderRatioOffset(0.0f)// �����_�����O�䗦�̃I�t�Z�b�g
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayerUI::~PlayerUI() {/*do nothing*/ }

/*
*	@brief	�e�N�X�`���̓ǂݍ���
*	@details �e�N�X�`���̓ǂݍ���
*	@param key �e�N�X�`���̃L�[
*	@return �Ȃ�
*/
void PlayerUI::LoadTexture(std::string key)
{
	// �e�N�X�`���}�l�[�W���[����e�N�X�`�����擾
	m_pTexture = m_pCommonResources->GetTextureManager()->GetTexture(key);
	// �T�C�Y�擾�̂��߂̏���
	// �ꎞ�ۑ��p���\�[�X�n���h��
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	// �e�N�X�`�����烊�\�[�X���擾
	m_pTexture->GetResource(resource.GetAddressOf());
	// �ꎞ�ۑ��p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	// ���\�[�X���e�N�X�`���ɕϊ�
	DX::ThrowIfFailed(resource.As(&texture2D));
	// �e�N�X�`���̏����擾
	// �e�N�X�`���̏����i�[����\����
	D3D11_TEXTURE2D_DESC desc;
	// �e�N�X�`���̏����擾
	texture2D->GetDesc(&desc);
	// �e�N�X�`����z��ɒǉ�
	m_pTextures.push_back(m_pTexture.Get());
	// �e�N�X�`���̕����擾
	m_textureWidth = desc.Width;
	// �e�N�X�`���̍������擾
	m_textureHeight = desc.Height;
}

/*
*	@brief	UI�̐���
*	@details UI�̐���
*	@param pDR �f�o�C�X���\�[�X
*	@param key �e�N�X�`���̃L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void PlayerUI::Create(DX::DeviceResources* pDR
	, const std::string& key
	, const DirectX::SimpleMath::Vector2& position
	, const DirectX::SimpleMath::Vector2& scale
	, KumachiLib::ANCHOR anchor)
{
	// �f�o�C�X���\�[�X
	m_pDR = pDR;
	// �ʒu
	m_position = position;
	// �X�P�[��
	m_baseScale = m_scale = scale;
	// �A���J�[
	m_anchor = anchor;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �e�N�X�`����ǂݍ���
	LoadTexture(key);
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief	�V�F�[�_�[�̍쐬
*	@details �e��V�F�[�_�[���쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerUI::CreateShaders()
{
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/PlayerUI/VS_PlayerUI.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerUI.cso", m_pPixelShader);
	// �W�I���g���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/PlayerUI/GS_PlayerUI.cso", m_pGeometryShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// HP�p�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_PlayerHP.cso", m_pHPPixelShader);
	// �~�`�Q�[�W�p�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_CircleGauge.cso", m_pCirclePixelShader);
	// UV�A�j���[�V�����p�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/PlayerUI/PS_Loading.cso", m_pAnimPixelShader);
	// HP�p�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	// HP�ȊO�̃V�F�[�_�[
	// ���_�V�F�[�_�[���Z�b�g
	m_shaders.vs = m_pVertexShader.Get();
	// �s�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[���Z�b�g
	m_shaders.gs = m_pGeometryShader.Get();
	// HP�p�V�F�[�_�[
	// ���_�V�F�[�_�[���Z�b�g
	m_hpShaders.vs = m_pVertexShader.Get();
	// �s�N�Z���V�F�[�_�[���Z�b�g
	m_hpShaders.ps = m_pHPPixelShader.Get();
	// �W�I���g���V�F�[�_�[���Z�b�g
	m_hpShaders.gs = m_pGeometryShader.Get();
	// �~�`�Q�[�W�p�V�F�[�_�[
	// ���_�V�F�[�_�[���Z�b�g
	m_circleShaders.vs = m_pVertexShader.Get();
	// �s�N�Z���V�F�[�_�[���Z�b�g
	m_circleShaders.ps = m_pCirclePixelShader.Get();
	// �W�I���g���V�F�[�_�[���Z�b�g
	m_circleShaders.gs = m_pGeometryShader.Get();
	// UV�A�j���[�V�����p�V�F�[�_�[
	// ���_�V�F�[�_�[���Z�b�g
	m_animShaders.vs = m_pVertexShader.Get();
	// �s�N�Z���V�F�[�_�[���Z�b�g
	m_animShaders.ps = m_pAnimPixelShader.Get();
	// �W�I���g���V�F�[�_�[���Z�b�g
	m_animShaders.gs = m_pGeometryShader.Get();
}
/*
*	@brief	�`��֐�
*	@details �|���S����`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerUI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_���
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(
			  Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))//	Position.xy	:�g�k�p�X�P�[��	Position.z	:�A���J�[�^�C�v(0�`8)�̐����Ŏw��
			, Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))	//	Color.xy�@	:�A���J�[���W(�s�N�Z���w��:1280 �~720) Color.zw	:�摜�T�C�Y
			, Vector2(m_renderRatio - m_renderRatioOffset,0))//	Tex.xy		:x = �Q�[�W�摜�̕`��͈�(0�`1), y = 0
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	// �E�B���h�E�T�C�Y
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	// �����_�����O�䗦
	m_constBuffer.renderRatio = m_renderRatio - m_renderRatioOffset;
	// �󂯓n���p�o�b�t�@�̓��e�X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	// �`��J�n
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	//	�V�F�[�_���Z�b�g����
	if (m_shaderType == ShaderType::HP)// HP�p�̃V�F�[�_�[�̏ꍇ
	{
		// HP�p�̃V�F�[�_�[���Z�b�g
		m_pDrawPolygon->SetShader(m_hpShaders, nullptr, 0);
	}
	else if (m_shaderType == ShaderType::CIRCLE)// �~�`�Q�[�W�p�̃V�F�[�_�[�̏ꍇ 
	{
		// �~�`�Q�[�W�p�̃V�F�[�_�[���Z�b�g
		m_pDrawPolygon->SetShader(m_circleShaders, nullptr, 0);
	}
	else if (m_shaderType == ShaderType::ANIM)// UV�A�j���[�V�����p�̃V�F�[�_�[�̏ꍇ
	{
		// UV�A�j���[�V�����p�̃V�F�[�_�[���Z�b�g
		m_pDrawPolygon->SetShader(m_animShaders, nullptr, 0);
	}
	else// ���̑��̃V�F�[�_�[�̏ꍇ
	{
		// ���ʂ̃V�F�[�_�[���Z�b�g
		m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	}
	// �|���S����`��
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief	�E�B���h�E�̃T�C�Y��ݒ�
*	@details �E�B���h�E�̃T�C�Y��ݒ�
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void PlayerUI::SetWindowSize(const int& width, const int& height)
{
	// �E�B���h�E�̕�
	m_windowWidth = width;
	// �E�B���h�E�̍���
	m_windowHeight = height;
}