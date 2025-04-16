/*
*	@file DamageEffect.cpp
*	@brief �U�����󂯂����̉��o�N���X
*/
#include "pch.h"
#include "DamageEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief ���_���
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  DamageEffect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief �R���X�g���N�^
*	@param[in] resources CommonResources�N���X�̃|�C���^
*	@return �Ȃ�
*/
DamageEffect::DamageEffect(CommonResources* resources)
	: m_commonResources(resources)
	, m_time(0.0f)
	, m_constBuffer{}
	, m_pDR(resources->GetDeviceResources())
	, m_pPlayer(nullptr)
	, m_pEnemy(nullptr)
	, m_enemyDirection{}
	, m_playEffect(false)
	, m_effectType(EffectType::DAMAGE)
	, m_pDrawPolygon(DrawPolygon::GetInstance())
	, m_pCreateShader(CreateShader::GetInstance())
{
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);	// �F�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// �V�F�[�_�[�쐬�N���X�̏�����
}

/*
*	@brief �f�X�g���N�^
*	@return �Ȃ�
*/
DamageEffect::~DamageEffect() {}

/*
*	@brief ������
*	@param[in] pPlayer �v���C���[�̃|�C���^
*	@return �Ȃ�
*/
void DamageEffect::Initialize()
{
	m_playEffect = true;// �G�t�F�N�g�Đ��t���O��true�ɂ���
	MakeShader();	// �V�F�[�_�[�̍쐬
	LoadTexture(L"Resources/Textures/WARNING.png");// �e�N�X�`���ǂݍ���
	m_pDrawPolygon->InitializePositionTexture(m_pDR);// ���_���̏�����
	if (m_effectType == EffectType::DAMAGE)
	{
		m_constBuffer.colors = DirectX::SimpleMath::Vector4(2.0f, 0.0f, 0.0f, 1.0f);// �G�t�F�N�g�^�C�v���_���[�W�Ȃ�ԐF
		m_enemyDirection = m_pPlayer->GetEnemyBulletDirection();// �U�����Ă����G�̌������擾
	}
	else if (m_effectType == EffectType::INCOMINGENEMY)
	{
		m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 0.5f, 0.0f, 1.0f);// �G�t�F�N�g�^�C�v���x���Ȃ物�F
		m_enemyPosition = m_pEnemy->GetPosition();// �G�̈ʒu���擾
		m_enemyDirection = m_pPlayer->GetPlayerPos() - m_enemyPosition;// �v���C���[�ƓG�̈ʒu�̍����擾
	}
}

/*
*	@brief �e�N�X�`�����\�[�X�ǂݍ��݊֐�
*	@param[in] path �e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void  DamageEffect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`�����\�[�X�r���[
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_textures.push_back(texture);// �e�N�X�`�����\�[�X�r���[���i�[
}



/*
*	@brief �V�F�[�_�[�쐬
*	@return �Ȃ�
*/
void  DamageEffect::MakeShader()
{
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/DamageEffect/VS_Damage.cso", m_vertexShader);// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/DamageEffect/PS_Damage.cso", m_pixelShader);	// �s�N�Z���V�F�[�_�[�쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();// ���̓��C�A�E�g�擾
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));// �R���X�^���g�o�b�t�@�쐬
	m_shaders.vs = m_vertexShader.Get();// ���_�V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pixelShader.Get();// �s�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[�͎g��Ȃ��̂�nullptr
}

/*
*	@brief �X�V
*	@param[in] elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void  DamageEffect::Update(float elapsedTime)
{
	if (!m_playEffect)return;// �G�t�F�N�g�Đ��t���O��false�Ȃ�X�V���Ȃ�
	m_time += elapsedTime;// ���Ԃ����Z
	if (m_time >= PLAY_TIME)// �Đ����Ԃ��߂�����
	{
		m_time = 0.0f;// ���Ԃ�������
		m_constBuffer.colors.w = 0.0f;// �A���t�@�l��������
		m_playEffect = false;// �G�t�F�N�g�Đ��t���O��false�ɂ���
		m_pPlayer->SetisPlayEffect(m_playEffect);// �v���C���[�ɃG�t�F�N�g�Đ��t���O�𑗂�
		return;// �X�V�I��
	}
	m_constBuffer.time = Vector4(m_time);// ���Ԃ��R���X�^���g�o�b�t�@�Ɋi�[
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);// �A���t�@�l���v�Z
	float angle = CalculateAngle();// �p�x�v�Z
	m_constBuffer.uv = GetUVFromAngle(angle);// �p�x����UV���W�����߂�
}
/*
*	@brief �p�x�v�Z
*	@return �p�x
*/
float DamageEffect::CalculateAngle() const
{
	Vector3 playerDir = m_pPlayer->GetPlayerDir();// �v���C���[�̌���
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(playerDir.x, playerDir.z);// �v���C���[�ƓG�̌����̍�
	angle = DirectX::XMConvertToDegrees(angle);// ���W�A������x�ɕϊ�
	return (angle < 0) ? angle + 360 : angle;// 0����360�̊ԂɎ��߂�
}
/*
*	@brief �p�x����UV���W�����߂�
*	@param[in] angle �p�x
*	@return UV���W
*/
DirectX::SimpleMath::Vector4 DamageEffect::GetUVFromAngle(float angle) const
{
	float rad = DirectX::XMConvertToRadians(angle);// �x���烉�W�A���ɕϊ�
	float radius = UV_W - UV_C;	// ���a�͍��E�܂��͏㉺�̒��S����̋���
	float u = UV_C - radius * -sin(rad);// ���W���v�ZU
	float v = UV_C + radius * cos(rad);// ���W���v�ZV
	return DirectX::SimpleMath::Vector4(u, v, 0, 0);// UV���W��Ԃ�
}
/*
*	@brief �`��
*	@return �Ȃ�
*/
void  DamageEffect::Render()
{

	VertexPositionTexture vertex[4] =	// ���_���(�|���S���̂S���_�̍��W���j
	{
		// ���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),  SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),  SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	m_constBuffer.matView = m_view.Transpose();// �r���[�s���]�u
	m_constBuffer.matProj = m_proj.Transpose();// �v���W�F�N�V�����s���]�u
	m_constBuffer.matWorld = m_world.Transpose();// ���[���h�s���]�u
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);// �R���X�^���g�o�b�t�@�̍X�V
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };// �R���X�^���g�o�b�t�@�̃Z�b�g
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->DrawSetting(// 	// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);// �`��J�n
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// �V�F�[�_�[�̓o�^
	m_pDrawPolygon->DrawTexture(vertex);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�[���
}