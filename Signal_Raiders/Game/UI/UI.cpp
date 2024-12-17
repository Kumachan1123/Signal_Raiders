/*
	@file	UI.cpp
	@brief	UI�N���X(�^�C�g���E���U���g�E�Z�b�e�B���O)
*/
#include "pch.h"
#include "UI.h"
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
	, m_anchor{ kumachi::ANCHOR::TOP_LEFT }
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
	, kumachi::ANCHOR anchor)
{
	m_pDR = pDR;// �f�o�C�X���\�[�X
	m_position = position;// �ʒu
	m_baseScale = m_scale = scale;// �X�P�[��
	m_anchor = anchor;// �A���J�[
	//	�V�F�[�_�[�̍쐬
	CreateShader();
	//	�e�N�X�`����ǂݍ���
	LoadTexture(path);
	// �|���S���`��p
	DrawPolygon::InitializePositionColorTexture(m_pDR);

}

void UI::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();// �f�o�C�X
	// �R���p�C�����ꂽ�V�F�[�_�[��ǂݍ���
	kumachi::BinaryFile VS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Menu/VS_Menu.cso");
	kumachi::BinaryFile GS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Menu/GS_Menu.cso");
	kumachi::BinaryFile PS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Menu/PS_Menu.cso");
	// �C���v�b�g���C�A�E�g�쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VS.GetData(), VS.GetSize(),
		m_pInputLayout.GetAddressOf());
	//	���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, m_pVertexShader.ReleaseAndGetAddressOf())))
	{//	�G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	�W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, m_pGeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	//	�s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, m_pPixelShader.ReleaseAndGetAddressOf())))
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
	device->CreateBuffer(&bd, nullptr, &m_pCBuffer);
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	m_shaders.gs = m_pGeometryShader.Get();
	m_shaders.ps = m_pPixelShader.Get();
}

void UI::Update(float elapsedTime)
{
	m_time += elapsedTime;

}

void UI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pDR->GetD3DDeviceContext();// �R���e�L�X�g
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
	DrawPolygon::UpdateSubResources(context, m_pCBuffer.Get(), &m_constBuffer);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	// �`�揀��
	DrawPolygon::DrawStartColorTexture(context, m_pInputLayout.Get(), m_pTextures);
	//	�V�F�[�_���Z�b�g����
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	//	�|���S����`��
	DrawPolygon::DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//	�V�F�[�_�̓o�^���������Ă���
	DrawPolygon::ReleaseShader(context);
}

void UI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
