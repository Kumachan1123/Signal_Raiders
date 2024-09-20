/*
	@file	Result.cpp
	@brief	���ʃN���X
*/
#include "pch.h"
#include "Result.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Game/KumachiLib//BinaryFile.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Result::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
Result::Result(CommonResources* resources)
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

Result::~Result()
{}

void Result::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}

void Result::Create(DX::DeviceResources* pDR, const wchar_t* path)
{
	m_pDR = pDR;

	ID3D11Device1* device = pDR->GetD3DDevice();
	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ���
	LoadTexture(path);
	// �Q�[���I�[�o�[�摜
	if (wcscmp(path, L"Resources/Textures/GameOver.png") == 0)
	{
		// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�
		m_ConstBuffer.Colors = SimpleMath::Vector4(0.15f, 0, 0.5f, 0);
		//	���_���(�|���S���̂S���_�̍��W���j
		VertexPositionTexture vertex[4] =
		{
			//	���_���													UV���
			VertexPositionTexture(SimpleMath::Vector3(-0.85f,  0.4f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.85f,  0.4f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.85f, -0.4f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-0.85f, -0.4f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
		};
		for (int i = 0; i < 4; i++)
		{
			m_vertex[i] = vertex[i];
		}
	}
	else // �N���A�摜
	{
		// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�
		m_ConstBuffer.Colors = SimpleMath::Vector4(0.5f, 0.5f, 0, 0);
		//	���_���(�|���S���̂S���_�̍��W���j
		VertexPositionTexture vertex[4] =
		{
			//	���_���													UV���
			VertexPositionTexture(SimpleMath::Vector3(-0.8f,  0.5f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.8f,  0.5f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(0.8f, -0.5f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-0.8f, -0.5f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
		};
		for (int i = 0; i < 4; i++)
		{
			m_vertex[i] = vertex[i];
		}
	}
	// ���_�̍��W�𒲐�
	for (int i = 0; i < 4; i++)
	{
		m_vertex[i].position.y += 0.25f;
	}
	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(device);
}

void Result::CreateShader()
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
}

void Result::Update(float elapsedTime)
{
	m_time += elapsedTime; // ���Ԃ��J�E���g
}

void Result::Render()
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
	m_ConstBuffer.time = m_time;
	//	�p�f�B���O
	m_ConstBuffer.padding = SimpleMath::Vector3(0, 0, 0);


	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_ConstBuffer, 0, 0);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->AnisotropicWrap() };
	context->PSSetSamplers(0, 1, sampler);
	// �������`��w��
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
	for (int i = 0; i < m_texture.size(); i++)
	{
		//	for���ň�C�ɐݒ肷��
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}
	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());
	//	�|���S����`��
	m_batch->Begin();
	m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[2], m_vertex[3]);
	m_batch->End();
	//	�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}