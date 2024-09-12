/*
	@file	PressKey.cpp
	@brief	�w���N���X
*/
#include "pch.h"
#include "PressKey.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
using namespace DirectX;
using namespace DirectX::SimpleMath;

PressKey::PressKey(CommonResources* commonResources)
	:m_pressKeyTexture{}
	, m_pressKeyTexCenter{}
	, m_spriteBatch{}
	, m_commonResources{ commonResources }
{}

PressKey::~PressKey()
{}

void PressKey::Initialize()
{
	// �f�o�C�X���\�[�X�̎擾
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Space.png",
			nullptr,
			m_pressKeyTexture.ReleaseAndGetAddressOf()
		)
	);
	// �R�����X�e�[�g
	m_states = std::make_unique<CommonStates>(device);
	// �e�N�X�`���̍쐬
	CreateTexture();
}

// �X�V
void PressKey::Update(float elapsedTime)
{
	m_time += elapsedTime;
	m_size = (sin(m_time) * 0.1f) + 1.0f;
}

// �`��
void PressKey::Render()
{
	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	// �^�C�g�����S�̕`��ʒu�����߂�
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// �摜�̒��S���v�Z����
	Vector2 spacePos{ rect.right / 2.0f, rect.bottom / 1.2f };
	// �`�悷��
	m_spriteBatch->Draw(
		m_pressKeyTexture.Get(),	// �e�N�X�`��(SRV)
		spacePos,				// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,			// ��`(RECT)
		Colors::White,		// �w�i�F
		0.0f,				// ��]�p(���W�A��)
		m_pressKeyTexCenter,// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		m_size,				// �X�P�[��(scale)
		SpriteEffects_None,	// �G�t�F�N�g(effects)
		0.0f				// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);
	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}

// �e�N�X�`���̍쐬
void PressKey::CreateTexture()
{
	// �ꎞ�I�ȕϐ��̐錾
	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D{};
	D3D11_TEXTURE2D_DESC desc{};
	Vector2 texSize{};
	// �e�N�X�`���̏����擾����================================
	// �e�N�X�`����ID3D11Resource�Ƃ��Č���
	m_pressKeyTexture->GetResource(resource.GetAddressOf());
	// ID3D11Resource��ID3D11Texture2D�Ƃ��Č���
	resource.As(&tex2D);
	//// �e�N�X�`���̒��S�ʒu���v�Z����
	tex2D->GetDesc(&desc);
	// �e�N�X�`���T�C�Y���擾���Afloat�^�ɕϊ�����
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	m_pressKeyTexCenter = texSize / 2.0f;
}