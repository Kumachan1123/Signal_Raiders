/*
	@file	TitleLogo.cpp
	@brief	�^�C�g�����S�N���X
*/
#include "pch.h"
#include "TitleLogo.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"



using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC>  TitleLogo::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
TitleLogo::TitleLogo(CommonResources* resources)
	:
	m_commonResources{ resources },
	m_vertexShader{},
	m_pixelShader{},
	m_pInputLayout{},
	m_pDR{},
	m_CBuffer{},
	m_batch{},
	m_states{},
	m_titleTexture{},
	m_time{},
	m_world{},
	m_view{},
	m_proj{},
	m_ConstBuffer{},
	m_pDrawPolygon{ DrawPolygon::GetInstance() },
	m_pCreateShader{ CreateShader::GetInstance() }
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}

TitleLogo::~TitleLogo()
{
}

void TitleLogo::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_titleTexture.push_back(texture);
}

void TitleLogo::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ��݁i2���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/Title.png");
	// �|���S���̕`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}

void TitleLogo::CreateShader()
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

void TitleLogo::Update(float elapsedTime)
{
	m_time += elapsedTime; // ���Ԃ��J�E���g
}

void TitleLogo::Render()
{
	//	�|���`�揈��
	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-0.85f,  0.75f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.85f,  0.75f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.85f, -0.75f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.85f, -0.75f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	// �r���[�ݒ�
	m_ConstBuffer.matView = m_view.Transpose();
	// �v���W�F�N�V�����ݒ�
	m_ConstBuffer.matProj = m_proj.Transpose();
	// ���[���h�ݒ�
	m_ConstBuffer.matWorld = m_world.Transpose();
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ� 
	m_ConstBuffer.Colors = SimpleMath::Vector4(0.25f, 0.75f, 0.75f, 0);
	// ���Ԑݒ�
	m_ConstBuffer.time = SimpleMath::Vector4(m_time);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(context, m_CBuffer.Get(), &m_ConstBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(context, 0, 1, cb);
	// �`�揀��
	m_pDrawPolygon->DrawStart(context, m_pInputLayout.Get(), m_titleTexture);
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShader(context, m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader(context);
}