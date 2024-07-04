//--------------------------------------------------------------------------------------
//@file    PlayerUI.cpp
//@brief  �v���C���[UI�N���X
//�쐬�ҁF���܂�
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
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

/// <summary>
/// �C���v�b�g���C�A�E�g
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerUI::PlayerUI()
	:m_pDR(nullptr)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_texture(nullptr)
	, m_res(nullptr)
	, m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_anchor(kumachi::ANCHOR::TOP_LEFT)
	, m_renderRatio(1.1f)
	, m_renderRatioOffset(0.0f)
{

}
PlayerUI::~PlayerUI() {}
/// <summary>
/// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
/// </summary>
/// <param name="path">���΃p�X(Resources/Textures/�E�E�E.png�Ȃǁj</param>
void PlayerUI::LoadTexture(const wchar_t* path)
{

	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, m_res.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;

}

/// <summary>
/// �����֐�
/// </summary>
/// <param name="pDR">���[�U�[���\�[�X�����玝���Ă���</param>
void PlayerUI::Create(DX::DeviceResources* pDR
					  , const wchar_t* path
					  , DirectX::SimpleMath::Vector2 position
					  , DirectX::SimpleMath::Vector2 scale
					  , kumachi::ANCHOR anchor)
{
	m_pDR = pDR;
	auto device = pDR->GetD3DDevice();
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;

	//	�V�F�[�_�[�̍쐬
	CreateShader();

	//	�摜�̓ǂݍ���
	LoadTexture(path);

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

}

void PlayerUI::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	m_scale = scale;
}
void PlayerUI::SetPosition(DirectX::SimpleMath::Vector2 position)
{
	m_position = position;
}
void PlayerUI::SetAnchor(kumachi::ANCHOR anchor)
{
	m_anchor = anchor;
}
void PlayerUI::SetRenderRatio(float ratio)
{
	m_renderRatio = ratio;
}
void PlayerUI::SetRenderRatioOffset(float offset)
{
	m_renderRatioOffset = offset;
}
/// <summary>
/// Shader�쐬�����������������֐�
/// </summary>
void PlayerUI::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/PlayerHP/VS_PlayerHP.cso");
	kumachi::BinaryFile GSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/PlayerHP/GS_PlayerHP.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/PlayerHP/PS_PlayerHP.cso");

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

	//	�W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
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
}




/// <summary>
/// �`��֐�
/// </summary>
void PlayerUI::Render()
{
	auto context = m_pDR->GetD3DDeviceContext();
	//	���_���
	//	Position.xy	:�g�k�p�X�P�[��
	//	Position.z	:�A���J�[�^�C�v(0�`8)�̐����Ŏw��
	//	Color.xy�@	:�A���J�[���W(�s�N�Z���w��:1280 �~720)
	//	Color.zw	:�摜�T�C�Y
	//	Tex.xy		:x = �Q�[�W�摜�̕`��͈�(0�`1), y = 0
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(
			 SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))
			,SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
			,SimpleMath::Vector2(m_renderRatio - m_renderRatioOffset,0))
	};

	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	cbuff.renderRatio = m_renderRatio - m_renderRatioOffset;
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
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

	//	�J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	//	�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());


	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//	�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

void PlayerUI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}
