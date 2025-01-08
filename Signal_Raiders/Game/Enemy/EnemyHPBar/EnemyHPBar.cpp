/*
	@file	EnemyHPBar.cpp
	@brief	�GHPBar�N���X
*/
#include "pch.h"
#include "EnemyHPBar.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyHPBar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// �R���X�g���N�^
EnemyHPBar::EnemyHPBar()
	: m_commonResources{}
	, m_maxHP(100)
	, m_currentHP(100)
	, m_displayedHP(100)
	, m_lerpSpeed(5.0f)
	, m_scale(1.5f)
	, m_time{ 0.0f }
	, m_isDead(false)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	// �r���{�[�h�̒��_����ݒ肷��
	m_hpbarVert[0] = { Vector3(-1.5f,3.25f,0.0f),Vector2(0.0f,0.0f) };	// ����
	m_hpbarVert[1] = { Vector3(1.5f,3.25f,0.0f),Vector2(1.0f,0.0f) };	// �E��
	m_hpbarVert[2] = { Vector3(1.5f,3.0f,0.0f),Vector2(1.0f,1.0f) };	// �E��
	m_hpbarVert[3] = { Vector3(-1.5f,3.0f,0.0f),Vector2(0.0f,1.0f) };	// ����
	// �r���{�[�h�̒��_����ݒ肷��
	m_hpbarBackVert[0] = { Vector3(-1.60f,3.27f,0.0f),Vector2(0.0f,0.0f) };	// ����
	m_hpbarBackVert[1] = { Vector3(1.60f,3.27f,0.0f),Vector2(1.0f,0.0f) };	// �E��
	m_hpbarBackVert[2] = { Vector3(1.60f,2.98f,0.0f),Vector2(1.0f,1.0f) };	// �E��
	m_hpbarBackVert[3] = { Vector3(-1.60f,2.98f,0.0f),Vector2(0.0f,1.0f) };	// ����

}
// �f�X�g���N�^
EnemyHPBar::~EnemyHPBar() {}

//---------------------------------------------------------
// �摜��ǂݍ���
//---------------------------------------------------------
void EnemyHPBar::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	m_gaugeTexture.push_back(texture);
}



void EnemyHPBar::CreateShader()
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/EnemyHP/VS_EnemyHP.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/EnemyHP/PS_EnemyHP.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}


//---------------------------------------------------------
//// ����������
////---------------------------------------------------------
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	// �|���S���`��p�N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());
	// �V�F�[�_�[�̍쐬
	CreateShader();
	LoadTexture(L"Resources/Textures/EnemyHPBar.png");// �e�N�X�`���ǂݍ���
	m_displayedHP = (float)(m_maxHP);// ����HP
}
// �`��
void EnemyHPBar::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// �r���{�[�h�s����쐬
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;

	// �r���{�[�h���A�t�B���ϊ�
	Matrix worldBillboard{};
	worldBillboard = billboardMatrix;
	worldBillboard *= Matrix::CreateScale(m_scale);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));	// �B
	worldBillboard *= Matrix::CreateTranslation(pos);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));	// �B
	// �r���[�ݒ�
	m_constBuffer.matView = view.Transpose();
	// �v���W�F�N�V�����ݒ�
	m_constBuffer.matProj = proj.Transpose();
	// ���[���h�ݒ�
	m_constBuffer.matWorld = worldBillboard.Transpose();
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ� 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	CreateBuffer();

	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_gaugeTexture);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(m_hpbarBackVert);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();

	// HPbar(��)�`��///////////////////////////////////////////////////////////////////////////////
	// �F���ւ񂱂� 
	m_constBuffer.colors = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	CreateBuffer();

	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_gaugeTexture);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(m_hpbarVert);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();

	//// �r���{�[�h��`��
	//m_primitiveBatch->Begin();
	//m_primitiveBatch->DrawQuad(m_hpbarVert[0], m_hpbarVert[1], m_hpbarVert[3], m_hpbarVert[2]);
	//m_primitiveBatch->End();
}
// �X�V
void EnemyHPBar::Update(float elapsedTime)
{
	// ����
	m_time += elapsedTime;


	// HP�����炩�ɍX�V���邽�߂̐��`���
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);

	// �\��HP�Ɋ�Â���HP�������v�Z
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);

	// HP�o�[�̕����v�Z 
	float maxBarWidth = 3.0f;
	float currentBarWidth = maxBarWidth * hpPercentage;

	// HP�o�[�̒��_��ݒ� (�o�[�̍��[�� -1.50f �Ƃ��āA�E�[�� 1.50f �ɂ���)
	m_hpbarVert[0].position.x = -1.50f;                    // ����
	m_hpbarVert[1].position.x = -1.50f + currentBarWidth;  // �E��
	m_hpbarVert[2].position.x = -1.50f + currentBarWidth;  // �E��
	m_hpbarVert[3].position.x = -1.50f;                    // ����

	if (m_hpbarVert[1].position.x <= -1.50f)
	{
		m_isDead = true;
	}

}

void EnemyHPBar::CreateBuffer()
{
	// ���Ԑݒ�
	// ���Ԑݒ�
	m_constBuffer.hp = SimpleMath::Vector4(float(m_currentHP), float(m_maxHP), 0, 0);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
}