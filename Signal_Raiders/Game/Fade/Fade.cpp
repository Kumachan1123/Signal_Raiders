#include "pch.h"
#include "Fade.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/KumachiLib/KumachiLib.h"
#include "Libraries/MyLib/DebugString.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �t�F�[�h�ŏ��l
const float Fade::FADE_MIN = -1.01f;
// �t�F�[�h�ő�l
const float Fade::FADE_MAX = 1.01f;
// �t�F�[�h���x
const float Fade::FADE_SPEED = 1.5f;


// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
// �R���X�g���N�^
Fade::Fade(CommonResources* commonResources)
	:
	m_pDR{ },
	m_CBuffer{},
	m_pInputLayout{},
	m_commonResources{ commonResources },
	m_batch{},
	m_states{},
	m_texture{},
	m_texture2{},
	m_vertexShader{},
	m_pixelShader{},
	m_world{},
	m_view{},
	m_proj{},
	m_time{ -0.75f },
	m_fadeState{ FadeState::None },
	m_fadeTexNum{ },
	m_pDrawPolygon{ DrawPolygon::GetInstance() },
	m_pCreateShader{ CreateShader::GetInstance() }
{
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
// �e�N�X�`���̓ǂݍ���
void Fade::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}
// ����
void Fade::Create(DX::DeviceResources* pDR)
{
	// �f�o�C�X���\�[�X�̎擾
	m_pDR = pDR;
	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ���
	LoadTexture(L"Resources/Textures/fade2.png");//fadeTex
	LoadTexture(L"Resources/Textures/Ready.png");//readyTex
	LoadTexture(L"Resources/Textures/Go.png");//goTex
	LoadTexture(L"Resources/Textures/Black.png");//backTex
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
// �V�F�[�_�[�쐬����
void Fade::CreateShader()
{
	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Fade/VS_Fade.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Fade/PS_Fade.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}
// �X�V
void Fade::Update(float elapsedTime)
{
	// �t�F�[�h�C��
	if (m_fadeState == FadeState::FadeIn)
	{
		// ���Ԃ����Z
		m_time += elapsedTime * FADE_SPEED;
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);
		if (m_time >= FADE_MAX)
		{
			m_fadeState = FadeState::FadeInEnd;
		}
	}
	// �t�F�[�h�A�E�g
	if (m_fadeState == FadeState::FadeOut)
	{
		// ���Ԃ����Z
		m_time -= elapsedTime * FADE_SPEED;
		m_time = Clamp(m_time, FADE_MIN, FADE_MAX);
		if (m_time <= FADE_MIN)
		{
			m_fadeState = FadeState::FadeOutEnd;
		}
	}
}

// �`��
void Fade::Render()
{
	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0, -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	ConstBuffer cbuff;
	cbuff.power = 0.01f;// �t�F�[�h�ƃf�B�]���u�̐ݒ�
	cbuff.fadeAmount = m_time;
	cbuff.num = m_fadeTexNum;	// �摜�ԍ�
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &cbuff);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	//	�`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	//	�V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}