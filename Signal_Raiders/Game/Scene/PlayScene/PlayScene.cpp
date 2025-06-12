/*
*	@file	PlayScene.cpp
*	@brief	�v���C�V�[���N���X
*/
#include <pch.h>
#include "PlayScene.h"

// ���ʂ̊
const float PlayScene::VOLUME = 0.05f;
// �Q�[���X�^�[�g����
const float PlayScene::GAME_START_TIME = 5.0f;
// �҂�����	
const float PlayScene::WAIT_TIME = 1.0f;
// �댯�M����\������v���C���[�̗̑�
const float PlayScene::CRISIS_HP = 20.0f;

/*
*	@brief �R���X�g���N�^
*	@details �v���C�V�[���N���X�̃R���X�g���N�^
*	@param sceneID �V�[��ID
*	@return �Ȃ�
*/
PlayScene::PlayScene(IScene::SceneID sceneID)
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_projection{}// �v���W�F�N�V�����s��
	, m_isChangeScene{ false }// �V�[���ύX�t���O
	, m_isResetHP{ false }// HP���Z�b�g�t���O
	, m_pStage{  }// �n��
	, m_pEffect{}// �G�t�F�N�g
	, m_pFade{}// �t�F�[�h
	, m_fadeState{ }// �t�F�[�h���
	, m_BGMvolume{ PlayScene::VOLUME }// BGM�̉���
	, m_SEvolume{ PlayScene::VOLUME }// SE�̉���
	, m_mouseSensitivity{ }// �}�E�X���x
	, m_nowSceneID{ sceneID }// ���݂̃V�[��ID
	, m_time{ 0.0f }// �o�ߎ���
	, m_waitTime{ 0.0f }// �҂�����
	, m_stageNumber{ 0 }// �X�e�[�W�ԍ�
	, m_pBloom{ Bloom::GetInstance() }// �u���[���G�t�F�N�g
{
}
/*
*	@brief �f�X�g���N�^
*	@details �v���C�V�[���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayScene::~PlayScene() { Finalize(); }
/*
*	@brief ������
*	@details �v���C�V�[���N���X�̏�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void PlayScene::Initialize(CommonResources* resources)
{
	// ���\�[�X�� nullptr �łȂ����Ƃ��m�F
	assert(resources);
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X���擾
	auto DR = m_pCommonResources->GetDeviceResources();
	// �ݒ�f�[�^�𐶐�
	m_pSettingData = std::make_unique<SettingData>();
	// �ݒ�t�@�C������f�[�^��ǂݍ���
	m_pSettingData->Load();
	// BGM�̉��ʂ�ݒ�i���ʂ�VOLUME�l���|����j
	m_BGMvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	// SE�̉��ʂ�ݒ�i���ʂ�VOLUME�l���|����j
	m_SEvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// �}�E�X���x��ݒ�
	m_mouseSensitivity = static_cast<float>(m_pSettingData->GetMouseSensitivity());
	// �n�ʁi�X�e�[�W�j�𐶐�
	m_pStage = std::make_unique<Stage>();
	// �n�ʂ̏�����
	m_pStage->Initialize(resources);
	// �ǂ̐����i���ʃ��\�[�X��n���j
	m_pWall = std::make_unique<Wall>(resources);
	// �ǂ̍쐬�i�f�o�C�X���\�[�X��n���j
	m_pWall->Create(DR);
	// ��̐����i�X�e�[�W�ԍ������Ɂj
	m_pSky = std::make_unique<Sky>(m_stageNumber);
	// ��̏�����
	m_pSky->Initialize(resources);
	// �v���C���[�̐����i���ʃ��\�[�X��n���j
	m_pPlayer = std::make_unique<Player>(resources);
	// �G�}�l�[�W���[�𐶐��i���ʃ��\�[�X��n���j
	m_pEnemyManager = std::make_unique<EnemyManager>(resources);
	// �X�e�[�W�ԍ���G�}�l�[�W���[�ɐݒ�
	m_pEnemyManager->SetStageNumber(m_stageNumber);
	// �v���C���[�̌��ʉ��̉��ʂ�ݒ�
	m_pPlayer->SetVolume(m_SEvolume);
	// BGM���SE�̕����傫���ꍇ�͕␳����
	if (m_SEvolume > m_BGMvolume)	m_pPlayer->SetVolumeCorrection(m_SEvolume - m_BGMvolume);
	// �v���C���[�̃}�E�X���x��ݒ�
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);
	// �v���C���[���������i�G�}�l�[�W���[��n���j
	m_pPlayer->Initialize(m_pEnemyManager.get());
	// �G�}�l�[�W���[���������i�v���C���[��n���j
	m_pEnemyManager->Initialize(m_pPlayer.get());
	// �G�̌��ʉ��̉��ʂ�ݒ�
	m_pEnemyManager->SetSEVolume(m_SEvolume);
	// �G�ɕǏ���n��
	m_pEnemyManager->SetWall(m_pWall.get());
	// �e�}�l�[�W���[�𐶐��i���ʃ��\�[�X��n���j
	m_pBulletManager = std::make_unique<BulletManager>(resources);
	// �e�}�l�[�W���[���������i�v���C���[�E�G�}�l�[�W���[��n���j
	m_pBulletManager->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// �v���C���[�ɒe�}�l�[�W���[��n��
	m_pPlayer->SetBulletManager(m_pBulletManager.get());
	// �G�}�l�[�W���[�ɒe�}�l�[�W���[��n��
	m_pEnemyManager->SetBulletManager(m_pBulletManager.get());
	// �G�J�E���^�[�𐶐�
	m_pEnemyCounter = std::make_unique<EnemyCounter>();
	// �G�J�E���^�[��������
	m_pEnemyCounter->Initialize(resources);
	// �S�[��UI�𐶐��i���ʃ��\�[�X��n���j
	m_pGoal = std::make_unique<Goal>(m_pCommonResources);
	// �S�[��UI�̍쐬�i�f�o�C�X���\�[�X��n���j
	m_pGoal->Create(DR);
	// �댯��Ԃ�UI�𐶐��i���ʃ��\�[�X��n���j
	m_pCrisis = std::make_unique<Crisis>(m_pCommonResources);
	// �댯��Ԃ�UI���쐬�i�f�o�C�X���\�[�X��n���j
	m_pCrisis->Create(DR);
	// Wi-Fi���[�f�B���OUI�𐶐�
	m_pWifiLoading = std::make_unique<WifiLoading>();
	// Wi-Fi���[�f�B���OUI��������
	m_pWifiLoading->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// �v���C���ɕ\�������UI������o�^
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayerHP>()));     // �v���C���[HP
	m_pPlayerUI.push_back(std::move(std::make_unique<DashGauge>()));    // �_�b�V���Q�[�W
	m_pPlayerUI.push_back(std::move(std::make_unique<BulletGauge>()));  // �e�Q�[�W
	m_pPlayerUI.push_back(std::move(std::make_unique<Reticle>()));      // �Ə�
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayGuide>()));    // ��������K�C�h
	// �o�^���ꂽUI��S�ď���������
	for (int it = 0; it < m_pPlayerUI.size(); ++it)	m_pPlayerUI[it]->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// �t�F�[�h�𐶐�
	m_pFade = std::make_unique<Fade>(m_pCommonResources);
	// �t�F�[�h�̏�����
	m_pFade->Initialize();
	// �t�F�[�h�C����Ԃɐݒ�
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// ���[�_�[�𐶐��i���ʃ��\�[�X��n���j
	m_pRadar = std::make_unique<Radar>(resources);
	// ���[�_�[�̏������i�v���C���[�ƓG�}�l�[�W���[��n���j
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// �{�X�o�ꉉ�o�𐶐�
	m_pBossAppear = std::make_unique<BossAppear>();
	// �{�X�o�ꉉ�o��������
	m_pBossAppear->Initialize(m_pCommonResources);
	// �{�X�o�ꉉ�o��SE���ʂ�ݒ�
	m_pBossAppear->SetSEVolume(m_SEvolume);
	// �u���[���G�t�F�N�g�̃|�X�g�v���Z�X����
	m_pBloom->CreatePostProcess(resources);
	// �}�E�X�J�[�\���̐���
	m_pMousePointer = std::make_unique<MousePointer>();
	// �}�E�X�J�[�\���̏�����
	m_pMousePointer->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// �Q�[���I���O�m�F��ʂ𐶐�
	m_pGameEndChecker = std::make_unique<GameEndChecker>();
	// �C���m�F��ʂ�UI���u�X�e�[�W��I�тȂ����v�ɕύX
	m_pGameEndChecker->SetIsStageSelect(true);
	// �Q�[���I���O�m�F��ʂ�������
	m_pGameEndChecker->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
}
/*
*	@brief �X�V����
*	@details �v���C�V�[���N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void PlayScene::Update(float elapsedTime)
{

	// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_pCommonResources->GetAudioManager()->Update();
	// BGM���Đ�����
	m_pCommonResources->GetAudioManager()->PlaySound("PlayBGM", m_BGMvolume);
	// �Q�[���I���O�̊m�F���L���ȏꍇ
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// �Q�[���I���O�̊m�F�������s��
		UpdateCheckGameEnd(elapsedTime);
		// UI�ɓn���X�V�����܂Ƃ߂��\���̂���������
		UpdateContext ctx{};
		ctx.elapsedTime = elapsedTime;
		// �}�E�X�|�C���^�[���X�V����
		m_pMousePointer->Update(ctx);
	}
	else// �Q�[���I���O�̊m�F�������ȏꍇ
	{
		// �L�[�{�[�h�̏�Ԃ��擾����
		auto& keyboardState = m_pCommonResources->GetInputManager()->GetKeyboardState();
		//�@ESC�L�[�������ꂽ��Q�[���I���m�F��L���ɂ���
		if (keyboardState.Escape && !m_pGameEndChecker->GetIsGameEndCheck())
		{
			// �Q�[���I���O�̊m�F������L��������
			m_pGameEndChecker->SetIsGameEndCheck(true);
			// ESC�L�[��SE���Đ�����
			m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		}
		// �Q�[�����X�V����
		UpdateGame(elapsedTime);
	}
	// ��ʑJ�ڃt�F�[�h����
	m_pFade->Update(elapsedTime);
	// �t�F�[�h�A�E�g���I��������V�[���ύX���\�ɂ���
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
}
/*
*	@brief �`�悷��
*	@details �v���C�V�[���N���X�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayScene::Render()
{
	using namespace DirectX::SimpleMath;

	// �J��������r���[�s����擾����
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	// �J��������v���W�F�N�V�����s����擾����
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	// �V���̃��[���h�s��(�T�C�Y��10�{���ăv���C���[�𒆐S�n�Ƃ���)
	Matrix skyWorld = Matrix::Identity
		* Matrix::CreateScale(10.0f)
		* Matrix::CreateTranslation(m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// ���[���h�s�������������
	Matrix world = Matrix::Identity;
	// �I�t�X�N���[���ɃI�u�W�F�N�g��`�悷��
	m_pBloom->ChangeOffScreenRT();
	// �V���`��
	m_pSky->Render(view, projection, skyWorld);
	// �n�ʕ`��
	m_pStage->Render(view, projection, world);
	// �Ǖ`��
	m_pWall->Render(view, projection);
	// �G��`�悷��
	m_pEnemyManager->Render();
	// �v���C���[��`�悷��
	m_pPlayer->Render();
	// �e��`�悷��
	m_pBulletManager->Render();
	// �u���[���G�t�F�N�g��������
	m_pBloom->PostProcess();
	// �Q�[���I���O�̊m�F�������L���ȏꍇ
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// �Q�[���I���O�̊m�F��ʂ�`�悷��
		m_pGameEndChecker->Render();
		// �}�E�X�|�C���^�[��`�悷��
		m_pMousePointer->Render();
	}
	else// �Q�[���I���O�̊m�F�����������ȏꍇ
	{
		// �Q�[���J�n�v���C�V�[���ɐ؂�ւ��Ă���5�b�Ԃ͗P�\����	
		if (m_time >= PlayScene::GAME_START_TIME)
		{
			// HP��10�ȉ��Ŋ�@��ԕ`��
			if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Render();
			// UI��`�悷��
			for (int it = 0; it < m_pPlayerUI.size(); ++it)m_pPlayerUI[it]->Render();
			// �G�J�E���^�[��`�悷��
			m_pEnemyCounter->Render();
			// ���[�_�[��`�悷��
			m_pRadar->Render();
			// �{�X�o�ꉉ�o���L���ȏꍇ�A�X�V����
			if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Render();
		}
		else // �Q�[���J�n����5�b��
		{
			// �w���摜��\��
			m_pGoal->Render();
			// Wi-Fi���[�f�B���O��\��
			m_pWifiLoading->Render();
		}
	}

	// �t�F�[�h�̕`��
	m_pFade->Render();
#ifdef _DEBUG// �f�o�b�O���[�h�ł̂ݎ��s
	// �f�o�b�O����\������
	auto debugString = m_pCommonResources->GetDebugString();
	// �ڑ����Ă���Wi-Fi�̓d�g���x
	debugString->AddString("Power:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality());
	// �ڑ����Ă���Wi-Fi��SSID�̒���
	debugString->AddString("SSID:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());
	// �v���C���[HP
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());
	// �v���C���[HP�̍ő�l
	debugString->AddString("MAXHP:%f", m_pPlayer->GetMaxPlayerHP());
#endif
}
/*
*	@brief �I������
*	@details �v���C�V�[���N���X�̏I��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayScene::Finalize()
{
	// ����������
	m_pSky.reset();
}
/*
*	@brief �Q�[���̍X�V
*	@details �Q�[���̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void PlayScene::UpdateGame(float elapsedTime)
{
	// �o�ߎ��Ԃ����Z����
	m_time += elapsedTime;
	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();
	// �ǂ̍X�V
	m_pWall->Update(elapsedTime);
	// �G�̍X�V
	m_pEnemyManager->Update(elapsedTime);
	// �v���C���[�̍X�V
	m_pPlayer->Update(elapsedTime);
	// �e��UI�ɓn�������܂Ƃ߂��\����
	UpdateContext ctx{};
	// �o�ߎ���
	ctx.elapsedTime = elapsedTime;
	// �_�b�V������
	ctx.dashStamina = m_pPlayer->GetDashTime();
	// �e�̐�
	ctx.bulletPoint = float(m_pPlayer->GetBulletManager()->GetPlayerBulletCount());
	// �e�̍X�V
	m_pBulletManager->Update(elapsedTime);
	// �Q�[���J�n�v���C�V�[���ɐ؂�ւ��Ă���5�b�Ԃ͗P�\����
	if (m_time <= PlayScene::GAME_START_TIME)
	{
		// �w���摜���X�V
		m_pGoal->Update(elapsedTime);
		// Wi-Fi���[�f�B���O���X�V
		m_pWifiLoading->Update(ctx);
	}
	else// 5�b�ȏ�o�߂�����Q�[���X�^�[�g
	{
		// HP�X�V
		m_pPlayer->SetMaxPlayerHP((float)(m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality()));
		// HP������ݒ肳�ꂽ�̂ŉ��߂Ĉ�x����HP��ݒ肵�Ȃ���
		if (m_isResetHP == false)
		{
			// UI���X�V����
			for (int it = 0; it < m_pPlayerUI.size(); ++it)
			{
				// �v���C���[HP��������
				auto pHP = dynamic_cast<PlayerHP*>(m_pPlayerUI[it].get());
				// �̗͂��v���C���[HP��UI�ɓn��
				if (pHP)pHP->SetMaxHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			}
			// �̗͂��v���C���[�N���X�ɓn��
			m_pPlayer->SetPlayerHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			// HP��ݒ肵���̂Ńt���O�𗧂Ă�
			m_isResetHP = true;
		}
		// �v���C���[HP���擾
		ctx.playerHP = m_pPlayer->GetPlayerHP();
		// �̗͂�10�ȉ��ɂȂ������@��ԍX�V
		if (m_pPlayer->GetPlayerHP() <= PlayScene::CRISIS_HP)m_pCrisis->Update(elapsedTime);
		// �o�^����������UI���X�V����
		for (int it = 0; it < m_pPlayerUI.size(); ++it)	m_pPlayerUI[it]->Update(ctx);
		// ���[�_�[���X�V����
		m_pRadar->Update(elapsedTime);
		// �G�̑������擾
		m_pEnemyCounter->SetEnemyIndex(m_pEnemyManager->GetEnemyIndex());
		// ���݂̓G�̐����擾
		m_pEnemyCounter->SetNowEnemy(m_pEnemyManager->GetEnemySize());
		// �G�J�E���^�[�̍X�V
		m_pEnemyCounter->Update(elapsedTime);
		// �ȉ��̏����𖞂����ꍇ�A�t�F�[�h�A�E�g��Ԃɂ���
		if (m_pPlayer->GetPlayerHP() <= 0.0f ||// �v���C���[��HP��0�ȉ����A
			(m_pEnemyManager->GetEnemySize() <= 0 &&// �G�����Ȃ���
				m_pEnemyManager->GetisBorned() &&// �������������Ă���
				m_pEnemyManager->GetIsBossAlive() == false))// �{�X�����Ȃ��Ȃ�
		{
			// �҂����Ԃ����Z����
			m_waitTime += elapsedTime;
		}
		// �҂����Ԃ�1�b�ȏ�Ȃ�t�F�[�h�A�E�g��Ԃɂ���
		if (m_waitTime >= PlayScene::WAIT_TIME)	m_pFade->SetState(Fade::FadeState::FadeOut);
		// �{�X�o�ꉉ�o���X�V����
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Update(elapsedTime);
	}
}
/*
*	@brief �Q�[���I���O�̊m�F����
*	@details �Q�[���I���O�̊m�F�������s��
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void PlayScene::UpdateCheckGameEnd(float elapsedTime)
{
	// �}�E�X�̏�Ԃ��擾����
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// UI�̍X�V�ɕK�v�ȏ����܂Ƃ߂��\����
	UpdateContext ctx{};
	// �o�ߎ��Ԃ�n��
	ctx.elapsedTime = elapsedTime;
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

/*
*	@brief �V�[���ύX
*	@details �V�[���ύX�̗L�����擾����
*	@param �Ȃ�
*	@return �V�[���ύX�̗L��(NONE::�������Ȃ��AGAMEOVER:�Q�[���I�[�o�[�ACLEAR:�N���A)
*/
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		// �I���t���O�������Ă�Ȃ�Q�[�����I������
		if (m_pGameEndChecker->GetIsEndGame())//PostQuitMessage(0);
			return IScene::SceneID::STAGESELECT;
		// BGM���~����
		m_pCommonResources->GetAudioManager()->StopSound("PlayBGM");
		// �v���C���[��HP��0�ȉ��Ȃ�
		if (m_pPlayer->GetPlayerHP() <= 0.0f)
		{
			// �Q�[���I�[�o�[�V�[���֑J��
			return IScene::SceneID::GAMEOVER;
		}
		else
		{
			// �G�����Ȃ��Ȃ�N���A�V�[���֑J��
			return IScene::SceneID::CLEAR;
		}
	}
	// �V�[���ύX���Ȃ��ꍇ�͉������Ȃ�
	return IScene::SceneID::NONE;
}