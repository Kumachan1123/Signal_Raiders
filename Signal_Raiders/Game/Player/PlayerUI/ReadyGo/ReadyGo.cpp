/*
	@file	ReadyGo.cpp
	@brief	�����N���X
*/
#include "pch.h"
#include "ReadyGo.h"
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
#include "Libraries/MyLib/DebugString.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
using namespace DirectX;

// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC>  ReadyGo::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};
// �R���X�g���N�^
ReadyGo::ReadyGo(CommonResources* resources)
	:m_pDR(nullptr)
	, m_time(0.0f)
	, m_constBuffer()
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_commonResources{ resources }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �F�̏�����
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

}

// �f�X�g���N�^
ReadyGo::~ReadyGo()
{
}

// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
void  ReadyGo::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture.push_back(texture);
}

// �����֐�
void  ReadyGo::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ��݁i2���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/crisis.png");
	LoadTexture(L"Resources/Textures/killAll.png");
	// �|���S���`��
	m_pDrawPolygon->InitializePositionTexture(m_pDR);

}

// �V�F�[�_�[�쐬����
void  ReadyGo::CreateShader()
{
	// ���_�V�F�[�_�[���s�N�Z���V�F�[�_�[���쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/ReadyGo/VS_ReadyGo.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/ReadyGo/PS_ReadyGo.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
}

//�X�V
void  ReadyGo::Update(float elapsedTime)
{
	//	���ԍX�V�im_time��0.1�����₵�A�P�𒴂�����O�����蒼���j
	m_time += elapsedTime;
	// �F�̍X�V
	// alpha�l��0.7f������������܂�0.9f�܂ŏ������グ��
	if (m_constBuffer.colors.w < 0.7f)
	{
		m_constBuffer.colors.w += 0.15f * elapsedTime;
	}
	else
	{
		// alpha�l��0.7f����0.9f�܂ł���������
		m_constBuffer.colors.w = 0.8f + 0.1f * sin(m_time);
	}
	// �F�̍X�V
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, m_constBuffer.colors.w);

}


// �`��֐�
void  ReadyGo::Render()
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
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.time = SimpleMath::Vector4(m_time);
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	//	�V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}