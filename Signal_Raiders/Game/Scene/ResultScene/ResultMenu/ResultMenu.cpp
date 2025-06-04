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
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �E�B���h�E���ۑ�
	m_windowWidth = width;
	// �E�B���h�E�����ۑ�
	m_windowHeight = height;
	// �u���������������v��ǂݍ���
	Add("RePlay"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// �u�X�e�[�W������ԁv��ǂݍ���
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
*	@brief ���j���[���X�V����
*	@details �}�E�X�ʒu�ɉ��������j���[�̑I����ԍX�V�ƃN���b�N������s��
*	@param elapsedTime �O�t���[������̌o�ߎ���(�b)
*	@return �Ȃ�
*/
void ResultMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// ���Ԃ����Z
	m_time += elapsedTime;
	// �����Ƀq�b�g�������ǂ�����������
	m_hit = false;
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// ���j���[�A�C�e���̐������J��Ԃ�
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		if (m_pUI[i]->IsHit(mousePos))
		{
			// �q�b�g�t���O�𗧂Ă�
			m_hit = true;
			// �O��I���������j���[�ƈႤ�ꍇ��SE���Đ�����t���O�𗧂Ă�
			if ((int(m_menuIndex)) != i) m_isSEPlay = false;
			// SE���Đ�����Ă��Ȃ��ꍇ
			if (!m_isSEPlay)
			{
				// �I�����̍Đ�
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
				// �Đ��t���O�𗧂Ă�
				m_isSEPlay = true;
			}
			// �q�b�g�������j���[�̃C���f�b�N�X��ۑ�
			m_menuIndex = i;
			// �q�b�g�����烋�[�v�𔲂���
			break;
		}
	}
	// �q�b�g�����Ȃ�I���C���f�b�N�X�𖳌��l�ɐݒ�
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;
	// ���N���b�N���ꂽ��I�����j���[�̃V�[��ID���X�V
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);
	//  ���j���[�A�C�e���̑I�����X�V
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// �A�C�e���̑I����Ԃ��X�V
		// �I����Ԃ̃X�P�[�����擾
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		// ���Ԃ����Z
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		// �I����Ԃ̃X�P�[�����擾
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		// ���Ԃ����Z
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}

	// �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		// �A�C�e���̑I����Ԃ��X�V
		// �I����Ԃ̃X�P�[�����擾
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		// ���Ԃ����Z
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	// �q�b�g���Ă��Ȃ���Ή������Ȃ�
	if (!m_hit)return;
	// �I�𒆂̏����T�C�Y���擾����
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	// �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);
	// �I�𒆃��j���[�̃X�P�[�������Ԍo�߂ɉ����ėh�炷�i�g�̂悤��0.1�{�����j
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);
	// �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
	m_pUI[m_menuIndex]->SetScale(select);
	// �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);
}
/*
	@brief ���j���[��`�悷��
	@details ���j���[�A�C�e���̉摜�Ɣw�i�g��`�悷��
	@param �Ȃ�
	@return �Ȃ�
*/
void ResultMenu::Render()
{
	// ���j���[�A�C�e���̐������J��Ԃ�
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// �A�C�e���p�E�B���h�E�w�i��\��
		if (i == m_menuIndex) m_pSelect[i]->Render();
		// ���ۂɕ\���������A�C�e���摜��\��
		m_pUI[i]->Render();
	}
	// �I��s�\�ȃA�C�e���̉摜��\��
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();
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
void ResultMenu::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	// UI�I�u�W�F�N�g�̐���
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);
	// �w��摜��UI�쐬
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// �E�B���h�E�T�C�Y��ݒ�
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UI�̃^�C�v�ɉ����ď����𕪊�
	if (type == UIType::SELECT)// �I���\�ȃA�C�e���Ȃ�
	{
		// �A�C�e����V�����ǉ�
		m_pUI.push_back(std::move(userInterface));
		// �w�i�p�̑I��g���ǉ�����
		std::unique_ptr<UI> back = std::make_unique<UI>(m_pCommonResources);
		// �w��摜��UI�쐬
		back->Create(m_pDR, "ResultSelect", position, scale, anchor);
		// �E�B���h�E�T�C�Y��ݒ�
		back->SetWindowSize(m_windowWidth, m_windowHeight);
		// �w�i�p�̃A�C�e�����V�����ǉ�����
		m_pSelect.push_back(std::move(back));
	}
	else// �I��s�ȃA�C�e���Ȃ�
	{
		// �I��s�\�ȃA�C�e����ǉ�
		m_pGuide.push_back(std::move(userInterface));
		return;
	}
}