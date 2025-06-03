/*
*	@file   ResultMenu.cpp
*   @brief  ���U���g��ʂ̃��j���[���Ǘ�����N���X
*/
#include <pch.h>
#include "ResultMenu.h"

// �����ȃ��j���[�C���f�b�N�X
const int ResultMenu::INVALID_MENU_INDEX = 6;// �����ȃ��j���[�C���f�b�N�X

/*
*	@brief �R���X�g���N�^
*	@details ���U���g���j���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ResultMenu::ResultMenu()
	: m_menuIndex{ 0 }						// ���ݑI�𒆂̃��j���[�C���f�b�N�X
	, m_pDR{ nullptr }						// �f�o�C�X���\�[�X�ւ̃|�C���^
	, m_pCommonResources{ nullptr }			// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pUI{}								// UI�I�u�W�F�N�g���X�g
	, m_pSelect{}							// �I�����ꂽ���ɕ\������w�iUI���X�g
	, m_pSelectTexturePath{ nullptr }		// �I��w�i�̃e�N�X�`���p�X
	, m_windowWidth{ 0 }					// �E�B���h�E��
	, m_windowHeight{ 0 }					// �E�B���h�E����
	, m_num{ SceneID::REPLAY }				// �I�΂ꂽ�V�[���i������REPLAY�j
	, m_hit{ false }						// �}�E�X��UI�Ƀq�b�g�������ǂ���
	, m_time{ 0.0f }						// ����
	, m_SEVolume{ 0.0f }					// SE����
	, m_isSEPlay{ false }					// SE�Đ��t���O
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���U���g���j���[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ResultMenu::~ResultMenu()
{
	//  UI�I�u�W�F�N�g�̉��
	for (auto& ui : m_pUI)ui.reset();
	m_pUI.clear();
	//  �I�����ꂽ���ɕ\������w�iUI�I�u�W�F�N�g�̉��
	for (auto& select : m_pSelect)select.reset();
	m_pSelect.clear();
	//  �I��s�\�ȃA�C�e���̉��
	for (auto& guide : m_pGuide)guide.reset();
	m_pGuide.clear();
}
/*
	@brief ���j���[������������
	@details ���j���[�A�C�e���̉摜��ǂݍ��݁A�ʒu�E�X�P�[���E�A���J�[��ݒ肷��
	@param resources ���ʃ��\�[�X�ւ̃|�C���^
	@param width �E�B���h�E�̕�
	@param height �E�B���h�E�̍���
	@return �Ȃ�
*/
void ResultMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// ���ʃ��\�[�X���Z�b�g
	m_pDR = m_pCommonResources->GetDeviceResources();// �f�o�C�X���\�[�X�擾
	m_windowWidth = width;// �E�B���h�E���ۑ�
	m_windowHeight = height;// �E�B���h�E�����ۑ�
	m_pSelectTexturePath = L"Resources/Textures/ResultSelect.png";// �I��g�̃e�N�X�`���p�X�ݒ�
	//  �u���������������v��ǂݍ���
	Add("RePlay"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// �@�u�X�e�[�W������ԁv��ǂݍ���
	Add("ToStageSelect"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 400)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// �u��������v��ǂݍ���
	Add("Guide"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);
}
/*
	@brief ���j���[���X�V����
	@details �}�E�X�ʒu�ɉ��������j���[�̑I����ԍX�V�ƃN���b�N������s��
	@param elapsedTime �O�t���[������̌o�ߎ���(�b)
	@return �Ȃ�
*/
void ResultMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker(); // �}�E�X�̃g���b�J�[���擾����
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();// �}�E�X�̏�Ԃ��擾
	m_time += elapsedTime;// ���Ԃ����Z
	m_hit = false; // �����Ƀq�b�g�������ǂ�����������
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));// �}�E�X�̍��W���擾
	for (int i = 0; i < m_pUI.size(); i++)// ���j���[�A�C�e���̐������J��Ԃ�
	{
		if (m_pUI[i]->IsHit(mousePos))// �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		{
			m_hit = true;	// �q�b�g�t���O�𗧂Ă�
			if ((int(m_menuIndex)) != i) m_isSEPlay = false;// �O��I���������j���[�ƈႤ�ꍇ��SE���Đ�����t���O�𗧂Ă�
			if (!m_isSEPlay)// SE���Đ�����Ă��Ȃ��ꍇ
			{
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);// SE�̍Đ�
				m_isSEPlay = true;// �Đ��t���O�𗧂Ă�
			}
			m_menuIndex = i;	// �q�b�g�������j���[�̃C���f�b�N�X��ۑ�
			break;	// �q�b�g�����烋�[�v�𔲂���
		}
	}
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;// �q�b�g�����Ȃ�I���C���f�b�N�X�𖳌��l�ɐݒ�
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);// ���N���b�N���ꂽ��I�����j���[�̃V�[��ID���X�V
	for (int i = 0; i < m_pUI.size(); i++)//  ���j���[�A�C�e���̑I�����X�V
	{
		// �A�C�e���̑I����Ԃ��X�V
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());// �I����Ԃ̃X�P�[�����擾
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);// ���Ԃ����Z
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale()); // �I����Ԃ̃X�P�[�����擾
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);// ���Ԃ����Z
	}

	for (int i = 0; i < m_pGuide.size(); i++)// �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	{
		// �A�C�e���̑I����Ԃ��X�V
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());// �I����Ԃ̃X�P�[�����擾
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);	// ���Ԃ����Z
	}
	if (!m_hit)return;// �q�b�g���Ă��Ȃ���Ή������Ȃ�
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// �I�𒆂̏����T�C�Y���擾����
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// �I�𒆃��j���[�̃X�P�[�������Ԍo�߂ɉ����ėh�炷�i�g�̂悤��0.1�{�����j
	m_pUI[m_menuIndex]->SetScale(select);// �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);// �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
}
/*
	@brief ���j���[��`�悷��
	@details ���j���[�A�C�e���̉摜�Ɣw�i�g��`�悷��
	@param �Ȃ�
	@return �Ȃ�
*/
void ResultMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)// ���j���[�A�C�e���̐������J��Ԃ�
	{
		if (i == m_menuIndex) m_pSelect[i]->Render(); // �A�C�e���p�E�B���h�E�w�i��\��
		m_pUI[i]->Render();// ���ۂɕ\���������A�C�e���摜��\��
	}
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();// �I��s�\�ȃA�C�e���̉摜��\��
}
/*
	@brief ���j���[�A�C�e����ǉ�����
	@details �w��̉摜��UI���쐬���A�������X�g�ɒǉ����� (�I���Ȃ�w�i�g���쐬)
	@param key �摜�t�@�C���̃L�[
	@param position �`��ʒu
	@param scale �`��X�P�[��
	@param anchor �A���J�[�w��
	@param type ���j���[�̎�ށi�I����/�s�j
	@return �Ȃ�
*/
void ResultMenu::Add(std::string key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);// UI�I�u�W�F�N�g�̐���
	userInterface->Create(m_pDR, key, position, scale, anchor);// �w��摜��UI�쐬
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�T�C�Y��ݒ�
	if (type == UIType::SELECT)// �I���\�ȃA�C�e���Ȃ�
	{
		m_pUI.push_back(std::move(userInterface));// �A�C�e����V�����ǉ�
		std::unique_ptr<UI> back = std::make_unique<UI>(m_pCommonResources);// �w�i�p�̑I��g���ǉ�����
		back->Create(m_pDR, "ResultSelect", position, scale, anchor);// �w��摜��UI�쐬
		back->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�T�C�Y��ݒ�
		m_pSelect.push_back(std::move(back)); // �w�i�p�̃A�C�e�����V�����ǉ�����
	}
	else// �I��s�ȃA�C�e���Ȃ�
	{
		m_pGuide.push_back(std::move(userInterface));//  �I��s�\�ȃA�C�e����ǉ�
		return;
	}
}
