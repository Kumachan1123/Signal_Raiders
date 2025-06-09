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
	, m_BGMvolume{ VOLUME }// BGM����
	, m_SEvolume{ VOLUME }// SE����
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pFade{}// �t�F�[�h�N���X�̃|�C���^�[
	, m_fadeState{ }// �t�F�[�h�̏��
	, m_elapsedTime{ 0.0f }// �o�ߎ���
	, m_pBackGround{ nullptr }// �w�i�N���X�̃|�C���^�[
	, m_pMousePointer{ nullptr }// �}�E�X�|�C���^�[�N���X�̃|�C���^�[
	, m_pGameEndChecker{ nullptr }// �Q�[���I���O�m�F��ʂ̃|�C���^�[
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
	// ���j���[�̏��������s��
	m_pSettingMenu->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// �}�E�X�J�[�\���p�̃|�C���^�[UI���쐬����
	m_pMousePointer = std::make_unique<MousePointer>();
	// �}�E�X�|�C���^�[�̏��������s��
	m_pMousePointer->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// ���ʃo�[�Ȃǂ�UI���܂ރZ�b�e�B���O�o�[���쐬����
	m_pSettingBar = std::make_unique<SettingBar>();
	// �Z�b�e�B���O�o�[�ɃZ�b�e�B���O���j���[���֘A�t����
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());
	// �Z�b�e�B���O�o�[�̏��������s��
	m_pSettingBar->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// �Q�[���I���O�m�F�̂��߂̃N���X�𐶐�����
	m_pGameEndChecker = std::make_unique<GameEndChecker>();
	// �Q�[���I���O�m�F��SE�̉��ʂ�ݒ肷��
	m_pGameEndChecker->SetSEVolume(m_SEvolume);
	// �Q�[���I���O�m�F�̏��������s��
	m_pGameEndChecker->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
}
/*
*	@brief �X�V
*	@details �Z�b�e�B���O�V�[���̍X�V���s��
*	@param elapsedTime �t���[������
*	@return �Ȃ�
*/
void SettingScene::Update(float elapsedTime)
{
	// �t���[�����Ԃ�ۑ�����
	m_elapsedTime = elapsedTime;
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
	// �Q�[���I���O�̊m�F����
	if (m_pGameEndChecker->GetIsGameEndCheck())// �Q�[���I���O�̊m�F���L���ȏꍇ
	{
		// �Q�[���I���O�̊m�F�������s��
		UpdateCheckGameEnd();
	}
	else// �Q�[���I���O�̊m�F�������ȏꍇ
	{
		// �L�[�{�[�h�̏�Ԃ��擾����
		auto& keyboardState = m_pCommonResources->GetInputManager()->GetKeyboardState();
		//�@ESC�L�[�������ꂽ��Q�[���C���m�F��L���ɂ���
		if (keyboardState.Escape && !m_pGameEndChecker->GetIsGameEndCheck())
		{
			// �Q�[���I���O�̊m�F������L��������
			m_pGameEndChecker->SetIsGameEndCheck(true);
			// ESC�L�[��SE���Đ�����
			m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		}
	}
	// �ݒ�o�[�̍X�V
	m_pSettingBar->Update(ctx);
	// �ݒ胁�j���[�̍X�V
	m_pSettingMenu->Update(ctx);
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
	// �Q�[���I���O�̊m�F�������L���ȏꍇ
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// �Q�[���I���O�̊m�F��ʂ�`�悷��
		m_pGameEndChecker->Render();
	}
	else// �Q�[���I���O�̊m�F�����������ȏꍇ
	{
		// �t�F�[�h�C�����I�������
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
		{
			// �e��UI�̕`��
			// �ݒ胁�j���[�̕`��
			m_pSettingMenu->Render();
			// �ݒ�o�[�̕`��
			m_pSettingBar->Render();
		}
	}
	// �}�E�X�J�[�\���̕`��
	m_pMousePointer->Render();
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
		// �I���t���O�������Ă�Ȃ�Q�[�����I������
		if (m_pGameEndChecker->GetIsEndGame())PostQuitMessage(0);
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
	// ���ʂ̎擾(10����1�Ŋ������擾)
	// BGM
	float BGMvolume = static_cast<float>(m_pSettingBar->GetSetting(0)) * 0.1f;
	// SE
	float SEvolume = static_cast<float>(m_pSettingBar->GetSetting(1)) * 0.1f;
	// �ݒ�̕ύX
	// BGM
	m_pSettingData->SetBGMVolume(m_pSettingBar->GetSetting(0));
	// SE
	m_pSettingData->SetSEVolume(m_pSettingBar->GetSetting(1));
	// ���ʂ̐ݒ�
	//	BGM����
	m_BGMvolume = VOLUME * BGMvolume;
	// SE����
	m_SEvolume = VOLUME * SEvolume;
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
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �t�F�[�h�̏�Ԃ��m�F���A�t�F�[�h�C�����������Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;
	// ���N���b�N���ꂽ��
	if (MyMouse::IsLeftMouseButtonPressed(mouseState))
	{
		// ���j���[�I���̏������s��
		HandleMenuSelection();
	}
	// �}�E�X�|�C���^�[�̍X�V���s��
	m_pMousePointer->Update(ctx);
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
*	@brief ���j���[�I���̏���
*	@details ���j���[�I���̏������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::HandleMenuSelection()
{
	// �I�����ꂽ���j���[��ID���擾
	auto selectID = m_pSettingMenu->GetSelectIDNum();
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

}
/*
*	@brief �Q�[���I���O�̊m�F����
*	@details �Q�[���I���O�̊m�F�������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingScene::UpdateCheckGameEnd()
{
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();

	// UI�̍X�V�ɕK�v�ȏ����܂Ƃ߂��\����
	UpdateContext ctx{};
	// �o�ߎ��Ԃ�n��
	ctx.elapsedTime = m_elapsedTime;
	// �Q�[���I���O�̊m�F��ʂ̍X�V
	m_pGameEndChecker->Update(ctx);
	// ���N���b�N����Ă��āAUI�ɃJ�[�\�����������Ă���ꍇ
	if (MyMouse::IsLeftMouseButtonPressed(mouseState) && m_pGameEndChecker->GetIsHit())
	{
		// SE�̍Đ�
		m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		// �Q�[������߂邩�ǂ����̃t���O���擾
		if (m_pGameEndChecker->GetIsEndGame())// �Q�[������߂�ꍇ
		{
			// �t�F�[�h�A�E�g�Ɉڍs
			m_pFade->SetState(Fade::FadeState::FadeOut);
		}
		else// �Q�[������߂Ȃ��ꍇ
		{
			// �Q�[���I���O�̊m�F�t���O�𖳌�������
			m_pGameEndChecker->SetIsGameEndCheck(false);
		}
	}
}
