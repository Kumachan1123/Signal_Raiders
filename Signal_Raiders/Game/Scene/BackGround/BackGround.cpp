/*
*	@file BackGround.h
*	@brief �w�i�N���X
*/

#include "pch.h"
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

	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // �V�F�[�_�[�쐬�N���X�̏�����
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	m_color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);// �F�̏�����
}
/*
*	@brief �f�X�g���N�^
*	@details �w�i�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
BackGround::~BackGround() {/* do nothing */ }
/*
*	@brief �e�N�X�`�����\�[�X�ǂݍ���
*	@details �e�N�X�`�����\�[�X��ǂݍ���
*	@param path �ǂݍ��ރe�N�X�`���̃p�X
*	@return �Ȃ�
*/
void  BackGround::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`���n���h��
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_pTexture.push_back(texture);// �e�N�X�`����ǂݍ���
}
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
	LoadTexture(L"Resources/Textures/Back.png");// �摜�̓ǂݍ��݁@
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
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/CRT/VS_CRT.cso", m_pVertexShader);// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/CRT/PS_CRT.cso", m_pPixelShader);// �s�N�Z���V�F�[�_�[�̍쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[(�g��Ȃ��̂�nullptr)
}
/*
*	@brief �X�V
*	@details �w�i�N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void  BackGround::Update(float elapsedTime)
{
	m_time += elapsedTime;// ���ԍX�V
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
	VertexPositionTexture vertex[4] =// ���_����UV�����i�[����z��
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),  Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();// �r���[�s��
	m_constBuffer.matProj = m_proj.Transpose();// �v���W�F�N�V�����s��
	m_constBuffer.matWorld = m_world.Transpose();// ���[���h�s��
	m_constBuffer.colors = Vector4(m_color);// �F
	m_constBuffer.time = Vector4(m_time);// ����
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->DrawSetting(	// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);//	�f�v�X�X�e���V���X�e�[�g
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);// �`�揀��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->DrawTexture(vertex);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�̓o�^���������Ă���
}