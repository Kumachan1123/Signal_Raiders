/*
*	@file	StageSelectMenu.cpp
*	@brief	���j���[�N���X
*/
#include "pch.h"
#include "StageSelectMenu.h"

// �����ȃ��j���[�C���f�b�N�X
const int StageSelectMenu::INVALID_MENU_INDEX = 6;// �����ȃ��j���[�C���f�b�N�X
/*
*	@brief �R���X�g���N�^
*	@details �X�e�[�W�Z���N�g���j���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*
*/
StageSelectMenu::StageSelectMenu()
	: m_menuIndex{ 0 }// ���ݑI�𒆂̃��j���[�C���f�b�N�X
	, m_pDR{ nullptr }// �f�o�C�X���\�[�X�ւ̃|�C���^
	, m_commonResources{ nullptr }// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pUI{}// UI
	, m_pSelect{}// �I�����ꂽ���ɕ\������w�iUI
	, m_pSelectTexturePath{ nullptr }// �I��w�i�̃e�N�X�`���p�X
	, m_windowWidth{ 0 }// �E�B���h�E��
	, m_windowHeight{ 0 }// �E�B���h�E����
	, m_num{ SceneID::PLAY }// �V�[��ID
	, m_time{ 0.0f }// ����
	, m_hit{ false }// �q�b�g�t���O
	, m_SEVolume{ 0.0f }// SE����
	, m_isSEPlay{ false }// SE�Đ��t���O
{
}
/*
*	@brief �f�X�g���N�^
*	@details �X�e�[�W�Z���N�g���j���[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
StageSelectMenu::~StageSelectMenu() {/*do nothing*/ }
/*
*	@brief ���j���[������������
*	@details ���j���[�A�C�e���̉摜��ǂݍ��݁A�ʒu�E�X�P�[���E�A���J�[��ݒ肷��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void StageSelectMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;// ���ʃ��\�[�X���Z�b�g
	m_pDR = m_commonResources->GetDeviceResources();// �f�o�C�X���\�[�X�擾
	m_windowWidth = width;// �E�B���h�E��
	m_windowHeight = height;// �E�B���h�E����
	m_pSelectTexturePath = L"Resources/Textures/StageSelect.png";// �I��g�̃e�N�X�`���p�X�ݒ�
	//�u�X�e�[�W1�̎ʐ^�v��ǂݍ���
	Add(L"Resources/Textures/stage1.png"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W2�̎ʐ^�v��ǂݍ���
	Add(L"Resources/Textures/stage2.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W3�̎ʐ^�v��ǂݍ���
	Add(L"Resources/Textures/stage3.png"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W4�̎ʐ^�v��ǂݍ���
	Add(L"Resources/Textures/stage4.png"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W5�̎ʐ^�v��ǂݍ���
	Add(L"Resources/Textures/stage5.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�^�C�g���ɖ߂�v��ǂݍ���
	Add(L"Resources/Textures/ToTitle.png"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u��������v��ǂݍ���
	Add(L"Resources/Textures/Guide.png"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);
}
/*
*	@brief ���j���[���X�V����
*	@details �}�E�X�ʒu�ɉ��������j���[�̑I����ԍX�V�ƃN���b�N������s��
*	@param elapsedTime �O�t���[������̌o�ߎ���(�b)
*	@return �Ȃ�
*/
void StageSelectMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();// �L�[�{�[�h�̃g���b�J�[���擾����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();// �}�E�X�̃g���b�J�[���擾����
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();// �}�E�X�̏�Ԃ��擾
	m_hit = false;// �����Ƀq�b�g������
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));// �}�E�X�̍��W���擾
	for (int i = 0; i < m_pUI.size(); i++)// ���j���[�A�C�e���̐������J��Ԃ�
	{
		if (m_pUI[i]->IsHit(mousePos))// �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		{
			m_hit = true;	// �q�b�g�t���O�𗧂Ă�
			if ((int(m_menuIndex)) != i) m_isSEPlay = false;// �O��I���������j���[�ƈႤ�ꍇ��SE���Đ�����t���O�𗧂Ă�
			if (!m_isSEPlay)// SE���Đ�����Ă��Ȃ��ꍇ
			{
				m_commonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);// SE�̍Đ�
				m_isSEPlay = true;// �Đ��t���O�𗧂Ă�
			}
			m_menuIndex = i;// �q�b�g�������j���[�̃C���f�b�N�X��ۑ�
			break;	// �q�b�g�����烋�[�v�𔲂���
		}
	}
	m_time += elapsedTime;// ���Ԃ����Z����
	if (!m_hit)m_menuIndex = INVALID_MENU_INDEX;// ��������q�b�g���ĂȂ�������I���Ȃ��ɂ���
	if (m_hit && kbTracker->pressed.Space || mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);// ���N���b�N���ꂽ��I�����j���[�̃V�[��ID���X�V
	for (int i = 0; i < m_pUI.size(); i++)// ���j���[�A�C�e���̑I�����X�V
	{
		//  �A�C�e���̑I����Ԃ��X�V
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());// �I����Ԃ̃X�P�[�����擾
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);// �A�C�e���̑I����Ԃ��X�V
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());// �I����Ԃ̃X�P�[�����擾
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);// �A�C�e���̑I����Ԃ��X�V
	}
	for (int i = 0; i < m_pGuide.size(); i++)// �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());// �X�P�[�����擾
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);// ���Ԃ����Z
	}
	if (m_menuIndex == INVALID_MENU_INDEX)return;// �Ȃɂ��I������Ă��Ȃ��ꍇ�͉������Ȃ�
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// �I�𒆂̏����T�C�Y���擾����
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// �I����Ԃ͏�����Ԃ��傫������
	m_pUI[m_menuIndex]->SetScale(select);// �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
	m_pSelect[m_menuIndex]->SetScale(select);// �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
}
/*
*	@brief ���j���[��`�悷��
*	@details �X�e�[�W�Z���N�g���j���[�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)// UI�̐������J��Ԃ�
	{
		m_pUI[i]->SetShaderType(UI::ShaderType::STAGE_SELECT);// �V�F�[�_�[�^�C�v��ݒ�
		m_pUI[i]->Render();	// �`��
	}
	for (unsigned int i = 0; i < m_pGuide.size(); i++)// �I��s�\�ȃA�C�e���̉摜��\��
	{
		m_pGuide[i]->SetShaderType(UI::ShaderType::NORMAL);// �V�F�[�_�[�^�C�v��ݒ�
		m_pGuide[i]->Render();// �`��
	}
}
/*
	@brief ���j���[�A�C�e����ǉ�����
	@details �w��̉摜��UI���쐬���A�������X�g�ɒǉ����� (�I���Ȃ�w�i�g���쐬)
	@param path �摜�t�@�C���̃p�X
	@param position �`��ʒu
	@param scale �`��X�P�[��
	@param anchor �A���J�[�w��
	@param type ���j���[�̎�ށi�I����/�s�j
	@return �Ȃ�
*/
void StageSelectMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();// UI�I�u�W�F�N�g�̐���
	userInterface->Create(m_pDR, path, position, scale, anchor);// �w��摜��UI�쐬
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�T�C�Y��ݒ�
	if (type == UIType::SELECT)// �I���\�ȃA�C�e���Ȃ�
	{
		m_pUI.push_back(std::move(userInterface));// �A�C�e����V�����ǉ�		
		std::unique_ptr<UI> base = std::make_unique<UI>();// �w�i�p�̃E�B���h�E�摜���ǉ�����
		base->Create(m_pDR, m_pSelectTexturePath, position, scale, anchor);// �w��摜��UI�쐬
		base->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�T�C�Y��ݒ�
		m_pSelect.push_back(std::move(base));// �w�i�p�̃A�C�e�����V�����ǉ�����
	}
	else// �I��s�ȃA�C�e���Ȃ�
	{
		m_pGuide.push_back(std::move(userInterface));// �I��s�\�ȃA�C�e����ǉ�
		return;
	}
}
