/*
	@file	UI.cpp
	@brief	UI�N���X(�^�C�g���E���U���g�E�Z�b�e�B���O)
*/
#include "pch.h"
#include "UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
using namespace DirectX;
const std::vector<D3D11_INPUT_ELEMENT_DESC> UI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UI::UI()
	: m_pDR{ nullptr }
	, m_time{ 0 }
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_textureWidth{ 0 }
	, m_textureHeight{ 0 }
	, m_scale{ DirectX::SimpleMath::Vector2::One }
	, m_baseScale{ DirectX::SimpleMath::Vector2::One }
	, m_position{ DirectX::SimpleMath::Vector2::Zero }
	, m_anchor{ KumachiLib::ANCHOR::TOP_LEFT }
	, m_shaderType{ ShaderType::NORMAL }
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
}

UI::~UI()
{
}

void UI::LoadTexture(const wchar_t* path)
{
	//	�w�肳�ꂽ�摜��ǂݍ���
	HRESULT result = DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, m_pTextureResource.ReleaseAndGetAddressOf(), m_pTexture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_pTextureResource.As(&tex));
	//	�ǂݍ��񂾉摜�̏����擾����
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	m_pTextures.push_back(m_pTexture);
	//	�ǂݍ��񂾉摜�̃T�C�Y���擾����
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;
	UNREFERENCED_PARAMETER(result);
}

void UI::Create(DX::DeviceResources* pDR, const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor)
{
	m_pDR = pDR;// �f�o�C�X���\�[�X
	m_position = position;// �ʒu
	m_baseScale = m_scale = scale;// �X�P�[��
	m_anchor = anchor;// �A���J�[
	//	�V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	//	�V�F�[�_�[�̍쐬
	CreateShader();
	//	�e�N�X�`����ǂݍ���
	LoadTexture(path);
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);

}

// �}�E�X�Ƃ̓����蔻��
bool UI::IsHit(const DirectX::SimpleMath::Vector2& pos) const
{
	//	�摜�̍���̍��W���擾
	DirectX::SimpleMath::Vector2 leftTop = m_position - DirectX::SimpleMath::Vector2(float(m_textureWidth) / 2.0f, float(m_textureHeight) / 2.0f);
	//	�摜�̉E���̍��W���擾
	DirectX::SimpleMath::Vector2 rightBottom = m_position + DirectX::SimpleMath::Vector2(float(m_textureWidth) / 2.0f, float(m_textureHeight) / 2.0f);
	//	�摜�̍���̍��W�ƉE���̍��W���r���āA�}�E�X�̍��W���͈͓��ɂ��邩�𔻒肷��
	if (leftTop.x <= pos.x && pos.x <= rightBottom.x
		&& leftTop.y <= pos.y && pos.y <= rightBottom.y)
	{
		return true;
	}
	return false;

}

void UI::CreateShader()
{
	// �V�F�[�_�[���쐬����
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Menu/VS_Menu.cso", m_pVertexShader);
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Menu/GS_Menu.cso", m_pGeometryShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_Menu.cso", m_pPixelShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_StageMenu.cso", m_pPixelShaderStageSelect);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	m_shaders.gs = m_pGeometryShader.Get();
	m_shaders.ps = m_pPixelShader.Get();
	m_StageSelectShaders.vs = m_pVertexShader.Get();
	m_StageSelectShaders.gs = m_pGeometryShader.Get();
	m_StageSelectShaders.ps = m_pPixelShaderStageSelect.Get();
}

void UI::Update(float elapsedTime)
{
	m_time += elapsedTime;

}

void UI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))
		, SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, SimpleMath::Vector2(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)))
	};
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	m_constBuffer.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	m_constBuffer.time = m_time;
	m_constBuffer.color = Vector3(0.5, 0.5, 0.5);

	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// �`��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	//	�V�F�[�_���Z�b�g����
	if (m_shaderType == ShaderType::NORMAL)
		m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	else
		m_pDrawPolygon->SetShader(m_StageSelectShaders, nullptr, 0);

	//	�|���S����`��
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

void UI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
