/*
*	@file	EnemyHPBar.cpp
*	@brief	�GHPBar�N���X
*/
#include <pch.h>
#include "EnemyHPBar.h"


/*
*	@brief	�C���v�b�g���C�A�E�g
*	@return 	�Ȃ�
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyHPBar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details �GHPBar�N���X�̃R���X�g���N�^
*	@param	�Ȃ�
*	@return �Ȃ�
*/
EnemyHPBar::EnemyHPBar()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_maxHP(100)// �G�̍ő�HP
	, m_currentHP(100)// �G�̌���HP
	, m_displayedHP(100)// �G�̕\��HP
	, m_lerpSpeed(5.0f)// ��ԑ��x
	, m_scale(1.5f)// �X�P�[��
	, m_time{ 0.0f }// �o�ߎ���
	, m_isDead(false)// ���S�t���O
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
	using namespace DirectX::SimpleMath;
	// HP�o�[�̒��_����ݒ肷��
	m_hpbarVert[0] = { Vector3(HPBAR_X_MIN,HPBAR_Y_MAX,0.0f),Vector2(0.0f,0.0f) };	// ����
	m_hpbarVert[1] = { Vector3(HPBAR_X_MAX,HPBAR_Y_MAX,0.0f),Vector2(1.0f,0.0f) };	// �E��
	m_hpbarVert[2] = { Vector3(HPBAR_X_MAX,HPBAR_Y_MIN,0.0f),Vector2(1.0f,1.0f) };	// �E��
	m_hpbarVert[3] = { Vector3(HPBAR_X_MIN,HPBAR_Y_MIN,0.0f),Vector2(0.0f,1.0f) };	// ����
	// HP�o�[�̔w�i�̒��_����ݒ肷��
	m_hpbarBackVert[0] = { Vector3(HPBARBACK_X_MIN,HPBARBACK_Y_MAX,0.0f),Vector2(0.0f,0.0f) };// ����
	m_hpbarBackVert[1] = { Vector3(HPBARBACK_X_MAX,HPBARBACK_Y_MAX,0.0f),Vector2(1.0f,0.0f) };	// �E��
	m_hpbarBackVert[2] = { Vector3(HPBARBACK_X_MAX,HPBARBACK_Y_MIN,0.0f),Vector2(1.0f,1.0f) };	// �E��
	m_hpbarBackVert[3] = { Vector3(HPBARBACK_X_MIN,HPBARBACK_Y_MIN,0.0f),Vector2(0.0f,1.0f) };// ����

}

/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return �Ȃ�
*/
EnemyHPBar::~EnemyHPBar()
{
	// ���ʃ��\�[�X��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief	�V�F�[�_�[���쐬����
*	@details	�GHP�o�[�̃V�F�[�_�[���쐬����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyHPBar::CreateShaders()
{
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/EnemyHP/VS_EnemyHP.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/EnemyHP/PS_EnemyHP.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[��n��
	m_shaders.ps = m_pPixelShader.Get();
	// �V�F�[�_�[�̍\���̂ɃW�I���g���V�F�[�_�[��n���i�g��Ȃ��̂�nullptr�j
	m_shaders.gs = nullptr;
}
/*
*	@brief	������
*	@details	�GHP�o�[�̏�����
*	@param	CommonResources* resources	���ʃ��\�[�X
*	@return �Ȃ�
*/
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �|���S���`��p�N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �e�N�X�`���̎擾
	m_pGaugeTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("EnemyHPBar"));
	// ����HP��ݒ�
	m_displayedHP = (float)(m_maxHP);
}
/*
*	@brief	�X�V
*	@details	�GHP�o�[�̍X�V
*	@param	float elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void EnemyHPBar::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_time += elapsedTime;
	// HP�����炩�ɍX�V���邽�߂̐��`���
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);
	// �\��HP�Ɋ�Â���HP�������v�Z
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);
	// HP�o�[�̍ő厞�̕���ݒ� 
	float maxBarWidth = 3.0f;
	// HP�o�[�̕����v�Z
	float currentBarWidth = maxBarWidth * hpPercentage;
	// HP�o�[�̒��_��ݒ� 
	m_hpbarVert[0].position.x = BAR_LEFT;                    // ����
	m_hpbarVert[1].position.x = BAR_LEFT + currentBarWidth;  // �E��
	m_hpbarVert[2].position.x = BAR_LEFT + currentBarWidth;  // �E��
	m_hpbarVert[3].position.x = BAR_LEFT;                    // ����
	// HP�o�[�̕���0�ȉ��ɂȂ����玀�S
	if (m_hpbarVert[1].position.x <= BAR_LEFT)m_isDead = true;
}
/*
*	@brief	�`��
*	@details	�GHP�o�[�̕`��
*	@param	DirectX::SimpleMath::Matrix view	�r���[�s��
*	@param	DirectX::SimpleMath::Matrix proj	�v���W�F�N�V�����s��
*	@param	DirectX::SimpleMath::Vector3 pos	���W
*	@param	DirectX::SimpleMath::Vector3 rot	��]
*	@return �Ȃ�
*/
void EnemyHPBar::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& rot)
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
	// �r���{�[�h�s�����
	worldBillboard = billboardMatrix;
	// �X�P�[����������
	worldBillboard *= Matrix::CreateScale(m_scale);
	// ��]��������
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));
	// �r���{�[�h�̈ʒu��ݒ�
	worldBillboard *= Matrix::CreateTranslation(pos);
	// ��]��������
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));
	// �r���[�ݒ�
	m_constBuffer.matView = view.Transpose();
	// �v���W�F�N�V�����ݒ�
	m_constBuffer.matProj = proj.Transpose();
	// ���[���h�ݒ�
	m_constBuffer.matWorld = worldBillboard.Transpose();
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// �[�x�X�e���V���X�e�[�g
	// HPbar(�w�i)�`��///////////////////////////////////////////////////////////////////////////////
	// �F�ݒ� 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	// �V�F�[�_�[�ɓn���o�b�t�@���쐬
	CreateBuffer();
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pGaugeTexture);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(m_hpbarBackVert);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
	// HPbar(��)�`��///////////////////////////////////////////////////////////////////////////////
	// �F�ݒ�
	m_constBuffer.colors = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// �V�F�[�_�[�ɓn���o�b�t�@���쐬
	CreateBuffer();
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pGaugeTexture);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(m_hpbarVert);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

/*
*	@brief	�V�F�[�_�[�ɓn���o�b�t�@���쐬
*	@details	�GHP�o�[�̃V�F�[�_�[�ɓn���o�b�t�@���쐬����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyHPBar::CreateBuffer()
{
	using namespace DirectX::SimpleMath;
	// ���Ԑݒ�
	m_constBuffer.hp = Vector4(float(m_currentHP), float(m_maxHP), 0, 0);
	// �󂯓n���p�o�b�t�@�̓��e�X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
}