/*
*	@file BackGround.h
*	@brief �w�i�N���X
*/

#include <pch.h>
#include "BackGround.h"


/*
*	@brief �C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  BackGround::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief �R���X�g���N�^
*	@details �w�i�N���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
BackGround::BackGround(CommonResources* resources)
	: m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_time(0.0f)// ����
	, m_constBuffer()// �R���X�^���g�o�b�t�@
	, m_pCommonResources{ resources }// ���ʃ��\�[�X
	, m_pDrawPolygon(DrawPolygon::GetInstance())// �`��N���X
	, m_pCreateShader(CreateShader::GetInstance())// �V�F�[�_�[�쐬�N���X
{


}
/*
*	@brief �f�X�g���N�^
*	@details �w�i�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
BackGround::~BackGround() {/* do nothing */ }
/*
*	@brief ����
*	@details �w�i�N���X�Ŏg���摜��V�F�[�_�[�̐����E�쐬
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void  BackGround::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;// �f�o�C�X���\�[�X�̎擾
	CreateShaders();// �V�F�[�_�[�̍쐬
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("BackGround"));// �e�N�X�`���̎擾
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// �|���S���`��p
}
/*
*	@brief �V�F�[�_�[�쐬
*	@details Create�֐����甲���o�����V�F�[�_�[���쐬���镔��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  BackGround::CreateShaders()
{
	// �f�o�C�X�̎擾
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(device, &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/CRT/VS_CRT.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/CRT/PS_CRT.cso", m_pPixelShader);
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
	// �F�̏�����
	m_color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}
/*
*	@brief �X�V
*	@details �w�i�N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void  BackGround::Update(float elapsedTime)
{
	// ���ԍX�V
	m_time += elapsedTime;
}
/*
*	@brief �`��
*	@details �w�i�N���X�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  BackGround::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_����UV�����i�[����z��
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f), Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),  Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),// ����
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	// �r���[�s���]�u���Ċi�[����
	m_constBuffer.matView = m_view.Transpose();
	// �v���W�F�N�V�����s���]�u���Ċi�[����
	m_constBuffer.matProj = m_proj.Transpose();
	// ���[���h�s���]�u���Ċi�[����
	m_constBuffer.matWorld = m_world.Transpose();
	// �F���i�[����
	m_constBuffer.colors = Vector4(m_color);
	// ���Ԃ��i�[����
	m_constBuffer.time = Vector4(m_time);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);//	�f�v�X�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}