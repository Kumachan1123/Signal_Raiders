/*
*	@file Crisis.cpp
*	@brief	�댯�M���N���X�iHP��20�ȉ��ŕ\�������j
*/
#include <pch.h>
#include "Crisis.h"
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Crisis::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details Crisis�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
Crisis::Crisis(CommonResources* resources)
	: m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_time(0.0f)// �o�ߎ���
	, m_constBuffer()// �R���X�^���g�o�b�t�@
	, m_pCommonResources{ resources }// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
	using namespace DirectX::SimpleMath;

	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(),
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);// �V�F�[�_�[�쐬�N���X�̏�����
	m_constBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 0.0f);// �F�̏�����
}
/*
*	@brief	�f�X�g���N�^
*	@details Crisis�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
Crisis::~Crisis() {/*do nothing*/ }
/*
*	@brief	�e�N�X�`�����\�[�X�ǂݍ���
*	@details �w�肳�ꂽ�p�X����e�N�X�`����ǂݍ��݁Am_pTexture�ɒǉ�����
*	@param path �ǂݍ��ރe�N�X�`���̃p�X
*	@return �Ȃ�
*/
void  Crisis::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �ꎞ�ۑ��p�e�N�X�`���n���h��
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// �w�肳�ꂽ�摜��ǂݍ���
	m_pTexture.push_back(texture);// �z��ɒǉ�
}
/*
*	@brief	�����֐�
*	@details DX::DeviceResources���󂯎��A�K�v�ȃ��\�[�X������������
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void  Crisis::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;// �f�o�C�X���\�[�X���Z�b�g
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// �|���S���`��p
	CreateShaders();// �V�F�[�_�[�̍쐬
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Crisis"));
}
/*
*	@brief	�V�F�[�_�[�쐬�֐�
*	@details ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���쐬���A�C���v�b�g���C�A�E�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  Crisis::CreateShaders()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/CRT/VS_CRT.cso", m_pVertexShader);// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/CRT/PS_CRT.cso", m_pPixelShader);// �s�N�Z���V�F�[�_�[�̍쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[���Z�b�g
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[�͎g�p���Ȃ�
}
/*
*	@brief	�X�V�֐�
*	@details ���Ԃ��X�V���A�F�̃A���t�@�l��ω�������
*	@param elapsedTime �O�t���[������̌o�ߎ���
*	@return �Ȃ�
*/
void  Crisis::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_time += elapsedTime;// ���ԍX�V 
	// �F�̍X�V
	if (m_constBuffer.colors.w < 0.7f)// alpha�l��0.7f������������܂�0.9f�܂ŏ������グ��
		m_constBuffer.colors.w += 0.15f * elapsedTime;
	else// alpha�l��0.7f����0.9f�܂ł���������
		m_constBuffer.colors.w = 0.8f + 0.1f * sin(m_time);
	m_constBuffer.colors = Vector4(1.0f, 0.0f, 0.0f, m_constBuffer.colors.w);// �F�̍X�V
}
/*
*	@brief	�`��֐�
*	@details �|���S����`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  Crisis::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =//	���_��� 
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),  Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f),   Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),    Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f),   Vector2(0.0f, 1.0f)),// ����
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();// �r���[�s���]�u
	m_constBuffer.matProj = m_proj.Transpose();// �v���W�F�N�V�����s���]�u
	m_constBuffer.matWorld = m_world.Transpose();// ���[���h�s���]�u
	m_constBuffer.time = Vector4(m_time);// ���Ԃ�Vector4�ɕϊ�(16�o�C�g���E�ɍ��킹�邽�߂�Vector4���g�p)
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// �󂯓n���p�o�b�t�@�̓��e�X�V
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);// �`�揀��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->DrawTexture(vertex);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�̓o�^���������Ă���
}