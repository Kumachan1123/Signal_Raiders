/*
*	@file DamageEffect.cpp
*	@brief �U�����󂯂����̉��o�N���X
*/
#include <pch.h>
#include "DamageEffect.h"
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC>  DamageEffect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief �R���X�g���N�^
*	@details �_���[�W�G�t�F�N�g�̃R���X�g���N�^
*	@param resources CommonResources�N���X�̃|�C���^
*	@return �Ȃ�
*/
DamageEffect::DamageEffect(CommonResources* resources)
	: m_pCommonResources(resources)// ���\�[�X�N���X�̃|�C���^
	, m_time(0.0f)// ����
	, m_constBuffer{}// �R���X�^���g�o�b�t�@
	, m_pDR(resources->GetDeviceResources())// �f�o�C�X���\�[�X
	, m_pPlayer(nullptr)// �v���C���[
	, m_pEnemy(nullptr)// �G
	, m_enemyDirection{}// �U�����Ă����G�̌���
	, m_playEffect(false)// �G�t�F�N�g�Đ��t���O
	, m_effectType(EffectType::DAMAGE)// �G�t�F�N�g�^�C�v
	, m_pDrawPolygon(DrawPolygon::GetInstance())// �`��|���S���N���X�̃C���X�^���X
	, m_pCreateShader(CreateShader::GetInstance())// �V�F�[�_�[�쐬�N���X�̃C���X�^���X
{
}
/*
*	@brief �f�X�g���N�^
*	@details �_���[�W�G�t�F�N�g�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
DamageEffect::~DamageEffect() {/* do nothing */ }
/*
*	@brief ������
*	@details �_���[�W�G�t�F�N�g�̏�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DamageEffect::Initialize()
{
	using namespace DirectX::SimpleMath;
	// �G�t�F�N�g�Đ��t���O��true�ɂ���
	m_playEffect = true;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	MakeShader();
	// ���_���̏�����
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
	// �G�t�F�N�g�̃^�C�v�ɉ����ĕ���
	if (m_effectType == EffectType::DAMAGE)// �_���[�W�G�t�F�N�g�Ȃ�
	{
		// �ԐF�ɐݒ�
		m_constBuffer.colors = Vector4(2.0f, 0.0f, 0.0f, 1.0f);
		// �U�����Ă����G�̌������擾
		m_enemyDirection = m_pPlayer->GetEnemyBulletDirection();
	}
	else if (m_effectType == EffectType::INCOMINGENEMY)// �x���G�t�F�N�g�Ȃ�
	{
		// ���F�ɐݒ�
		m_constBuffer.colors = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
		// �G�̈ʒu���擾
		m_enemyPosition = m_pEnemy->GetPosition();
		// �v���C���[�ƓG�̈ʒu�̍����擾
		m_enemyDirection = m_pPlayer->GetPlayerPos() - m_enemyPosition;
	}
}
/*
*	@brief �V�F�[�_�[�쐬
*	@details �V�F�[�_�[�̍쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  DamageEffect::MakeShader()
{
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/DamageEffect/VS_Damage.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/DamageEffect/PS_Damage.cso", m_pPixelShader);
	// ���̓��C�A�E�g�擾
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �R���X�^���g�o�b�t�@�쐬
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// ���_�V�F�[�_�[���Z�b�g
	m_shaders.vs = m_pVertexShader.Get();
	// �s�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[�͎g��Ȃ��̂�nullptr
	m_shaders.gs = nullptr;
}
/*
*	@brief �X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void  DamageEffect::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �G�t�F�N�g�Đ��t���O��false�Ȃ�X�V���Ȃ�
	if (!m_playEffect)return;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// �Đ����Ԃ��߂�����
	if (m_time >= PLAY_TIME)
	{
		// ���Ԃ�������
		m_time = 0.0f;
		// �A���t�@�l��������
		m_constBuffer.colors.w = 0.0f;
		// �G�t�F�N�g�Đ��t���O��false�ɂ���
		m_playEffect = false;
		// �v���C���[�ɃG�t�F�N�g�Đ��t���O�𑗂�
		m_pPlayer->SetisPlayEffect(m_playEffect);
		// �X�V�I��
		return;
	}
	// ���Ԃ��R���X�^���g�o�b�t�@�Ɋi�[
	m_constBuffer.time = Vector4(m_time);
	// �A���t�@�l���v�Z
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);
	// �p�x���v�Z����
	float angle = CalculateAngle();
	// �p�x����UV���W�����߂�
	m_constBuffer.uv = GetUVFromAngle(angle);
}
/*
*	@brief �p�x�v�Z
*	@details �G�̌����ƃv���C���[�̌����̍����v�Z
*	@param �Ȃ�
*	@return �p�x
*/
float DamageEffect::CalculateAngle() const
{
	using namespace DirectX::SimpleMath;
	// �v���C���[�̌���
	Vector3 playerDir = m_pPlayer->GetPlayerDir();
	// �v���C���[�ƓG�̌����̍�
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(playerDir.x, playerDir.z);
	// ���W�A������x�ɕϊ�
	angle = DirectX::XMConvertToDegrees(angle);
	// 0����360�̊ԂɎ��߂�
	return (angle < 0) ? angle + 360 : angle;
}
/*
*	@brief �p�x����UV���W�����߂�
*	@details �p�x����UV���W�����߂�
*	@param angle �p�x
*	@return UV���W
*/
DirectX::SimpleMath::Vector4 DamageEffect::GetUVFromAngle(float angle) const
{
	using namespace DirectX::SimpleMath;
	// �x���烉�W�A���ɕϊ�
	float rad = DirectX::XMConvertToRadians(angle);
	// ���a�͍��E�܂��͏㉺�̒��S����̋���
	float radius = UV_W - UV_C;
	// ���W���v�Z:U
	float u = UV_C - radius * -sin(rad);
	// ���W���v�Z:V
	float v = UV_C + radius * cos(rad);
	// UV���W��Ԃ�
	return  Vector4(u, v, 0, 0);
}
/*
*	@brief �`��
*	@details �_���[�W�G�t�F�N�g�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void  DamageEffect::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		// ���_���													UV���
		VertexPositionTexture(Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),  Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f),   Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),   Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f),  Vector2(0.0f, 1.0f)),// ����
	};
	// �r���[�s���]�u���ăR���X�^���g�o�b�t�@�Ɋi�[
	m_constBuffer.matView = m_view.Transpose();
	// �v���W�F�N�V�����s���]�u���ăR���X�^���g�o�b�t�@�Ɋi�[
	m_constBuffer.matProj = m_proj.Transpose();
	// ���[���h�s���]�u���ăR���X�^���g�o�b�t�@�Ɋi�[
	m_constBuffer.matWorld = m_world.Transpose();
	// �R���X�^���g�o�b�t�@�̍X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �R���X�^���g�o�b�t�@�̃Z�b�g
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	// �`��J�n
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// �V�F�[�_�[�̓o�^
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�[���
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief �G�t�F�N�g�̔j������
*	@details �G�t�F�N�g�̔j������
*	@param �Ȃ�
*	@return �j������Ȃ�true�A�j�����Ȃ��Ȃ�false
*/
bool DamageEffect::Destroy() const
{
	// �j�����Ԃ��߂�����j������
	if (m_time >= DESTROY_TIME)
	{
		return true;// �j�����Ԃ��߂�����true
	}
	else
	{
		// ����ȊO��false
		return false;
	}
}