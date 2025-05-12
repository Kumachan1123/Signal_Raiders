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
	: m_world(Matrix::Identity)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
	, m_timer(0.0f)
	, m_pDR{}
	, m_constantBuffer{}
	, m_pInputLayout{}
	, m_texture{}
	, m_vertices{}
	, m_commonResources{}
{
}

/*
*  @brief �f�X�g���N�^
*  @param �Ȃ�
*  @return �Ȃ�
*/
BossAppear::~BossAppear()
{
	Finalize();
}

/*
*  @brief �������֐�
*  @param resources �R�������\�[�X
*  @return �Ȃ�
*/
void BossAppear::Initialize(CommonResources* resources)
{

	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);

	// �V�F�[�_�[�̍쐬
	this->SettingShader();
	// �e�N�X�`���̓ǂݍ���
	this->LoadTexture(L"Resources/Textures/BossAppear.png");

	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}

/*
*  @brief �e�N�X�`���̓ǂݍ���
*�@@param path �e�N�X�`���̃p�X
*  @return �Ȃ�
*/
void BossAppear::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());
	m_texture.push_back(texture);
}

/*
*  @brief �V�F�[�_�[�̐ݒ�
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::SettingShader()
{
	// �V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/BossAppear/VS_BossAppear.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppear.cso", m_pixelShader);
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppearBack.cso", m_pixelShaderBack);

	// �C���v�b�g���C�A�E�g���󂯎��
	// �萔�o�b�t�@�쐬
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;

	m_shadersBack.vs = m_vertexShader.Get();
	m_shadersBack.ps = m_pixelShaderBack.Get();
	m_shadersBack.gs = nullptr;
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
	if (m_timer > expandStartTime)
	{
		float t = (m_timer - expandStartTime) / expandDuration;
		t = std::min(t, 1.0f); // 1.0�𒴂��Ȃ��悤�ɃN�����v
		scaleFactor = Lerp(1.0f, 5.0f, t * 10); // 1.0 ���� 2.0 �փX���[�Y�Ɋg��
	}

	// ���_�̊g��
	vertex[0].position = SimpleMath::Vector3(-1.0f, 0.5f, 0.0f) * scaleFactor;
	vertex[1].position = SimpleMath::Vector3(1.0f, 0.7f, 0.0f) * scaleFactor;
	vertex[2].position = SimpleMath::Vector3(1.0f, -0.5f, 0.0f) * scaleFactor;
	vertex[3].position = SimpleMath::Vector3(-1.0f, -0.7f, 0.0f) * scaleFactor;

	// �R���X�^���g�o�b�t�@�̐ݒ�
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_constantBuffer.time = Vector4(m_timer);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constantBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �`��I��
	// �V�F�[�_�̓o�^���������Ă���
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
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_constantBuffer.time = Vector4(m_timer);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constantBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shadersBack, nullptr, 0);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �`��I��
	m_pDrawPolygon->ReleaseShader();
}