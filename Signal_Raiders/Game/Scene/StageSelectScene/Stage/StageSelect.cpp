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
	m_pInputLayout{},
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
	m_ConstBuffer{},
	m_pDrawPolygon{ DrawPolygon::GetInstance() },
	m_pCreateShader{ CreateShader::GetInstance() }
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}

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
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}

void StageSelect::CreateShader()
{
	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
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
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_ConstBuffer);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	//	�V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	�e�N�X�`���`��J�n
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(m_vertex);
	//	�V�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}