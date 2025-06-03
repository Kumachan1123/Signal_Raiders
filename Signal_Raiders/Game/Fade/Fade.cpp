/*
*	@file	Fade.cpp
*	@brief	�t�F�[�h�N���X
*/
#include <pch.h>
#include "Fade.h"
/*
*	@brief �t�F�[�h�ŏ��l
*/
const float Fade::FADE_MIN = -1.01f;
/*
*	@brief �t�F�[�h�ő�l
*/
const float Fade::FADE_MAX = 1.01f;
/*
*	@brief �t�F�[�h���x
*/
const float Fade::FADE_SPEED = 1.5f;
/*
*	@brief �t�F�[�h�����l
*/
const float Fade::FADE_INIT = -0.75f;
/*
*	@brief �t�F�[�h�̊��炩��
*/
const float Fade::FADE_SMOOTHNESS = 0.05f;

/*
*	@brief �C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief �R���X�g���N�^
*	@param commonResources	���ʃ��\�[�X
*	@return	�Ȃ�
*/
Fade::Fade(CommonResources* commonResources)
	: m_pDR{ }// �f�o�C�X���\�[�X
	, m_pCBuffer{}// �萔�o�b�t�@
	, m_pInputLayout{}// ���̓��C�A�E�g
	, m_pCommonResources{ commonResources }// ���ʃ��\�[�X
	, m_pTexture{}// �e�N�X�`��
	, m_pVertexShader{}// ���_�V�F�[�_�[
	, m_pPixelShader{}// �s�N�Z���V�F�[�_�[
	, m_world{}// ���[���h�s��
	, m_view{}// �r���[�s��
	, m_proj{}// �v���W�F�N�V�����s��
	, m_fadeTime{ FADE_INIT }// �t�F�[�h����
	, m_fadeState{ FadeState::None }// �t�F�[�h���
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
}
/*
*	@brief �f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Fade::~Fade() {/*do nothing*/ }
/*
*	@brief ����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Fade::Initialize()
{
	// �f�o�C�X�̎擾
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(device, &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �f�o�C�X���\�[�X�̎擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �e�N�X�`���̎擾
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Fade"));
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief �V�F�[�_�[�̍쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Fade::CreateShaders()
{
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Fade/VS_Fade.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Fade/PS_Fade.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[��n��
	m_shaders.ps = m_pPixelShader.Get();
	// �V�F�[�_�[�̍\���̂ɃW�I���g���V�F�[�_�[��nullptr�ɂ���(�g��Ȃ��̂�)
	m_shaders.gs = nullptr;
}
/*
*	@brief �X�V
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::Update(float elapsedTime)
{
	// �t�F�[�h�C��
	FadeIn(elapsedTime);
	// �t�F�[�h�A�E�g
	FadeOut(elapsedTime);
}
/*
*	@brief �t�F�[�h�C��
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::FadeIn(float elapsedTime)
{
	// �t�F�[�h�C�����Ȃ�
	if (m_fadeState == FadeState::FadeIn)
	{
		// ���Ԃ��v�Z
		m_fadeTime += elapsedTime * FADE_SPEED;
		// �t�F�[�h�𐧌�
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// �t�F�[�h���ő�l�ɂȂ�����t�F�[�h�C���I��
		if (m_fadeTime >= FADE_MAX)m_fadeState = FadeState::FadeInEnd;
	}
}
/*
*	@brief �t�F�[�h�A�E�g
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::FadeOut(float elapsedTime)
{
	// �t�F�[�h�A�E�g���Ȃ�
	if (m_fadeState == FadeState::FadeOut)
	{
		// ���Ԃ��v�Z
		m_fadeTime -= elapsedTime * FADE_SPEED;
		// �t�F�[�h�𐧌�
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// �t�F�[�h���ŏ��l�ɂȂ�����t�F�[�h�A�E�g�I��
		if (m_fadeTime <= FADE_MIN)m_fadeState = FadeState::FadeOutEnd;
	}
}
/*
*	@brief �`��
*	@param �Ȃ�
*/
void Fade::Render()
{
	using namespace DirectX;
	//	���_���
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),	// ����
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),		// �E��
		VertexPositionTexture(SimpleMath::Vector3(1.0, -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),		// �E��
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),		// ����
	};
	// �V�F�[�_�[�ɓn���o�b�t�@���쐬
	ConstBuffer cbuff;
	// �t�F�[�h�̊��炩����ݒ�
	cbuff.smoothness = FADE_SMOOTHNESS;
	// �t�F�[�h�̐i�s�x��ݒ�
	cbuff.fadeAmount = m_fadeTime;
	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &cbuff);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_�������l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}