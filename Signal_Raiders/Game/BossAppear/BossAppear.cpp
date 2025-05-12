/*
*	@file BossAppear.cpp
*	@brief �{�X�o�����o�̃N���X�̃\�[�X�t�@�C��
*/
#include "pch.h"
#include "BossAppear.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief �C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC> BossAppear::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*  @brief �R���X�g���N�^
*  @param �Ȃ�
*  @return �Ȃ�
*/
BossAppear::BossAppear()
	: m_world(Matrix::Identity)// ���[���h�s��
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
	, m_timer(0.0f)// �^�C�}�[
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_constantBuffer{}// �R���X�^���g�o�b�t�@
	, m_pInputLayout{}// �C���v�b�g���C�A�E�g
	, m_texture{}// �e�N�X�`��
	, m_commonResources{}// �R�������\�[�X
{
}

/*
*  @brief �f�X�g���N�^
*  @param �Ȃ�
*  @return �Ȃ�
*/
BossAppear::~BossAppear()
{
	Finalize();// �I������
}

/*
*  @brief �������֐�
*  @param resources �R�������\�[�X
*  @return �Ȃ�
*/
void BossAppear::Initialize(CommonResources* resources)
{
	m_commonResources = resources;	// �R�������\�[�X�̎擾
	m_pDR = m_commonResources->GetDeviceResources();	// �f�o�C�X���\�[�X�̎擾
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// �V�F�[�_�[�쐬�N���X�̏�����
	this->SettingShader();	// �V�F�[�_�[�̍쐬
	this->LoadTexture(L"Resources/Textures/BossAppear.png");// �e�N�X�`���̓ǂݍ���
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// �|���S���`��p

}

/*
*  @brief �e�N�X�`���̓ǂݍ���
*�@@param path �e�N�X�`���̃p�X
*  @return �Ȃ�
*/
void BossAppear::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`���̃|�C���^�i�ꎞ�ۑ��p�j
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_texture.push_back(texture);// �e�N�X�`���̃|�C���^���i�[
}

/*
*  @brief �V�F�[�_�[�̐ݒ�
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::SettingShader()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/BossAppear/VS_BossAppear.cso", m_vertexShader);// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppear.cso", m_pixelShader);// �s�N�Z���V�F�[�_�[�쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g���擾
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppearBack.cso", m_pixelShaderBack);// �s�N�Z���V�F�[�_�[�쐬�i�w�i�p�j
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));	// �萔�o�b�t�@�쐬
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.ps = m_pixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[�͎g�p���Ȃ�
	// �w�i�p�V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shadersBack.vs = m_vertexShader.Get();// ���_�V�F�[�_�[
	m_shadersBack.ps = m_pixelShaderBack.Get();	// �s�N�Z���V�F�[�_�[
	m_shadersBack.gs = nullptr;// �W�I���g���V�F�[�_�[�͎g�p���Ȃ�
}

/*
*  @brief �X�V�֐�
*  @param elapsedTime �t���[������
*  @return �Ȃ�
*/
void BossAppear::Update(float elapsedTime)
{
	m_timer += elapsedTime;// ���ԉ��Z
}

/*
*  @brief �`��֐�
*  @param view �r���[�s��
*  @param proj �v���W�F�N�V�����s��
*  @return �Ȃ�
*/
void BossAppear::Render()
{
	DrawBack();// �w�i�`��
	DrawMain();// ���C���e�N�X�`���`��
}
/*
*  @brief �I���֐�
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::Finalize()
{
	//�@do nothing
}

/*
*  @brief �`��֐�(���C���e�N�X�`��)
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::DrawMain()
{
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  0.5f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  0.7f, 0.0f) , SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0, -0.5f, 0.0f)  , SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -0.7f, 0.0f) , SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// 2�b�o�ߌ�Ɋg����J�n
	float expandStartTime = 2.0f;  // �g��J�n����
	float expandDuration = 0.5f;    // �g��ɂ����鎞�ԁi0.5�b�Ŋ����j
	// �g�嗦���v�Z
	float scaleFactor = 1.0f; // �����l�i�g��Ȃ��j
	// �g��J�n���Ԃ��߂�����g����J�n
	if (m_timer > expandStartTime)
	{
		float t = (m_timer - expandStartTime) / expandDuration;// �g�厞�Ԃ��v�Z
		t = std::min(t, 1.0f); // 1.0�𒴂��Ȃ��悤�ɃN�����v
		scaleFactor = Lerp(1.0f, 5.0f, t * 10); // 1.0 ���� 2.0 �փX���[�Y�Ɋg��
	}
	// ���_�̊g��
	vertex[0].position = SimpleMath::Vector3(-1.0f, 0.5f, 0.0f) * scaleFactor;
	vertex[1].position = SimpleMath::Vector3(1.0f, 0.7f, 0.0f) * scaleFactor;
	vertex[2].position = SimpleMath::Vector3(1.0f, -0.5f, 0.0f) * scaleFactor;
	vertex[3].position = SimpleMath::Vector3(-1.0f, -0.7f, 0.0f) * scaleFactor;
	// �R���X�^���g�o�b�t�@�̐ݒ�
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);// �F
	m_constantBuffer.time = Vector4(m_timer); // ����
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constantBuffer);	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);// �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �`��I��
	m_pDrawPolygon->ReleaseShader();
}

/*
*  @brief �`��֐�(�w�i)
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::DrawBack()
{
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0f, 1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �R���X�^���g�o�b�t�@�̐ݒ�
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);// �F
	m_constantBuffer.time = Vector4(m_timer);// ����
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constantBuffer);	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);// �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shadersBack, nullptr, 0);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �`��I��
	m_pDrawPolygon->ReleaseShader();
}