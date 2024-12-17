#include "pch.h"
#include "Fade.h"
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
	m_inputLayout{},
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
	m_fadeTexNum{ }
{
	// do nothing.
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
	DrawPolygon::InitializePositionTexture(m_pDR);
}
// �V�F�[�_�[�쐬����
void Fade::CreateShader()
{
	// �f�o�C�X���\�[�X�̎擾
	auto device = m_pDR->GetD3DDevice();
	// ���_�V�F�[�_�[�̍쐬
	kumachi::BinaryFile VSFade = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Fade/VS_Fade.cso");
	// �s�N�Z���V�F�[�_�[�̍쐬
	kumachi::BinaryFile PSFade = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Fade/PS_Fade.cso");
	// �C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSFade.GetData(), VSFade.GetSize(),
		m_inputLayout.GetAddressOf());
	// ���_�V�F�[�_�[�̍쐬
	if (FAILED(device->CreateVertexShader(VSFade.GetData(), VSFade.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
	{
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�[�̍쐬
	if (FAILED(device->CreatePixelShader(PSFade.GetData(), PSFade.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
	{
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
	DrawPolygon::UpdateSubResources(context, m_CBuffer.Get(), &cbuff);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	//	�`�揀��
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_texture);
	//	�V�F�[�_���Z�b�g����
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	//	�|���S����`��
	DrawPolygon::DrawTexture(vertex);
	//	�V�F�[�_�̓o�^���������Ă���
	DrawPolygon::ReleaseShader(context);
}