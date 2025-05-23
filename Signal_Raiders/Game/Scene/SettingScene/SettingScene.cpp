/*
*	@file	SettingScene.cpp
*	@brief	���j���[�N���X
*/
#include "pch.h"
#include "SettingScene.h"

const float SettingScene::VOLUME = 0.05f;// ���ʂ̊�l
/*
*	@brief �R���X�g���N�^
*	@details �Z�b�e�B���O�V�[���N���X�̃R���X�g���N�^
*	@param sceneID �V�[��ID
*	@return �Ȃ�
*/
SettingScene::SettingScene(IScene::SceneID sceneID)
	: m_commonResources{}// ���ʃ��\�[�X
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
	Finalize();// ��n��
}
/*
*	@brief ������
*	@details �Z�b�e�B���O�V�[���̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void SettingScene::Initialize(CommonResources* resources)
{
	assert(resources);// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	m_commonResources = resources;// ���ʃ��\�[�X���Z�b�g
	auto DR = m_commonResources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_pFade = std::make_unique<Fade>(m_commonResources);// �t�F�[�h�̐���
	m_pFade->Initialize();// �t�F�[�h�̏�����
	m_pFade->SetState(Fade::FadeState::FadeIn);// �t�F�[�h�C���Ɉڍs
	m_pBackGround = std::make_unique<BackGround>(m_commonResources);// �w�i���쐬����
	m_pBackGround->Create(DR);// �w�i�̏�����
	m_pSettingMenu = std::make_unique<SettingMenu>();// �Z�b�e�B���O���j���[���쐬
	m_pMousePointer = std::make_unique<MousePointer>();// �}�E�X�|�C���^�[���쐬
	m_pSettingBar = std::make_unique<SettingBar>();// �Z�b�e�B���O�o�[���쐬
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());// �Z�b�e�B���O���j���[���Z�b�g
	m_pUI.push_back(std::move(m_pSettingMenu));// ���j���[��UI�ɓo�^
	m_pUI.push_back(std::move(m_pSettingBar));// �Z�b�e�B���O�o�[��UI�ɓo�^
	m_pUI.push_back(std::move(m_pMousePointer));// �}�E�X�|�C���^�[��UI�ɓo�^
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);// UI�̏�����
	m_pSettingData = std::make_unique<SettingData>();// �ݒ�f�[�^�N���X�̍쐬
	m_pSettingData->Load();// �ݒ�t�@�C���̓ǂݍ���
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGM���ʂ�ݒ�
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SE���ʂ�ݒ�
}
/*
*	@brief �X�V
*	@details �Z�b�e�B���O�V�[���̍X�V���s��
*	@param elapsedTime �t���[������
*	@return �Ȃ�
*/
void SettingScene::Update(float elapsedTime)
{
	UpdateContext ctx;// �e��UI�ɓn�������܂Ƃ߂��\����
	ctx.bulletPoint = 0;// �g��Ȃ�
	ctx.dashStamina = 0;//�g��Ȃ�
	ctx.elapsedTime = elapsedTime;//�t���[������
	ctx.playerHP = 0;//�g��Ȃ�
	UpdateSettingBars(ctx);	// ���j���[�̍X�V
	UpdateFadeAndMouse(ctx);// �t�F�[�h��Ԃƃ}�E�X�̍X�V
	UpdateBackgroundAndFade(elapsedTime);// �w�i�ƃt�F�[�h�̍X�V
}
/*
*	@brief �`��
*	@details �Z�b�e�B���O�V�[���̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::Render()
{
	m_pBackGround->Render();// �w�i�̕`��
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)// �t�F�[�h�C�����I�������
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();// UI�̕`��
	m_pFade->Render();// �t�F�[�h�̕`��
}
/*
*	@brief �I������
*	@details �Z�b�e�B���O�V�[���̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::Finalize()
{
	SetVolume();// ���ʂ̐ݒ�
}
/*
*	@brief ���̃V�[��ID���擾
*	@details �Z�b�e�B���O�V�[���̎��̃V�[��ID���擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
IScene::SceneID SettingScene::GetNextSceneID() const
{

	if (m_isChangeScene)// �V�[���ύX������ꍇ
	{
		m_commonResources->GetAudioManager()->StopSound("TitleBGM");// BGM�̒�~
		m_commonResources->GetAudioManager()->StopSound("SE");// SE�̒�~
		m_commonResources->GetAudioManager()->StopSound("Select");// Select�̒�~
		return IScene::SceneID::TITLE;// �^�C�g���V�[���Ɉڍs
	}
	return IScene::SceneID::NONE;// �V�[���ύX���Ȃ��ꍇ
}
/*
*	@brief ���ʂ�ݒ肷��
*	@details BGM��SE�̉��ʂ�ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::SetVolume()
{
	for (int it = 0; it < m_pUI.size(); ++it)// UI�̐���������
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(m_pUI[it].get()))// �ݒ�o�[�̃|�C���^�[���擾
		{
			// ���ʂ̎擾(10����1�Ŋ������擾)
			float BGMvolume = static_cast<float>(pSettingBar->GetSetting(0)) * 0.1f;// BGM
			float SEvolume = static_cast<float>(pSettingBar->GetSetting(1)) * 0.1f;// SE
			// �ݒ�̕ύX
			m_pSettingData->SetBGMVolume(pSettingBar->GetSetting(0));// BGM
			m_pSettingData->SetSEVolume(pSettingBar->GetSetting(1));// SE
			// ���ʂ̐ݒ�
			m_BGMvolume = VOLUME * BGMvolume;//	BGM����
			m_SEvolume = VOLUME * SEvolume;// SE����
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
	std::vector<SettingMenu*> settingMenus;// SettingMenu�����X�g�A�b�v
	for (auto& settingMenu : m_pUI)// UI�̐������J��Ԃ�
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(settingMenu.get()))// SettingMenu�̃|�C���^�[���擾
			settingMenus.push_back(pSettingMenu);// SettingMenu�����X�g�A�b�v
	for (auto& settingBar : m_pUI)// SettingBar�����X�g�A�b�v���A���j���[�����Z�b�g
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(settingBar.get()))// SettingBar�̃|�C���^�[���擾
			for (auto& menu : settingMenus)// SettingMenu�̐������J��Ԃ�
				pSettingBar->SetStateIDNum(static_cast<SettingMenu::SettingID>(menu->GetMenuIndex()));// ���j���[�̃C���f�b�N�X���擾
		settingBar->Update(ctx);// UI�̍X�V
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
	m_commonResources->GetAudioManager()->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V����
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();// �}�E�X�̃g���b�J�[���擾����
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;// ���j���[�ł̑I���������s��ꂽ��
	if (mtracker->GetLastState().leftButton) HandleMenuSelection(ctx);// ���N���b�N���ꂽ�烁�j���[�I���������s��
	else UpdateMousePointers(ctx);// �}�E�X�|�C���^�[�̍X�V
}
/*
*	@brief �w�i�ƃt�F�[�h�̍X�V
*	@details �w�i�ƃt�F�[�h�̍X�V���s��
*	@param elapsedTime �t���[������
*	@return �Ȃ�
*/
void SettingScene::UpdateBackgroundAndFade(float elapsedTime)
{
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;// �V�[���ύX�t���O�𗧂Ă�
	m_commonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);// BGM�̍Đ�
	m_pBackGround->Update(elapsedTime);// �w�i�̍X�V
	m_pFade->Update(elapsedTime);// �t�F�[�h�̍X�V
}
/*
*	@brief �}�E�X�|�C���^�[�̍X�V
*	@details �}�E�X�|�C���^�[�̍X�V���s��
*	@param ctx �X�V���
*	@return �Ȃ�
*/
void SettingScene::UpdateMousePointers(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)// UI�̐������J��Ԃ�
		if (auto pMousePointer = dynamic_cast<MousePointer*>(m_pUI[it].get()))// �}�E�X�|�C���^�[�̃|�C���^�[���擾
			pMousePointer->Update(ctx);// �}�E�X�|�C���^�[�̍X�V
}
/*
*	@brief ���j���[�I���̏���
*	@details ���j���[�I���̏������s��
*	@param ctx �X�V���
*	@return �Ȃ�
*/
void SettingScene::HandleMenuSelection(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)// UI�̐������J��Ԃ�
	{
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(m_pUI[it].get()))// SettingMenu�̃|�C���^�[���擾		
		{
			auto selectID = pSettingMenu->GetSelectIDNum();// �I�����ꂽ���j���[��ID���擾
			if (selectID == SettingMenu::SelectID::END ||// �u�I���v��
				selectID == SettingMenu::SelectID::APPLY)// �u�K�p�v�����I�΂���
			{
				m_commonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);// SE�̍Đ�
				m_pFade->SetState(Fade::FadeState::FadeOut);// �t�F�[�h�A�E�g�Ɉڍs
				return;
			}
			UpdateMousePointers(ctx);// �}�E�X�|�C���^�[�̍X�V
		}
	}
}
