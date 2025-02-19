//--------------------------------------------------------------------------------------
// File: WifiLoading.cpp
//
// Wi-Fi���[�f�B���OUI�N���X
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "WifiLoading.h"

WifiLoading::WifiLoading()
	: m_windowHeight(0)
	, m_windowWidth(0)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_loading(nullptr)
	, m_loadgingText(nullptr)
	, m_animSpeed(1)// �A�j���[�V�����X�s�[�h
	, m_anim(0)// �A�j���[�V�����J�E���g
	, m_animTime(0)// �A�j���[�V��������
	, m_frameRows(1)// �摜�̍s��
	, m_frameCols(4)// �摜�̗�
{
}

WifiLoading::~WifiLoading()
{
}

void WifiLoading::Initialize(DX::DeviceResources* pDR, int width, int height)
{
	m_pDR = pDR;
	m_windowWidth = width;
	m_windowHeight = height;
	// ���[�f�B���O�摜
	Add(m_loading, L"Resources/Textures/Loading.png",
		DirectX::SimpleMath::Vector2(Screen::UI_RIGHT, Screen::UI_HEIGHT),
		DirectX::SimpleMath::Vector2(.5f, .5f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ���[�f�B���O�e�L�X�g
	Add(m_loadgingText, L"Resources/Textures/Loading_Text.png",
		DirectX::SimpleMath::Vector2(Screen::UI_RIGHT - 70, Screen::UI_HEIGHT),
		DirectX::SimpleMath::Vector2(.5f, .5f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ���[�f�B���O�摜�̃A�j���[�V�����ݒ�
	m_loading->SetFrameRows(m_frameRows);
	m_loading->SetFrameCols(m_frameCols);
}

void WifiLoading::Update(float elapsedTime)
{
	m_animTime += elapsedTime * m_animSpeed;// �o�ߎ��Ԃ����Z
	if (m_animTime > 1.25f)// 1�b�o��
	{
		m_animTime = 0.0f;// �^�C�}�[�����Z�b�g
	}

	// 0.25 ���݂ɒ���
	float stepRatio = floor(m_animTime * m_frameCols) / m_frameCols;
	m_loading->SetRenderRatio(stepRatio);
}

void WifiLoading::Render()
{
	m_loading->SetShaderType(PlayerUI::ShaderType::ANIM);
	m_loading->Render();
	m_loadgingText->SetShaderType(PlayerUI::ShaderType::OTHER);
	m_loadgingText->Render();
}

void WifiLoading::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();
	pPlayerUI->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}
