/*
*	@file	StageSelect.cpp
*	@brief	�X�e�[�W�I���N���X
*/
#include "pch.h"
#include "StageSelect.h"
/*
*	@brief �C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  StageSelect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
StageSelect::StageSelect(CommonResources* resources)
	: m_pCommonResources{ resources }// ���ʃ��\�[�X
	, m_pVertexShader{}// ���_�V�F�[�_�[
	, m_pPixelShader{}// �s�N�Z���V�F�[�_�[
	, m_pInputLayout{}// ���̓��C�A�E�g
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pCBuffer{}// �R���X�^���g�o�b�t�@
	, m_pTexture{}// �e�N�X�`��
	, m_time{ 0.0f }// ����
	, m_world{}// ���[���h�s��
	, m_view{}// �r���[�s��
	, m_proj{}// �v���W�F�N�V�����s��
	, m_vertex{}// ���_���
	, m_ConstBuffer{}// �R���X�^���g�o�b�t�@
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(),// �V�F�[�_�[�쐬�N���X�̏�����
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief �f�X�g���N�^
*	@details �X�e�[�W�Z���N�g�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
StageSelect::~StageSelect() {/*do nothing*/ }
/*
*	@brief �e�N�X�`���ǂݍ���
*	@details �e�N�X�`����ǂݍ���
*	@param path �e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void StageSelect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �ꎞ�ۑ��p�e�N�X�`��
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_pTexture.push_back(texture);// �z��ɓo�^
}
/*
*	@brief ������
*	@details �X�e�[�W�Z���N�g�N���X�̏�����
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void StageSelect::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pDR = pDR;// �f�o�C�X���\�[�X���Z�b�g
	CreateShaders();// �V�F�[�_�[�̍쐬
	LoadTexture(L"Resources/Textures/StageSelectTex.png");// �摜�̓ǂݍ���
	m_ConstBuffer.color = SimpleMath::Vector4(.25, .25, .25, 0);// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�
	VertexPositionTexture vertex[4] =// ���_����UV���
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(0.85f,0.45f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.85f,0.45f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.85f, 1.05f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.85f, 1.05f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
	};
	for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];// ���_�����Z�b�g
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// �|���S���`��p
}
/*
*	@brief �V�F�[�_�[���쐬
*	@details �V�F�[�_�[���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelect::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);// �s�N�Z���V�F�[�_�[�쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_-
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[(�g��Ȃ��̂�nullptr)
}
/*
*	@brief �X�V
*	@details �X�e�[�W�Z���N�g�N���X�̍X�V
*	@param elapsedTime �X�V����
*	@return �Ȃ�
*/
void StageSelect::Update(float elapsedTime)
{
	m_time += elapsedTime; // ���Ԃ��J�E���g
}
/*
*	@brief �`��
*	@details �X�e�[�W�Z���N�g�N���X�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelect::Render()
{
	using namespace DirectX::SimpleMath;
	m_ConstBuffer.matView = m_view.Transpose();// �r���[�ݒ�
	m_ConstBuffer.matProj = m_proj.Transpose();// �v���W�F�N�V�����ݒ�
	m_ConstBuffer.matWorld = m_world.Transpose();// ���[���h�ݒ�
	m_ConstBuffer.time = Vector4(m_time);// ���Ԑݒ�
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);//	�V�F�[�_���Z�b�g����
	m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);//�[�x�X�e���V���X�e�[�g
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);// �e�N�X�`���`��J�n
	m_pDrawPolygon->DrawTexture(m_vertex);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�[�̓o�^���������Ă���
}