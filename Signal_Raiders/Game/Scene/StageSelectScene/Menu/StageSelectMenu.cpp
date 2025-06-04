/*
*	@file	StageSelectMenu.cpp
*	@brief	���j���[�N���X
*/
#include <pch.h>
#include "StageSelectMenu.h"
// �����ȃ��j���[�C���f�b�N�X
const int StageSelectMenu::INVALID_MENU_INDEX = 6;
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
	, m_pCommonResources{ nullptr }// ���ʃ��\�[�X�ւ̃|�C���^
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
*	@details �X�e�[�W�Z���N�g���j���[�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
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
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �E�B���h�E��
	m_windowWidth = width;
	// �E�B���h�E����
	m_windowHeight = height;
	// �I��g�̃e�N�X�`���p�X�ݒ�
	m_pSelectTexturePath = L"Resources/Textures/StageSelect.png";
	//�u�X�e�[�W1�̎ʐ^�v��ǂݍ���
	Add("Stage1"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W2�̎ʐ^�v��ǂݍ���
	Add("Stage2"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W3�̎ʐ^�v��ǂݍ���
	Add("Stage3"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W4�̎ʐ^�v��ǂݍ���
	Add("Stage4"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�X�e�[�W5�̎ʐ^�v��ǂݍ���
	Add("Stage5"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u�^�C�g���ɖ߂�v��ǂݍ���
	Add("ToTitle"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//�u��������v��ǂݍ���
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
void StageSelectMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �}�E�X�g���b�J�[���擾����
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// �}�E�X�̌��݂̏�Ԃ��擾����
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// ���I����Ԃɂ���
	m_hit = false;
	// �}�E�X���W��2D�x�N�g���Ŏ擾����
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// UI�̊e�v�f�ɑ΂��ăq�b�g������s��
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// �}�E�X�̍��W��UI�ɏd�Ȃ��Ă��邩�m�F����
		if (m_pUI[i]->IsHit(mousePos))
		{
			// �q�b�g�t���O�𗧂Ă�
			m_hit = true;
			// �I�����j���[���O��ƈႤ�ꍇ�ASE�Đ��t���O�����Z�b�g
			if ((int(m_menuIndex)) != i)m_isSEPlay = false;
			// SE�����Đ��Ȃ�
			if (!m_isSEPlay)
			{
				// ���ʉ����Đ�����
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
				// SE�Đ��t���O�𗧂Ă�
				m_isSEPlay = true;
			}
			// ���ݑI������Ă��郁�j���[�̃C���f�b�N�X���X�V����
			m_menuIndex = i;
			// 1�ł��q�b�g�����瑼�͖�������i�I����1�����j
			break;
		}
	}
	// �o�ߎ��Ԃ����Z����
	m_time += elapsedTime;
	// �����q�b�g���Ă��Ȃ���΃C���f�b�N�X�𖳌��ɂ���
	if (!m_hit)	m_menuIndex = INVALID_MENU_INDEX;
	// ���N���b�N���ꂽ��I�����ꂽ���j���[�ɑΉ�����V�[��ID��ݒ肷��
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);
	// UI�v�f�̏�Ԃ��X�V���� 
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// �w�i�p�̑I���E�B���h�E�̃X�P�[�����X�V����
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		// �o�ߎ��Ԃ�n���ăA�j���i�s
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		// ���j���[�{�̂̃X�P�[�����X�V����
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		// �o�ߎ��Ԃ�n���ăA�j���i�s
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// ��I��Ώۂ�UI�̃A�j����Ԃ��X�V����
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		// �X�P�[���X�V����
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		// �o�ߎ��Ԃ��X�V����
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	// �I�����ꂽ���j���[���Ȃ��ꍇ�͉������Ȃ��Ŗ߂�
	if (m_menuIndex == INVALID_MENU_INDEX)	return;
	// �I�����ꂽUI�̌��̃X�P�[�����擾����
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	// �X�P�[���̕��
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);
	// ���Ԃɉ�����sin�g�ŕω�����X�P�[���𐶐�����i�I�𒆂̉��o�j
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);
	// UI�{�̂ɕω��X�P�[���𔽉f����
	m_pUI[m_menuIndex]->SetScale(select);
	// �w�i�E�B���h�E�ɂ����l�̃X�P�[����K�p����
	m_pSelect[m_menuIndex]->SetScale(select);
}
/*
*	@brief ���j���[��`�悷��
*	@details �X�e�[�W�Z���N�g���j���[�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectMenu::Render()
{
	// UI�̐������J��Ԃ�
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// �V�F�[�_�[�^�C�v��ݒ�
		m_pUI[i]->SetShaderType(UI::ShaderType::STAGE_SELECT);
		// �`��
		m_pUI[i]->Render();
	}
	// �I��s�\�ȃA�C�e���̉摜��\��
	for (unsigned int i = 0; i < m_pGuide.size(); i++)
	{
		// �V�F�[�_�[�^�C�v��ݒ�
		m_pGuide[i]->SetShaderType(UI::ShaderType::NORMAL);
		// �`��
		m_pGuide[i]->Render();
	}
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
void StageSelectMenu::Add(const std::string& key,
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
	// UI�̎�ނɉ����ď����𕪊�
	if (type == UIType::SELECT)// �I���\�ȃA�C�e���Ȃ�
	{
		// �A�C�e����V�����ǉ�		
		m_pUI.push_back(std::move(userInterface));
		// �w�i�p�̃E�B���h�E�摜���ǉ�����
		std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);
		// �w��摜��UI�쐬
		base->Create(m_pDR, "StageSelect", position, scale, anchor);
		// �E�B���h�E�T�C�Y��ݒ�
		base->SetWindowSize(m_windowWidth, m_windowHeight);
		// �w�i�p�̃A�C�e�����V�����ǉ�����
		m_pSelect.push_back(std::move(base));
	}
	else// �I��s�ȃA�C�e���Ȃ�
	{
		// �I��s�\�ȃA�C�e����ǉ�
		m_pGuide.push_back(std::move(userInterface));
		return;
	}
}
