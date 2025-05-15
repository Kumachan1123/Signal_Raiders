/*
	@file	EnemyHPBar.cpp
	@brief	�GHPBar�N���X
*/
#include "pch.h"
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
*	@param[in]	�Ȃ�
*	@return �Ȃ�
*/
EnemyHPBar::EnemyHPBar()
	: m_commonResources{}// ���ʃ��\�[�X
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
*	@return �Ȃ�
*/
EnemyHPBar::~EnemyHPBar() {/*do nothing*/ }

/*
*	@brief	�摜��ǂݍ���
*	@param[in]	const wchar_t*�@path	�摜�̃p�X
*	@return �Ȃ�
*/
void EnemyHPBar::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`�����i�[����|�C���^
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`����ǂݍ���
	m_gaugeTexture.push_back(texture);// �e�N�X�`�����i�[����
}
/*
*	@brief	�V�F�[�_�[���쐬����
*	@param[in]	�Ȃ�
*	@return �Ȃ�
*/
void EnemyHPBar::CreateShaders()
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(),
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/EnemyHP/VS_EnemyHP.cso", m_vertexShader);// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/EnemyHP/PS_EnemyHP.cso", m_pixelShader);// �s�N�Z���V�F�[�_�[�쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();	// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.ps = m_pixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[(�g��Ȃ��̂�nullptr)
}
/*
*	@brief	������
*	@param[in]	CommonResources* resources	���ʃ��\�[�X
*	@return �Ȃ�
*/
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;// ���ʃ��\�[�X���擾
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());	// �|���S���`��p�N���X�̏�����
	CreateShaders();	// �V�F�[�_�[�̍쐬
	LoadTexture(L"Resources/Textures/EnemyHPBar.png");// �e�N�X�`���ǂݍ���
	m_displayedHP = (float)(m_maxHP);// ����HP
}
/*
*	@brief	�X�V
*	@param[in]	float elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void EnemyHPBar::Update(float elapsedTime)
{
	m_time += elapsedTime;// �o�ߎ��Ԃ����Z
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);	// HP�����炩�ɍX�V���邽�߂̐��`���
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);	// �\��HP�Ɋ�Â���HP�������v�Z
	float maxBarWidth = 3.0f;	// HP�o�[�̍ő厞�̕���ݒ� 
	float currentBarWidth = maxBarWidth * hpPercentage;	// HP�o�[�̕����v�Z
	// HP�o�[�̒��_��ݒ� 
	m_hpbarVert[0].position.x = BAR_LEFT;                    // ����
	m_hpbarVert[1].position.x = BAR_LEFT + currentBarWidth;  // �E��
	m_hpbarVert[2].position.x = BAR_LEFT + currentBarWidth;  // �E��
	m_hpbarVert[3].position.x = BAR_LEFT;                    // ����
	if (m_hpbarVert[1].position.x <= BAR_LEFT)m_isDead = true;// HP�o�[�̕���0�ȉ��ɂȂ����玀�S

}
/*
*	@brief	�`��
*	@param[in]	DirectX::SimpleMath::Matrix view	�r���[�s��
*	@param[in]	DirectX::SimpleMath::Matrix proj	�v���W�F�N�V�����s��
*	@param[in]	DirectX::SimpleMath::Vector3 pos	���W
*	@param[in]	DirectX::SimpleMath::Vector3 rot	��]
*	@return �Ȃ�
*/
void EnemyHPBar::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	Matrix billboardMatrix = view.Invert();	// �r���{�[�h�s����쐬
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	Matrix worldBillboard{};	// �r���{�[�h���A�t�B���ϊ�
	worldBillboard = billboardMatrix;// �r���{�[�h�s�����
	worldBillboard *= Matrix::CreateScale(m_scale);	// �X�P�[����������
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));	// ��]��������
	worldBillboard *= Matrix::CreateTranslation(pos);	// �r���{�[�h�̈ʒu��ݒ�
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));	// ��]��������
	m_constBuffer.matView = view.Transpose();	// �r���[�ݒ�
	m_constBuffer.matProj = proj.Transpose();	// �v���W�F�N�V�����ݒ�
	m_constBuffer.matWorld = worldBillboard.Transpose();// ���[���h�ݒ�
	m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// �[�x�X�e���V���X�e�[�g
	// HPbar(�w�i)�`��///////////////////////////////////////////////////////////////////////////////
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);	// �F�ݒ� 
	CreateBuffer();// �V�F�[�_�[�ɓn���o�b�t�@���쐬
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_gaugeTexture);	// �`�揀��
	m_pDrawPolygon->DrawTexture(m_hpbarBackVert);	// �|���S����`��
	m_pDrawPolygon->ReleaseShader();	// �V�F�[�_�̓o�^���������Ă���

	// HPbar(��)�`��///////////////////////////////////////////////////////////////////////////////
	m_constBuffer.colors = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);// �F�ݒ�
	CreateBuffer();// �V�F�[�_�[�ɓn���o�b�t�@���쐬
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_gaugeTexture);	// �`�揀��
	m_pDrawPolygon->DrawTexture(m_hpbarVert);	// �|���S����`��
	m_pDrawPolygon->ReleaseShader();	// �V�F�[�_�̓o�^���������Ă���
}

/*
*	@brief	�V�F�[�_�[�ɓn���o�b�t�@���쐬
*	@param[in]	�Ȃ�
*	@return �Ȃ�
*/
void EnemyHPBar::CreateBuffer()
{
	using namespace DirectX::SimpleMath;
	m_constBuffer.hp = Vector4(float(m_currentHP), float(m_maxHP), 0, 0);	// ���Ԑݒ�
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constBuffer);	// �󂯓n���p�o�b�t�@�̓��e�X�V
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);	// �V�F�[�_���Z�b�g����

}