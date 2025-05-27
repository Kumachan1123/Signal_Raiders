/*
*	@file	TitleLogo.cpp
*	@brief	�^�C�g�����S�N���X
*/
#include "pch.h"
#include "TitleLogo.h"
/*
*	@brief �C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  TitleLogo::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief �R���X�g���N�^
*	@details �^�C�g�����S�N���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
TitleLogo::TitleLogo(CommonResources* resources)
	: m_pCommonResources{ resources }// ���ʃ��\�[�X
	, m_pVertexShader{}// ���_�V�F�[�_�[
	, m_pPixelShader{}// �s�N�Z���V�F�[�_�[
	, m_pInputLayout{}// �C���v�b�g���C�A�E�g
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pCBuffer{}// �R���X�^���g�o�b�t�@
	, m_titleTexture{}// �e�N�X�`��
	, m_time{ 0.0f }// ����
	, m_world{}// ���[���h�s��
	, m_view{}// �r���[�s��
	, m_proj{}// �v���W�F�N�V�����s��
	, m_ConstBuffer{}// �R���X�^���g�o�b�t�@�i�V�F�[�_�[�ɓn�����j
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // �V�F�[�_�[�쐬�N���X�̏�����
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief �f�X�g���N�^
*	@details �����ł͉������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleLogo::~TitleLogo() {/*do nothing*/ }
/*
*	@brief �e�N�X�`���ǂݍ���
*	@details �e�N�X�`����ǂݍ���
*	@param path �e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void TitleLogo::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �ꎞ�ۑ��p�e�N�X�`��
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_titleTexture.push_back(texture);// �z��ɓo�^
}
/*
*	@brief �^�C�g�����S�̏�����
*	@details �^�C�g�����S�̏��������s��
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void TitleLogo::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;// �f�o�C�X���\�[�X���Z�b�g
	CreateShaders();// �V�F�[�_�[�̍쐬
	LoadTexture(L"Resources/Textures/Title.png");// �摜�̓ǂݍ��݁@
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// �|���S���̕`��p
}

void TitleLogo::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);// �s�N�Z���V�F�[�_�[�̍쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[(�g��Ȃ��̂�nullptr)
}
/*
*	@brief �X�V
*	@details �^�C�g�����S�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void TitleLogo::Update(float elapsedTime)
{
	m_time += elapsedTime; // ���Ԃ��J�E���g
}
/*
*	@brief �`��
*	@details �^�C�g�����S�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleLogo::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] = // ���_����UV���
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(-0.85f,  0.75f, 0.0f), Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(0.85f,  0.75f, 0.0f),  Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(0.85f, -0.75f, 0.0f),  Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(-0.85f, -0.75f, 0.0f), Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	m_ConstBuffer.matView = m_view.Transpose();// �r���[�ݒ�
	m_ConstBuffer.matProj = m_proj.Transpose();// �v���W�F�N�V�����ݒ�
	m_ConstBuffer.matWorld = m_world.Transpose();// ���[���h�ݒ�
	m_ConstBuffer.Colors = Vector4(0.25f, 0.75f, 0.75f, 0);// �O���f�[�V�����G�t�F�N�g�̐F�ݒ� 
	m_ConstBuffer.time = Vector4(m_time);// ���Ԑݒ�
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);// �󂯓n���p�o�b�t�@�̓��e�X�V
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_titleTexture);// �`��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->DrawTexture(vertex);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�̓o�^���������Ă���
}