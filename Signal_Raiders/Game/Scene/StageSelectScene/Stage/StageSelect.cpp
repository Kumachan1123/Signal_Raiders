/*
	@file	StageSelect.cpp
	@brief	�X�e�[�W�I���N���X
*/
#include "pch.h"
#include "StageSelect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC>  StageSelect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
StageSelect::StageSelect(CommonResources* resources)
	:
	m_commonResources{ resources },
	m_vertexShader{},
	m_pixelShader{},
	m_inputLayout{},
	m_pDR{},
	m_CBuffer{},
	m_batch{},
	m_states{},
	m_texture{},
	m_time{},
	m_world{},
	m_view{},
	m_proj{},
	m_vertex{},
	m_ConstBuffer{}
{}

StageSelect::~StageSelect()
{}

void StageSelect::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}

void StageSelect::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ���
	LoadTexture(L"Resources/Textures/StageSelectTex.png");
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�
	m_ConstBuffer.color = SimpleMath::Vector4(.25, .25, .25, 0);
	//	���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(0.85f, -0.6f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.85f, -0.6f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.85f,  0.f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.85f,  0.f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),

	};
	for (int i = 0; i < 4; i++)
	{
		m_vertex[i] = vertex[i];
	}
	// ���_�̍��W�𒲐�
	for (int i = 0; i < 4; i++)
	{
		m_vertex[i].position.y += 1.0f;
	}
	// �|���S���`��p
	DrawPolygon::InitializePositionTexture(m_pDR);
}

void StageSelect::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();
	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ��݁i�^�C�g���摜�̃V�F�[�_�[���g���܂킷�j
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/VS_Title.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/PS_Title.cso");
	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), VSData.GetData(), VSData.GetSize(), m_inputLayout.GetAddressOf());
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
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}

void StageSelect::Update(float elapsedTime)
{
	m_time += elapsedTime; // ���Ԃ��J�E���g
}

void StageSelect::Render()
{
	//	�|���`�揈��
	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	�r���[�ݒ�
	m_ConstBuffer.matView = m_view.Transpose();
	//	�v���W�F�N�V�����ݒ�
	m_ConstBuffer.matProj = m_proj.Transpose();
	//	���[���h�ݒ�
	m_ConstBuffer.matWorld = m_world.Transpose();
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ��Create�֐��Őݒ�ς�
	// ���Ԑݒ�
	m_ConstBuffer.time = SimpleMath::Vector4(m_time);
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	DrawPolygon::UpdateSubResources(context, m_CBuffer.Get(), &m_ConstBuffer);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	//	�V�F�[�_���Z�b�g����
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	//	�e�N�X�`���`��J�n
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_texture);
	//	�|���S����`��
	DrawPolygon::DrawTexture(m_vertex);
	//	�V�F�[�_�̓o�^���������Ă���
	DrawPolygon::ReleaseShader(context);
}