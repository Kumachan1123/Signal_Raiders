/*
*	@file	Result.cpp
*	@brief	���ʃN���X
*/
#include <pch.h>
#include "Result.h"
/*
*	@brief �C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Result::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief �R���X�g���N�^
*	@details ���ʃN���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
Result::Result(CommonResources* resources)
	: m_pCommonResources{ resources }// ���ʃ��\�[�X
	, m_pVertexShader{}// ���_�V�F�[�_�[
	, m_pPixelShader{}// �s�N�Z���V�F�[�_�[
	, m_pInputLayout{}// �C���v�b�g���C�A�E�g
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pCBuffer{}// �R���X�^���g�o�b�t�@
	, m_pTexture{}// �e�N�X�`��
	, m_time{ 0.0f }// ����
	, m_world{}// ���[���h�s��
	, m_view{}// �r���[�s��
	, m_proj{}// �v���W�F�N�V�����s��
	, m_vertex{}// ���_���
	, m_ConstBuffer{}// �R���X�^���g�o�b�t�@(�V�F�[�_�[�ɓn����)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���ʃN���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Result::~Result() {/*do nothing */ }
/*
*	@brief ����
*	@details ���ʃN���X�Ŏg���摜��V�F�[�_�[�̐����E�쐬
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void Result::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �f�o�C�X���\�[�X���擾
	m_pDR = pDR;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �v���C�V�[���ł̌��ʂɉ����ĕω�
	if (GetSceneID() == IScene::SceneID::GAMEOVER)// �Q�[���I�[�o�[
	{
		// �Q�[���I�[�o�[�摜�̓ǂݍ���
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("GameOver"));
		// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�
		m_ConstBuffer.Colors = Vector4(0.15f, 0, 0.5f, 0);
		// ���_����UV���
		VertexPositionTexture vertex[4] =
		{
			//	���_���													UV���
			VertexPositionTexture(Vector3(-0.85f,  0.4f, 0.0f), Vector2(0.0f, 0.0f)),// ����
			VertexPositionTexture(Vector3(0.85f,  0.4f, 0.0f),  Vector2(1.0f, 0.0f)),// �E��
			VertexPositionTexture(Vector3(0.85f, -0.4f, 0.0f),  Vector2(1.0f, 1.0f)),// �E��
			VertexPositionTexture(Vector3(-0.85f, -0.4f, 0.0f), Vector2(0.0f, 1.0f)),// ����
		};
		// ���_�����i�[
		for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];
	}
	else // �Q�[���N���A
	{
		// �N���A�摜�̓ǂݍ���
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("GameClear"));
		// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�
		m_ConstBuffer.Colors = Vector4(0.5f, 0.5f, 0, 0);
		// ���_����UV���
		VertexPositionTexture vertex[4] =
		{
			//	���_���													UV���
			VertexPositionTexture(Vector3(-0.8f,  0.5f, 0.0f), Vector2(0.0f, 0.0f)),// ����
			VertexPositionTexture(Vector3(0.8f,  0.5f, 0.0f),  Vector2(1.0f, 0.0f)),// �E��
			VertexPositionTexture(Vector3(0.8f, -0.5f, 0.0f),  Vector2(1.0f, 1.0f)),// �E��
			VertexPositionTexture(Vector3(-0.8f, -0.5f, 0.0f), Vector2(0.0f, 1.0f)),// ����
		};
		// ���_�����i�[
		for (int i = 0; i < 4; i++)m_vertex[i] = vertex[i];
	}
	// ���_�̍��W�𒲐�
	for (int i = 0; i < 4; i++)m_vertex[i].position.y += 0.25f;
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief �V�F�[�_�[�쐬
*	@details Create�֐����甲���o�����V�F�[�_�[���쐬���镔��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Result::CreateShaders()
{
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[��n��
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[�͎g��Ȃ��̂�nullptr��ݒ�
	m_shaders.gs = nullptr;
}
/*
*	@brief �X�V
*	@details ���ʃN���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Result::Update(float elapsedTime)
{
	// ���Ԃ��J�E���g
	m_time += elapsedTime;
}
/*
*	@brief �`��
*	@details ���ʃN���X�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Result::Render()
{
	using namespace DirectX::SimpleMath;
	// �r���[�s���]�u���Đݒ�
	m_ConstBuffer.matView = m_view.Transpose();
	// �v���W�F�N�V�����s���]�u���Đݒ�
	m_ConstBuffer.matProj = m_proj.Transpose();
	// ���[���h�s���]�u���Đݒ�
	m_ConstBuffer.matWorld = m_world.Transpose();
	// ���Ԃ�ݒ�
	m_ConstBuffer.time = m_time;
	// �p�f�B���O
	m_ConstBuffer.padding = Vector3(0, 0, 0);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_ConstBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(m_vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}