/*
*	@file	TitleMenu.cpp
*	@brief	�^�C�g�����j���[�N���X
*/
#include "pch.h"
#include "TitleMenu.h"



TitleMenu::TitleMenu()
	: m_menuIndex{ 0 }
	, m_pDR{ nullptr }
	, m_commonResources{ nullptr }
	, m_pUI{}
	, m_pSelect{}
	, m_pSelectTexturePath{ nullptr }
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_time{ 0.0f }
	, m_hit{ false }
	, m_num{ SceneID::STAGESELECT }
{
}


TitleMenu::~TitleMenu()
{
}

void TitleMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSelectTexturePath = L"Resources/Textures/select.png";
	//  �u�v���C�v��ǂݍ���
	Add(L"Resources/Textures/play.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u�����Ă��v��ǂݍ���
	Add(L"Resources/Textures/setting.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 350)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u�����v��ǂݍ���
	Add(L"Resources/Textures/end.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 450)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u��������v��ǂݍ���
	Add(L"Resources/Textures/Guide.png"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);

}

void TitleMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();
	m_hit = false;// �����Ƀq�b�g�������ǂ���
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	//  ���j���[�A�C�e���̐������J��Ԃ�
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		if (m_pUI[i]->IsHit(mousePos))
		{
			//  �͈͓��ɂ���ꍇ�́A�I�𒆂̃A�C�e�����X�V
			m_menuIndex = i;
			m_hit = true;
			break;
		}
	}
	// ��������q�b�g���ĂȂ�������A�I���Ȃ��ɂ���
	if (!m_hit) { m_menuIndex = 6; }

	m_time += elapsedTime;

	if (kbTracker->pressed.Space || mtracker->GetLastState().leftButton)
	{
		m_num = static_cast<SceneID>(m_menuIndex);
	}
	//  ���j���[�A�C�e���̑I�����X�V
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  �A�C�e���̑I����Ԃ��X�V
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	if (!m_hit && m_menuIndex == 6)// �������ĂȂ�������I��
		return;
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// �I�𒆂̏����T�C�Y���擾����
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// �I����Ԃ͏�����ԁ{50���̑傫���Ƃ���
	m_pUI[m_menuIndex]->SetScale(select);// �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);// �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
}
/*
*	@brief	�`��
*	@detail	���j���[�A�C�e����`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)// �o�^����UI�̐����[�v
	{
		if (i == m_menuIndex) m_pSelect[i]->Render();//  �A�C�e���p�E�B���h�E�w�i��\��
		m_pUI[i]->Render();	//  ���ۂɕ\���������A�C�e���摜��\��
	}
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();// �I��s�\�ȃA�C�e����\��
#ifdef _DEBUG

	auto debugString = m_commonResources->GetDebugString(); // �f�o�b�O����\������
	for (int i = 0; i < m_transforms.size(); i++)// �o�^����UI�̐����[�v
		debugString->AddString("Transform.Pos:%f,%f  Scale:%f,%f", m_transforms[i].position.x, m_transforms[i].position.y, m_transforms[i].scale.x, m_transforms[i].scale.y);//  UI�̍��W��\��

#endif
}
/*
*	@brief	�A�C�e����ǉ�����
*	@detail	�A�C�e����ǉ�����
*	@param path		�A�C�e���̃p�X
*	@param position	�A�C�e���̈ʒu
*	@param scale	�A�C�e���̃X�P�[��
*	@param anchor	�A�C�e���̃A���J�[
*	@param type		�A�C�e���̎��
*	@return �Ȃ�
*/
void TitleMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();// ���j���[�Ƃ��ăA�C�e����ǉ�����
	userInterface->Create(m_pDR, path, position, scale, anchor);// �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y��ݒ肷��
	if (type == UIType::SELECT)
	{
		m_pUI.push_back(std::move(userInterface));//  �I���\�A�C�e����V�����ǉ�

		std::unique_ptr<UI> base = std::make_unique<UI>();//  �w�i�p�̃E�B���h�E�摜���ǉ�����
		base->Create(m_pDR, m_pSelectTexturePath, position, scale, anchor);// �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
		base->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y��ݒ肷��
		m_pSelect.push_back(std::move(base));// �w�i�p�̃A�C�e�����V�����ǉ�����
		m_transforms.push_back({ position, scale });// UI�̏���z��ɓo�^
	}
	else
	{
		m_pGuide.push_back(std::move(userInterface));// �I��s�\�A�C�e����V�����ǉ�
		return;
	}
}
