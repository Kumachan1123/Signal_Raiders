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
{
	// �F�̏�����
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DamageEffect::~DamageEffect()
{
}

/// <summary>
/// �������֐�
/// </summary>
void DamageEffect::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
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

	ID3D11Device1* device = pDR->GetD3DDevice();

	//	�V�F�[�_�[�̍쐬
	CreateShader();

	//	�摜�̓ǂݍ��݁i2���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/crisis.png");

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

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
}

//�X�V
void  DamageEffect::Update(float elapsedTime)
{
	// ���ԍX�V
	m_time += elapsedTime;
	// �U�����Ă����G�̂������
	m_enemyDirection = m_pPlayer->GetEnemyDir();
	// �v���C���[�̌���
	m_playerDirection = m_pPlayer->GetPlayerDir();
	// �v���C���[�̌����ƓG�̌����̍�����0����360�̊Ԃ̊p�x�����߂�
	float angle = atan2(m_enemyDirection.x, m_enemyDirection.z) - atan2(m_playerDirection.x, m_playerDirection.z);
	// ���W�A������x�ɕϊ�
	angle = angle * 180 / DirectX::XM_PI;
	// 0����360�̊ԂɎ��߂�
	if (angle < 0)
	{
		angle += 360;
	}
	// �v���C���[�̌����ɂ����UV���W��ς���(0=��,�O�A1=�E,��)
	// �p�x��22.5����67.5�̊Ԃ�������
	if (angle >= 22.5 && angle <= 67.5)			m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_H);// �E��
	// �p�x��67.5����112.5�̊Ԃ�������
	else if (angle > 67.5 && angle <= 112.5)	m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_C);// �E
	// �p�x��112.5����157.5�̊Ԃ�������
	else if (angle > 112.5 && angle <= 157.5)	m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_W, UV_Y);// �E�O
	// �p�x��157.5����202.5�̊Ԃ�������
	else if (angle > 157.5 && angle <= 202.5)	m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_C, UV_Y);// �O
	// �p�x��202.5����247.5�̊Ԃ�������
	else if (angle > 202.5 && angle <= 247.5)	m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_Y);// ���O
	// �p�x��247.5����292.5�̊Ԃ�������
	else if (angle > 247.5 && angle <= 292.5)	m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_C);// ��
	// �p�x��292.5����337.5�̊Ԃ�������
	else if (angle > 292.5 && angle <= 337.5)	m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_X, UV_H);// ����
	// �p�x��337.5����22.5�̊Ԃ�������
	else										m_constBuffer.uv = DirectX::SimpleMath::Vector2(UV_C, UV_H);// ��

}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void  DamageEffect::Render()
{

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE,  SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-SIZE_X * SCALE, -SIZE_Y * SCALE, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.time = m_time;
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &m_constBuffer, 0, 0);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	�������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	//	�J�����O�͂Ȃ�
	context->RSSetState(m_states->CullNone());

	//	�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	Create�֐��œǂݍ��񂾉摜���s�N�Z���V�F�[�_�ɓo�^����B
	//for (int i = 0; i < m_texture.size(); i++)
	//{
	//	//	for���ň�C�ɐݒ肷��
	//	context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	//}

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�|���S����`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	//	�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}