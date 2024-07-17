/*
	@file	TitleScene.cpp
	@brief	�^�C�g���V�[���N���X
*/
#include "pch.h"
#include "TitleScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
// FMOD�̃C���N���[�h
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include <Libraries/Microsoft/DebugDraw.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
TitleScene::TitleScene()
	:
	m_commonResources{},
	m_spriteBatch{},
	m_spriteFont{},
	m_titleTexture{},
	m_pressKeyTexture{},
	m_pressKeyTexCenter{},
	m_titleTexCenter{},
	m_isChangeScene{},
	m_system{ nullptr },
	m_soundSE{ nullptr },
	m_soundBGM{ nullptr },
	m_channelSE{ nullptr },
	m_channelBGM{ nullptr },
	m_isFade{},
	m_volume{},
	m_counter{}
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
TitleScene::~TitleScene()
{
	// do nothing.
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void TitleScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �X�v���C�g�t�H���g���쐬����
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(
		device,
		L"Resources/Fonts/SegoeUI_18.spritefont"
	);

	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Space.png",
			nullptr,
			m_pressKeyTexture.ReleaseAndGetAddressOf()
		)
	);

	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Title.png",
			nullptr,
			m_titleTexture.ReleaseAndGetAddressOf()
		)
	);

	/*
		�ȉ��A�e�N�X�`���̑傫�������߂遨�e�N�X�`���̒��S���W���v�Z����
	*/
	// �ꎞ�I�ȕϐ��̐錾
	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};
	Microsoft::WRL::ComPtr<ID3D11Resource> resource2{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D2{};
	D3D11_TEXTURE2D_DESC desc{};
	D3D11_TEXTURE2D_DESC desc2{};
	Vector2 texSize{};
	Vector2 texSize2{};

	// �e�N�X�`���̏����擾����================================
	// �e�N�X�`����ID3D11Resource�Ƃ��Č���
	m_titleTexture->GetResource(resource.GetAddressOf());
	m_pressKeyTexture->GetResource(resource2.GetAddressOf());
	// ID3D11Resource��ID3D11Texture2D�Ƃ��Č���
	resource.As(&tex2D);
	resource2.As(&tex2D2);

	// �e�N�X�`�������擾����
	tex2D->GetDesc(&desc);

	// �e�N�X�`���T�C�Y���擾���Afloat�^�ɕϊ�����
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);

	// �e�N�X�`���̒��S�ʒu���v�Z����
	m_titleTexCenter = texSize / 2.0f;
	tex2D2->GetDesc(&desc2);

	// �e�N�X�`���T�C�Y���擾���Afloat�^�ɕϊ�����
	texSize2.x = static_cast<float>(desc2.Width);
	texSize2.y = static_cast<float>(desc2.Height);
	m_pressKeyTexCenter = texSize2 / 2.0f;

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	// Sound�p�̃I�u�W�F�N�g������������
	InitializeFMOD();

	// �t�F�[�h�Ɋւ��鏀��
	m_isFade = false;
	m_volume = 1.0f;
	m_counter = 0;
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void TitleScene::Update(float elapsedTime)
{
	// �錾���������A���ۂ͎g�p���Ă��Ȃ��ϐ�
	UNREFERENCED_PARAMETER(elapsedTime);
	FMOD_RESULT result;
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
		result = m_system->playSound(m_soundSE, nullptr, false, &m_channelSE);
		assert(result == FMOD_OK);
		m_isChangeScene = true;
	}


	// ��d�Đ����Ȃ�
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
	}
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void TitleScene::Render()
{
	auto states = m_commonResources->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// �^�C�g�����S�̕`��ʒu�����߂�
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// �摜�̒��S���v�Z����
	Vector2 titlePos{ rect.right / 2.0f, rect.bottom / 2.0f };

	// �^�C�g�����S��`�悷��
	m_spriteBatch->Draw(
		m_titleTexture.Get(),	// �e�N�X�`��(SRV)
		titlePos,				// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,			// ��`(RECT)
		Colors::White,		// �w�i�F
		0.0f,				// ��]�p(���W�A��)
		m_titleTexCenter,		// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,				// �X�P�[��(scale)
		SpriteEffects_None,	// �G�t�F�N�g(effects)
		0.0f				// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);
	// �摜�̒��S���v�Z����
	Vector2 spacePos{ rect.right / 2.0f, rect.bottom / 1.2f };

	// �^�C�g�����S��`�悷��
	m_spriteBatch->Draw(
		m_pressKeyTexture.Get(),	// �e�N�X�`��(SRV)
		spacePos,				// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,			// ��`(RECT)
		Colors::White,		// �w�i�F
		0.0f,				// ��]�p(���W�A��)
		m_pressKeyTexCenter,		// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,				// �X�P�[��(scale)
		SpriteEffects_None,	// �G�t�F�N�g(effects)
		0.0f				// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);
#ifdef _DEBUG
	// �����ɃX�v���C�g�t�H���g�ŕ������`�悷����@
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Title Scene", Vector2(10, 40));

	wchar_t buf[32];
	swprintf_s(buf, 32, L"right : %d, bottom : %d", rect.right, rect.bottom);
	m_spriteFont->DrawString(m_spriteBatch.get(), buf, Vector2(10, 70));
#endif
	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void TitleScene::Finalize()
{
	// do nothing.
		// Sound�p�̃I�u�W�F�N�g���������
	m_soundSE->release();
	m_soundBGM->release();
	m_system->release();
}

//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		m_channelBGM->stop();
		return IScene::SceneID::PLAY;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}

//---------------------------------------------------------
// FMOD�̃V�X�e���̏������Ɖ����f�[�^�̃��[�h
//---------------------------------------------------------
void TitleScene::InitializeFMOD()
{
	// �V�X�e�����C���X�^���X������
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// �V�X�e��������������
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SE�����[�h����
	result = m_system->createSound("Resources/Sounds/select.mp3", FMOD_DEFAULT, nullptr, &m_soundSE);
	assert(result == FMOD_OK);

	// BGM�����[�h����
	result = m_system->createSound("Resources/Sounds/title.mp3", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);
}
