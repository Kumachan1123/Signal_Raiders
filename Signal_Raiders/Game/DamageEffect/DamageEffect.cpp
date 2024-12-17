//--------------------------------------------------------------------------------------
// File: DamageEffect.h
//
// �U�����󂯂����̉��o�N���X
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "DamageEffect.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
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

/// <summary>
/// �R���X�g���N�^
/// </summary>
DamageEffect::DamageEffect(CommonResources* resources)
	:m_pDR{ nullptr }
	, m_time{ 0.0f }
	, m_constBuffer{}
	, m_commonResources{ resources }
	, m_pPlayer{ nullptr }
	, m_enemyDirection{  }
	, m_playEffect{ false }
{
	// �F�̏�����
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DamageEffect::~DamageEffect()
{
	DrawPolygon::ReleasePositionTexture();
}

/// <summary>
/// �������֐�
/// </summary>
void DamageEffect::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_playEffect = true;
}

/// <summary>
/// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
/// </summary>
/// <param name="path">���΃p�X(Resources/Textures/�E�E�E.png�Ȃǁj</param>
void  DamageEffect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}

/// <summary>
/// �����֐�
/// </summary>
/// <param name="pDR">���[�U�[���\�[�X�����玝���Ă���</param>
void  DamageEffect::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ��݁i2���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/crisis.png");
	// �|���S���`��p
	DrawPolygon::InitializePositionTexture(m_pDR);
}


/// <summary>
/// Shader�쐬�����������������֐�
/// </summary>
void  DamageEffect::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/DamageEffect/VS_Damage.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/DamageEffect/PS_Damage.cso");

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	�s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);

	// �V�F�[�_�[�̍\���̂ɃZ�b�g
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}

// �X�V
void  DamageEffect::Update(float elapsedTime)
{

	if (m_time >= PLAY_TIME)
	{
		m_time = 0.0f;
		m_constBuffer.colors.w = 0.0f;
		m_playEffect = false;
		m_pPlayer->SetisPlayEffect(m_playEffect);
		return;
	}
	// ���ԍX�V
	m_time += elapsedTime;
	m_constBuffer.time = m_time;
	// �A���t�@�l(sin�֐����g���ē�b�ȓ���0����1����������)
	m_constBuffer.colors.w = 0.5f + 0.5f * sin(m_time * 2.0f);
	// �v���C���[�̌���
	m_playerDirection = m_pPlayer->GetPlayerDir();
	// �v���C���[�̌����ƓG�̌����̍�����0����360�̊Ԃ̊p�x�����߂�
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(m_playerDirection.x, m_playerDirection.z);
	// ���W�A������x�ɕϊ�
	angle = angle * 180 / DirectX::XM_PI;
	// 0����360�̊ԂɎ��߂�
	if (angle < 0)angle += 360;

	// �v���C���[�̌����ɂ����UV���W��ς���i16�����j
	if (angle >= 11.25 && angle < 33.75)           m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_H); // �E��1
	else if (angle >= 33.75 && angle < 56.25)      m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_W + UV_C) / 2, UV_H); // �E��2
	else if (angle >= 56.25 && angle < 78.75)      m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, (UV_H + UV_C) / 2); // �E��3
	else if (angle >= 78.75 && angle < 101.25)     m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_C); // �E
	else if (angle >= 101.25 && angle < 123.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, (UV_Y + UV_C) / 2); // �E�O1
	else if (angle >= 123.75 && angle < 146.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_W + UV_C) / 2, UV_Y); // �E�O2
	else if (angle >= 146.25 && angle < 168.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_Y); // �E�O3
	else if (angle >= 168.75 && angle < 191.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_C, UV_Y); // �O
	else if (angle >= 191.25 && angle < 213.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_Y); // ���O3
	else if (angle >= 213.75 && angle < 236.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_X + UV_C) / 2, UV_Y); // ���O2
	else if (angle >= 236.25 && angle < 258.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, (UV_Y + UV_C) / 2); // ���O1
	else if (angle >= 258.75 && angle < 281.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_C); // ��
	else if (angle >= 281.25 && angle < 303.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, (UV_H + UV_C) / 2); // ����1
	else if (angle >= 303.75 && angle < 326.25)    m_constBuffer.uv = DirectX::SimpleMath::Vector2((UV_X + UV_C) / 2, UV_H); // ����2
	else if (angle >= 326.25 && angle < 348.75)    m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_H); // ����3
	else                                           m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_C, UV_H); // ��
}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void  DamageEffect::Render()
{

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	// ���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		// ���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	DrawPolygon::UpdateSubResources(context, m_cBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	// �`�揀��
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_texture);
	// �V�F�[�_���Z�b�g����
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	// �|���S����`��
	DrawPolygon::DrawTexture(vertex);
	//	�V�F�[�_�̓o�^���������Ă���
	DrawPolygon::ReleaseShader(context);
}