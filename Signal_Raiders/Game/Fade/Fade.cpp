/*
*	@file	Fade.cpp
*	@brief	�t�F�[�h�N���X
*/
#include "pch.h"
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
	, m_time{ FADE_INIT }// ����
	, m_fadeState{ FadeState::None }// �t�F�[�h���
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(),
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// �V�F�[�_�[�쐬�N���X�̏�����
}
/*
*	@brief �f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Fade::~Fade() {/*do nothing*/ }
/*
*	@brief �e�N�X�`���̓ǂݍ���
*	@param path	�e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void Fade::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �ꎞ�ۑ��p�ϐ�
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_pTexture.push_back(texture);// �z��ɒǉ�
}
/*
*	@brief ����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Fade::Initialize()
{
	m_pDR = m_pCommonResources->GetDeviceResources();	// �f�o�C�X���\�[�X�̎擾
	CreateShaders();// �V�F�[�_�[�̍쐬
	LoadTexture(L"Resources/Textures/fade.png");	// �摜�̓ǂݍ���
	m_pDrawPolygon->InitializePositionTexture(m_pDR);	// �|���S���`��p
}
/*
*	@brief �V�F�[�_�[�̍쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Fade::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Fade/VS_Fade.cso", m_pVertexShader);// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Fade/PS_Fade.cso", m_pPixelShader);// �s�N�Z���V�F�[�_�[�쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[(�g��Ȃ��̂�nullptr)
}

/*
*	@brief �X�V
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::Update(float elapsedTime)
{
	FadeIn(elapsedTime);// �t�F�[�h�C��

	FadeOut(elapsedTime);// �t�F�[�h�A�E�g

}
/*
*	@brief �t�F�[�h�C��
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::FadeIn(float elapsedTime)
{
	if (m_fadeState == FadeState::FadeIn)// �t�F�[�h�C�����Ȃ�
	{
		m_time += elapsedTime * FADE_SPEED;// ���Ԃ����Z
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);// �t�F�[�h�𐧌�
		if (m_time >= FADE_MAX)// �t�F�[�h���ő�l�ɂȂ�����
		{
			m_fadeState = FadeState::FadeInEnd;// �t�F�[�h�C���I��
		}
	}
}
/*
*	@brief �t�F�[�h�A�E�g
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::FadeOut(float elapsedTime)
{
	if (m_fadeState == FadeState::FadeOut)// �t�F�[�h�A�E�g���Ȃ�
	{
		m_time -= elapsedTime * FADE_SPEED;	// ���Ԃ����Z
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);	// �t�F�[�h�𐧌�
		if (m_time <= FADE_MIN)// �t�F�[�h���ŏ��l�ɂȂ�����
		{
			m_fadeState = FadeState::FadeOutEnd;// �t�F�[�h�A�E�g�I��
		}
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
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0, -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	ConstBuffer cbuff;	// �V�F�[�_�[�ɓn���o�b�t�@���쐬
	cbuff.smoothness = FADE_SMOOTHNESS;// �t�F�[�h�̊��炩��
	cbuff.fadeAmount = m_time;	// �t�F�[�h�̐i�s�x
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &cbuff);	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->DrawSetting(	// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// �[�x�X�e���V���X�e�[�g
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);	//	�`�揀��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);	//	�V�F�[�_���Z�b�g����
	m_pDrawPolygon->DrawTexture(vertex);	//	�|���S����`��
	m_pDrawPolygon->ReleaseShader();	//	�V�F�[�_�̓o�^���������Ă���

}