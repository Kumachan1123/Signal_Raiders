/*
*	@file	SettingScene.cpp
*	@brief	���j���[�N���X
*/
#include <pch.h>
#include "SettingScene.h"

// ���ʂ̊�l
const float SettingScene::VOLUME = 0.05f;
/*
*	@brief �R���X�g���N�^
*	@details �Z�b�e�B���O�V�[���N���X�̃R���X�g���N�^
*	@param sceneID �V�[��ID
*	@return �Ȃ�
*/
SettingScene::SettingScene(IScene::SceneID sceneID)
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_isChangeScene{ false }// �V�[���ύX�t���O
	, m_isFade{ false }// �t�F�[�h�t���O
	, m_BGMvolume{ VOLUME }// BGM����
	, m_SEvolume{ VOLUME }// SE����
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pFade{}// �t�F�[�h�N���X�̃|�C���^�[
	, m_fadeState{ }// �t�F�[�h�̏��
	, m_pBackGround{ nullptr }// �w�i�N���X�̃|�C���^�[
	, m_nowSceneID{ sceneID }// ���݂̃V�[��ID
	, m_pSettingMenu{}// ���j���[�N���X�̃|�C���^�[
	, m_pSettingBar{}// �ݒ�o�[�N���X�̃|�C���^�[
	, m_pSettingData{}// �ݒ�f�[�^�N���X�̃|�C���^�[
{}
/*
*	@brief �f�X�g���N�^
*	@details �Z�b�e�B���O�V�[���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SettingScene::~SettingScene()
{
	// ��n��
	Finalize();
}
/*
*	@brief ������
*	@details �Z�b�e�B���O�V�[���̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void SettingScene::Initialize(CommonResources* resources)
{
	// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	assert(resources);
	// ���ʃ��\�[�X�������o�ɃZ�b�g����
	m_pCommonResources = resources;
	// DirectX�p�̃f�o�C�X���\�[�X���擾����
	auto DR = m_pCommonResources->GetDeviceResources();
	// �t�F�[�h���o�𐶐�����
	m_pFade = std::make_unique<Fade>(m_pCommonResources);
	// �t�F�[�h�̏��������s��
	m_pFade->Initialize();
	// �ŏ��̓t�F�[�h�C����Ԃɐݒ肵�Ă���
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// �w�i�`��p�̃N���X�𐶐�����
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);
	// �w�i�̃��f����e�N�X�`���Ȃǂ�����������
	m_pBackGround->Create(DR);
	// �ݒ�f�[�^���Ǘ�����N���X���쐬����
	m_pSettingData = std::make_unique<SettingData>();
	// �ݒ�t�@�C����ǂݍ���Œl���擾����
	m_pSettingData->Load();
	// BGM���ʂ�ݒ�t�@�C������ǂݍ��݁A��������
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	// SE���ʂ����l�ɐݒ�t�@�C������擾���Ē�������
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// �ݒ��ʂ̃��j���[UI�𐶐�����
	m_pSettingMenu = std::make_unique<SettingMenu>();
	// ���j���[�ɑ΂���SE�̉��ʂ�K�p����
	m_pSettingMenu->SetSEVolume(m_SEvolume);
	// �}�E�X�J�[�\���p�̃|�C���^�[UI���쐬����
	m_pMousePointer = std::make_unique<MousePointer>();
	// ���ʃo�[�Ȃǂ�UI���܂ރZ�b�e�B���O�o�[���쐬����
	m_pSettingBar = std::make_unique<SettingBar>();
	// �Z�b�e�B���O�o�[�ɃZ�b�e�B���O���j���[���֘A�t����
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());
	// �Z�b�e�B���O���j���[��UI���X�g�ɒǉ�����
	m_pUI.push_back(std::move(m_pSettingMenu));
	// �Z�b�e�B���O�o�[��UI���X�g�ɒǉ�����
	m_pUI.push_back(std::move(m_pSettingBar));
	// �}�E�X�|�C���^�[��UI���X�g�ɒǉ�����
	m_pUI.push_back(std::move(m_pMousePointer));
	// UI���ׂĂɑ΂��ď������������s��
	for (int it = 0; it < m_pUI.size(); ++it)
		m_pUI[it]->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
}
/*
*	@brief �X�V
*	@details �Z�b�e�B���O�V�[���̍X�V���s��
*	@param elapsedTime �t���[������
*	@return �Ȃ�
*/
void SettingScene::Update(float elapsedTime)
{
	// �e��UI�ɓn�������܂Ƃ߂��\����
	UpdateContext ctx;
	// �g��Ȃ�
	ctx.bulletPoint = 0;
	// �g��Ȃ�
	ctx.dashStamina = 0;
	//�t���[������
	ctx.elapsedTime = elapsedTime;
	//�g��Ȃ�
	ctx.playerHP = 0;
	// ���j���[�̍X�V
	UpdateSettingBars(ctx);
	// �t�F�[�h��Ԃƃ}�E�X�̍X�V
	UpdateFadeAndMouse(ctx);
	// �w�i�ƃt�F�[�h�̍X�V
	UpdateBackgroundAndFade(elapsedTime);
}
/*
*	@brief �`��
*	@details �Z�b�e�B���O�V�[���̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::Render()
{
	// �w�i�̕`��
	m_pBackGround->Render();
	// �t�F�[�h�C�����I�������
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// UI�̕`��
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();
	}
	// �t�F�[�h�̕`��
	m_pFade->Render();
}
/*
*	@brief �I������
*	@details �Z�b�e�B���O�V�[���̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::Finalize()
{
	// ���ʂ̐ݒ�
	SetVolume();
}
/*
*	@brief ���̃V�[��ID���擾
*	@details �Z�b�e�B���O�V�[���̎��̃V�[��ID���擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
IScene::SceneID SettingScene::GetNextSceneID() const
{

	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		// BGM�̒�~
		m_pCommonResources->GetAudioManager()->StopSound("TitleBGM");
		// ���艹�̒�~
		m_pCommonResources->GetAudioManager()->StopSound("SE");
		// �I�����̒�~
		m_pCommonResources->GetAudioManager()->StopSound("Select");
		// �^�C�g���V�[���Ɉڍs
		return IScene::SceneID::TITLE;
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}
/*
*	@brief ���ʂ�ݒ肷��
*	@details BGM��SE�̉��ʂ�ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::SetVolume()
{
	// UI�̐���������
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		// �ݒ�o�[�̃|�C���^�[���擾
		if (auto pSettingBar = dynamic_cast<SettingBar*>(m_pUI[it].get()))
		{
			// ���ʂ̎擾(10����1�Ŋ������擾)
			// BGM
			float BGMvolume = static_cast<float>(pSettingBar->GetSetting(0)) * 0.1f;
			// SE
			float SEvolume = static_cast<float>(pSettingBar->GetSetting(1)) * 0.1f;
			// �ݒ�̕ύX
			// BGM
			m_pSettingData->SetBGMVolume(pSettingBar->GetSetting(0));
			// SE
			m_pSettingData->SetSEVolume(pSettingBar->GetSetting(1));
			// ���ʂ̐ݒ�
			//	BGM����
			m_BGMvolume = VOLUME * BGMvolume;
			// SE����
			m_SEvolume = VOLUME * SEvolume;
			return;
		}
	}
}
/*
*	@brief �ݒ�o�[�̍X�V
*	@details �ݒ�o�[�̍X�V���s��
*	@param ctx �X�V���
*	@return �Ȃ�
*/
void SettingScene::UpdateSettingBars(const UpdateContext& ctx)
{
	// SettingMenu�����X�g�A�b�v
	std::vector<SettingMenu*> settingMenus;
	// UI�̐������J��Ԃ�
	for (auto& settingMenu : m_pUI)
	{
		// SettingMenu�̃|�C���^�[���擾���A����������o�^
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(settingMenu.get()))settingMenus.push_back(pSettingMenu);
	}
	// SettingBar�����X�g�A�b�v���A���j���[�����Z�b�g
	for (auto& settingBar : m_pUI)
	{
		// SettingBar�̃|�C���^�[���擾
		if (auto pSettingBar = dynamic_cast<SettingBar*>(settingBar.get()))
		{
			// SettingMenu�̐������J��Ԃ��A���j���[�̃C���f�b�N�X���擾
			for (auto& menu : settingMenus)
				pSettingBar->SetStateIDNum(static_cast<SettingMenu::SettingID>(menu->GetMenuIndex()));
		}
		// UI�̍X�V
		settingBar->Update(ctx);
	}
}
/*
*	@brief �t�F�[�h�ƃ}�E�X�̍X�V
*	@details �t�F�[�h�ƃ}�E�X�̍X�V���s��
*	@param ctx �X�V���
*	@return �Ȃ�
*/
void SettingScene::UpdateFadeAndMouse(const UpdateContext& ctx)
{
	// �I�[�f�B�I�}�l�[�W���[�̍X�V����
	m_pCommonResources->GetAudioManager()->Update();
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// �t�F�[�h�̏�Ԃ��m�F���A�t�F�[�h�C�����������Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;
	// ���N���b�N���ꂽ��
	if (mtracker->GetLastState().leftButton)
	{
		// ���j���[�I���̏������s��
		HandleMenuSelection(ctx);
	}
	else// ���N���b�N����Ă��Ȃ��ꍇ
	{
		// �}�E�X�|�C���^�[�̍X�V
		UpdateMousePointers(ctx);
	}
}
/*
*	@brief �w�i�ƃt�F�[�h�̍X�V
*	@details �w�i�ƃt�F�[�h�̍X�V���s��
*	@param elapsedTime �t���[������
*	@return �Ȃ�
*/
void SettingScene::UpdateBackgroundAndFade(float elapsedTime)
{
	// �t�F�[�h�A�E�g���������Ă�����V�[���ύX�t���O�𗧂Ă�
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
	// BGM�̍Đ�
	m_pCommonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);
	// �w�i�̍X�V
	m_pBackGround->Update(elapsedTime);
	// �t�F�[�h�̍X�V
	m_pFade->Update(elapsedTime);
}
/*
*	@brief �}�E�X�|�C���^�[�̍X�V
*	@details �}�E�X�|�C���^�[�̍X�V���s��
*	@param ctx �X�V���
*	@return �Ȃ�
*/
void SettingScene::UpdateMousePointers(const UpdateContext& ctx)
{
	// UI�̐������J��Ԃ�
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		// �}�E�X�|�C���^�[�̃|�C���^�[���擾
		if (auto pMousePointer = dynamic_cast<MousePointer*>(m_pUI[it].get()))
		{
			// �}�E�X�|�C���^�[�̍X�V
			pMousePointer->Update(ctx);
		}
	}
}
/*
*	@brief ���j���[�I���̏���
*	@details ���j���[�I���̏������s��
*	@param ctx �X�V���
*	@return �Ȃ�
*/
void SettingScene::HandleMenuSelection(const UpdateContext& ctx)
{
	// UI�̐������J��Ԃ�
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		// SettingMenu�̃|�C���^�[���擾		
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(m_pUI[it].get()))
		{
			// �I�����ꂽ���j���[��ID���擾
			auto selectID = pSettingMenu->GetSelectIDNum();
			// �u�I���v���u�K�p�v���I�΂���
			if (selectID == SettingMenu::SelectID::END ||
				selectID == SettingMenu::SelectID::APPLY)
			{
				// SE�̍Đ�
				m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
				// �t�F�[�h�A�E�g�Ɉڍs
				m_pFade->SetState(Fade::FadeState::FadeOut);
				// ��������UI�͌��Ȃ��Ă����̂ł��̏������I����
				return;
			}
			// �}�E�X�|�C���^�[�̍X�V
			UpdateMousePointers(ctx);
		}
	}
}
