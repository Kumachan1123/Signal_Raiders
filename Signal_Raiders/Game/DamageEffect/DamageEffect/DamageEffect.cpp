//--------------------------------------------------------------------------------------
// File: DamageEffect.h
//
// �U�����󂯂����̉��o�N���X
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "DamageEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �C���v�b�g���C�A�E�g
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC>  DamageEffect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// �R���X�g���N�^
DamageEffect::DamageEffect(CommonResources* resources)
	: m_commonResources{ resources }
	, m_time{ 0.0f }
	, m_constBuffer{}
	, m_pDR{ resources->GetDeviceResources() }
	, m_pPlayer{ nullptr }
	, m_enemyDirection{  }
	, m_playEffect{ false }
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	// �F�̏�����
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}

// �f�X�g���N�^
DamageEffect::~DamageEffect()
{
}

// ������
void DamageEffect::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_playEffect = true;
	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ��݁i2���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/crisis.png");
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
	// �����Ă����G�̌������擾
	m_enemyDirection = m_pPlayer->GetEnemyBulletDirection();
}

// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
void  DamageEffect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}



// �V�F�[�_�[�쐬����
void  DamageEffect::CreateShader()
{
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/DamageEffect/VS_Damage.cso", m_vertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/DamageEffect/PS_Damage.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃZ�b�g
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}



// �X�V
void  DamageEffect::Update(float elapsedTime)
{
	if (!m_playEffect)return;
	// ���ԍX�V
	m_time += elapsedTime;
	if (m_time >= PLAY_TIME)
	{
		m_time = 0.0f;
		m_constBuffer.colors.w = 0.0f;
		m_playEffect = false;
		m_pPlayer->SetisPlayEffect(m_playEffect);
		return;
	}
	m_constBuffer.time = Vector4(m_time);
	// �A���t�@�l(sin�֐����g���ē�b�ȓ���0����1����������)
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);
	// �p�x���v�Z
	float angle = CalculateAngle();
	m_constBuffer.uv = GetUVFromAngle(angle);
}
// �p�x�v�Z
float DamageEffect::CalculateAngle() const
{
	Vector3 playerDir = m_pPlayer->GetPlayerDir();// �v���C���[�̌���
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(playerDir.x, playerDir.z);// �v���C���[�ƓG�̌����̍�
	angle = DirectX::XMConvertToDegrees(angle);// ���W�A������x�ɕϊ�
	return (angle < 0) ? angle + 360 : angle;// 0����360�̊ԂɎ��߂�
}
// �p�x����UV���W�����߂�
DirectX::SimpleMath::Vector4 DamageEffect::GetUVFromAngle(float angle) const
{
	// �p�x�idegree�j�����W�A���ɕϊ�
	float rad = DirectX::XMConvertToRadians(angle);

	// ���a�͍��E�܂��͏㉺�̒��S����̋���
	float radius = UV_W - UV_C;

	// ���W���v�Z
	float u = UV_C - radius * -sin(rad);
	float v = UV_C + radius * cos(rad);

	return DirectX::SimpleMath::Vector4(u, v, 0, 0);
}
// �`��
void  DamageEffect::Render()
{

	// ���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		// ���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),  SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),  SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}