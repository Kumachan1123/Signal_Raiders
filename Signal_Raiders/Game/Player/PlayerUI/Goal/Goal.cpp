/*
*	@file	Goal.cpp
*	@brief	�ڕW��\������N���X
*/
#include <pch.h>
#include "Goal.h"
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Goal::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details Goal�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
Goal::Goal(CommonResources* resources)
	:m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_time(0.0f) // �o�ߎ���
	, m_constBuffer() // �R���X�^���g�o�b�t�@
	, m_pDrawPolygon{ DrawPolygon::GetInstance() } // �|���S���`��N���X
	, m_pCommonResources{ resources } // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pCreateShader{ CreateShader::GetInstance() } // �V�F�[�_�[�쐬�N���X
{

	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);// �F�̏�����
}
/*
*	@brief	�f�X�g���N�^
*	@details Goal�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
Goal::~Goal() {/*do nothing*/ }
/*
*	@brief	�����֐�
*	@details DX::DeviceResources���󂯎��A�K�v�ȃ��\�[�X������������
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void  Goal::Create(DX::DeviceResources* pDR)
{
	// �f�o�C�X���\�[�X���Z�b�g
	m_pDR = pDR;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �e�摜�̓ǂݍ���
	// �w�i�e�N�X�`��
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Crisis"));
	// �u���ׂĂ̓G��|���I�v�̃e�N�X�`��
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("KillAll"));
	// �|���S���`��N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief	�V�F�[�_�[�쐬�֐�
*	@details ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���쐬���A�C���v�b�g���C�A�E�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  Goal::CreateShaders()
{
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/ReadyGo/VS_ReadyGo.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/ReadyGo/PS_ReadyGo.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[���Z�b�g
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[�͎g�p���Ȃ��̂�nullptr���Z�b�g
	m_shaders.gs = nullptr;
}
/*
*	@brief	�X�V�֐�
*	@details ���Ԃ��X�V���A�F�̃A���t�@�l��ω�������
*	@param elapsedTime �O�t���[������̌o�ߎ���
*	@return �Ȃ�
*/
void  Goal::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// ���ԍX�V 
	m_time += elapsedTime;
	// alpha�l��0.7f������������܂��������グ��
	// 0.7f�ȏゾ������A0.9f�܂ŉ�������
	if (m_constBuffer.colors.w < 0.7f)m_constBuffer.colors.w += 0.15f * elapsedTime;
	else m_constBuffer.colors.w = 0.8f + 0.1f * sin(m_time);
	// �F�̍X�V
	m_constBuffer.colors = Vector4(1.0f, 1.0f, 0.0f, m_constBuffer.colors.w);
}
/*
*	@brief	�`��֐�
*	@details �|���S����`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  Goal::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),  Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f),   Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),    Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f),   Vector2(0.0f, 1.0f)), // ����
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬���� 
	// �r���[�s���]�u���ēn��
	m_constBuffer.matView = m_view.Transpose();
	// �v���W�F�N�V�����s���]�u���ēn��
	m_constBuffer.matProj = m_proj.Transpose();
	// ���[���h�s���]�u���ēn��
	m_constBuffer.matWorld = m_world.Transpose();
	// ���Ԃ�n��
	m_constBuffer.time = SimpleMath::Vector4(m_time);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
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
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}