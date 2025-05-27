/*
*	@file	PlayScene.cpp
*	@brief	�v���C�V�[���N���X
*/
#include "pch.h"
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
	assert(resources);// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	m_pCommonResources = resources;// ���ʃ��\�[�X���擾
	auto DR = m_pCommonResources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_pSettingData = std::make_unique<SettingData>();// �ݒ�f�[�^�N���X�𐶐�����
	m_pSettingData->Load();// �ݒ�f�[�^���擾����
	m_BGMvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGM�̉��ʂ�ݒ肷��
	m_SEvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SE�̉��ʂ�ݒ肷��
	m_mouseSensitivity = static_cast<float>(m_pSettingData->GetMouseSensitivity());// �}�E�X���x��ݒ肷��
	m_pStage = std::make_unique<Stage>();// �n�ʁi�X�e�[�W�����j
	m_pStage->Initialize(resources);// �n�ʂ�����������
	m_pWall = std::make_unique<Wall>(resources);// �ǐ���
	m_pWall->Create(DR);// �ǂ𐶐�����
	m_sky = std::make_unique<Sky>(m_stageNumber);// �󐶐�
	m_sky->Initialize(resources);// �������������
	m_pPlayer = std::make_unique<Player>(resources);// �v���C���[������������
	m_pEnemyManager = std::make_unique<EnemyManager>(resources);// �G�S�̂�����������
	m_pEnemyManager->SetStageNumber(m_stageNumber);// �X�e�[�W�ԍ���ݒ肷��
	m_pPlayer->SetVolume(m_SEvolume);// �v���C���[���o�����ʉ��̉��ʂ�ݒ肷��
	if (m_SEvolume > m_BGMvolume)m_pPlayer->SetVolumeCorrection(m_SEvolume - m_BGMvolume);// �v���C���[���o�����ʉ��̉��ʂ�␳����
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);// �}�E�X���x��ݒ肷��
	m_pPlayer->Initialize(m_pEnemyManager.get());// �v���C���[������������
	m_pEnemyManager->Initialize(m_pPlayer.get());// �G������������
	m_pEnemyManager->SetSEVolume(m_SEvolume);// �G���o�����ʉ��̉��ʂ�ݒ肷��
	m_pEnemyManager->SetWall(m_pWall.get());// �G�ɕǂ̏���n��
	m_pBulletManager = std::make_unique<BulletManager>(resources);// �e�}�l�[�W���[������������
	m_pBulletManager->Initialize(m_pPlayer.get(), m_pEnemyManager.get());// �e�}�l�[�W���[������������
	m_pPlayer->SetBulletManager(m_pBulletManager.get());// �v���C���[�ƓG�}�l�[�W���[�ɒe�}�l�[�W���[��n��
	m_pEnemyManager->SetBulletManager(m_pBulletManager.get());// �G�}�l�[�W���[�ɒe�}�l�[�W���[��n��
	m_pEnemyCounter = std::make_unique<EnemyCounter>();// �G�J�E���^�[�𐶐�����
	m_pEnemyCounter->Initialize(resources);// �G�J�E���^�[������������
	m_pGoal = std::make_unique<Goal>(m_pCommonResources);// �w���摜�𐶐�����
	m_pGoal->Create(DR);// �w���摜�𐶐�����
	m_pCrisis = std::make_unique<Crisis>(m_pCommonResources);// �댯��ԕ\���𐶐�����
	m_pCrisis->Create(DR);// �댯��ԕ\���𐶐�����
	m_pWifiLoading = std::make_unique<WifiLoading>();// Wi-Fi���[�f�B���O(�Q�[���J�n����T�b�Ԃ̂ݕ\������UI�j�𐶐�����
	m_pWifiLoading->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);// Wi-Fi���[�f�B���O������������
	// �v���C���ɕ\�������UI������o�^
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayerHP>()));		// �v���C���[HP
	m_pPlayerUI.push_back(std::move(std::make_unique<DashGauge>()));	// �_�b�V���Q�[�W
	m_pPlayerUI.push_back(std::move(std::make_unique<BulletGauge>()));	// �e�Q�[�W
	m_pPlayerUI.push_back(std::move(std::make_unique<Reticle>()));		// �Ə�
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayGuide>()));	// �������
	for (int it = 0; it < m_pPlayerUI.size(); ++it)// �o�^��������������������
		m_pPlayerUI[it]->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);// UI��������
	m_pFade = std::make_unique<Fade>(m_pCommonResources);// �t�F�[�h�𐶐�����
	m_pFade->Initialize();// �t�F�[�h�̏�����
	m_pFade->SetState(Fade::FadeState::FadeIn);// �t�F�[�h�C����Ԃɂ���
	m_pRadar = std::make_unique<Radar>(resources);// ���[�_�[������������
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemyManager.get());// ���[�_�[������������
	m_pBossAppear = std::make_unique<BossAppear>();// �{�X�o�ꉉ�o�𐶐�����
	m_pBossAppear->Initialize(m_pCommonResources);// �{�X�o�ꉉ�o������������
	m_pBossAppear->SetSEVolume(m_SEvolume);// �{�X�o�ꉉ�o��SE���ʂ�ݒ肷��
	m_pBloom->CreatePostProcess(resources);// �u���[���G�t�F�N�g�̐�������
}
/*
*	@brief �X�V����
*	@details �v���C�V�[���N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void PlayScene::Update(float elapsedTime)
{
	m_time += elapsedTime;// �o�ߎ���
	m_pCommonResources->GetAudioManager()->PlaySound("PlayBGM", m_BGMvolume);// ��d�Đ����Ȃ�
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();// �J�����������Ă���������擾����
	m_pWall->Update(elapsedTime);// �ǂ̍X�V
	m_pCommonResources->GetAudioManager()->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_pEnemyManager->Update(elapsedTime);// �G�̍X�V
	m_pPlayer->Update(elapsedTime);// �v���C���[�̍X�V
	UpdateContext ctx;// �e��UI�ɓn�������܂Ƃ߂��\����
	ctx.elapsedTime = elapsedTime;// �o�ߎ���
	ctx.dashStamina = m_pPlayer->GetDashTime();// �_�b�V������
	ctx.bulletPoint = float(m_pPlayer->GetBulletManager()->GetPlayerBulletCount());// �e�̐�
	m_pBulletManager->Update(elapsedTime);// �e�̍X�V
	if (m_time <= PlayScene::GAME_START_TIME)// �Q�[���J�n����5�b�Ԃ�
	{
		m_pGoal->Update(elapsedTime);// �w���摜���X�V
		m_pWifiLoading->Update(ctx);// Wi-Fi���[�f�B���O���X�V
	}
	else// 5�b�ȏ�o�߂�����
	{
		m_pPlayer->SetMaxPlayerHP((float)(m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality()));// HP�X�V
		if (m_isResetHP == false)// HP������ݒ肳�ꂽ�̂ŉ��߂Ĉ�x����HP��ݒ肵�Ȃ���
		{
			for (int it = 0; it < m_pPlayerUI.size(); ++it)// UI���X�V����
			{
				auto pHP = dynamic_cast<PlayerHP*>(m_pPlayerUI[it].get());// �v���C���[HP��������
				if (pHP)pHP->SetMaxHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());// �̗͂��v���C���[HP��UI�ɓn��
			}
			m_pPlayer->SetPlayerHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());// �̗͂��v���C���[�N���X�ɓn��
			m_isResetHP = true;// HP��ݒ肵���̂Ńt���O�𗧂Ă�
		}
		ctx.playerHP = m_pPlayer->GetPlayerHP();// �v���C���[HP���擾
		if (m_pPlayer->GetPlayerHP() <= PlayScene::CRISIS_HP)m_pCrisis->Update(elapsedTime);// �̗͂�10�ȉ��ɂȂ������@��ԍX�V
		for (int it = 0; it < m_pPlayerUI.size(); ++it)// �o�^�����������X�V����
			m_pPlayerUI[it]->Update(ctx);// UI���X�V����
		m_pRadar->Update(elapsedTime);// ���[�_�[���X�V����
		m_pEnemyCounter->SetEnemyIndex(m_pEnemyManager->GetEnemyIndex());// �G�̑������擾
		m_pEnemyCounter->SetNowEnemy(m_pEnemyManager->GetEnemySize());// ���݂̓G�̐����擾
		m_pEnemyCounter->Update(elapsedTime);// �G�J�E���^�[�̍X�V
		if (m_pPlayer->GetPlayerHP() <= 0.0f ||// �v���C���[��HP��0�ȉ����A
			(m_pEnemyManager->GetEnemySize() <= 0 &&// �G�����Ȃ���
				m_pEnemyManager->GetisBorned() &&// �������������Ă���
				m_pEnemyManager->GetIsBossAlive() == false))// �{�X�����Ȃ��Ȃ�
		{
			m_waitTime += elapsedTime;// �҂����Ԃ����Z����
		}
		if (m_waitTime >= PlayScene::WAIT_TIME)// �҂����Ԃ�1�b�ȏ�Ȃ�
			m_pFade->SetState(Fade::FadeState::FadeOut);// �t�F�[�h�A�E�g��Ԃɂ���
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Update(elapsedTime);// �{�X�o�ꉉ�o���X�V����
	}
	m_pFade->Update(elapsedTime);// ��ʑJ�ڃt�F�[�h����
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;// �t�F�[�h�A�E�g���I��������V�[���ύX���\�ɂ���
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

	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();// �J��������r���[�s����擾����
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();// �J��������v���W�F�N�V�����s����擾����
	Matrix skyWorld = Matrix::Identity * Matrix::CreateScale(10);;// �X�J�C�{�b�N�X�̃��[���h�s��(�T�C�Y10�{)
	Matrix world = Matrix::Identity;// ���[���h�s��
	m_pBloom->ChangeOffScreenRT();// �I�t�X�N���[���ɃI�u�W�F�N�g��`�悷��
	m_sky->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());// �V���`��
	m_pStage->Render(view, projection, world, Vector3(0, 0, 0));// �n�ʕ`��
	m_pWall->Render(view, projection);// �Ǖ`��
	m_pEnemyManager->Render();// �G��`�悷��
	m_pPlayer->Render();// �v���C���[��`�悷��
	m_pBulletManager->Render();// �e��`�悷��
	m_pBloom->PostProcess();// �u���[���G�t�F�N�g��������
	if (m_time >= PlayScene::GAME_START_TIME)// �Q�[���J�n����5�b�ȏ�o�߂�����
	{
		if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Render();// HP��10�ȉ��Ŋ�@��ԕ`��
		for (int it = 0; it < m_pPlayerUI.size(); ++it)m_pPlayerUI[it]->Render();// UI��`�悷��
		m_pEnemyCounter->Render();// �G�J�E���^�[��`�悷��
		m_pRadar->Render();// ���[�_�[��`�悷��
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Render();// �{�X�o�ꉉ�o���L���ȏꍇ�A�X�V����
	}
	else // �Q�[���J�n����5�b��
	{
		m_pGoal->Render();// �w���摜��\��
		m_pWifiLoading->Render();// Wi-Fi���[�f�B���O��\��
	}
	m_pFade->Render();// �t�F�[�h�̕`��
#ifdef _DEBUG
	auto debugString = m_pCommonResources->GetDebugString();// �f�o�b�O����\������
	debugString->AddString("Power:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality());// �ڑ����Ă���Wi-Fi�̓d�g���x
	debugString->AddString("SSID:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());// �ڑ����Ă���Wi-Fi��SSID�̒���
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());// �v���C���[HP
	debugString->AddString("MAXHP:%f", m_pPlayer->GetMaxPlayerHP());// �v���C���[HP�̍ő�l
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
	m_sky.reset();// ����������
}
/*
*	@brief �V�[���ύX
*	@details �V�[���ύX�̗L�����擾����
*	@param �Ȃ�
*	@return �V�[���ύX�̗L��(NONE::�������Ȃ��AGAMEOVER:�Q�[���I�[�o�[�ACLEAR:�N���A)
*/
IScene::SceneID PlayScene::GetNextSceneID() const
{
	if (m_isChangeScene)// �V�[���ύX������ꍇ
	{
		m_pCommonResources->GetAudioManager()->StopSound("PlayBGM");// BGM���~����
		if (m_pPlayer->GetPlayerHP() <= 0.0f)return IScene::SceneID::GAMEOVER;// �v���C���[��HP��0�ȉ��Ȃ�Q�[���I�[�o�[�V�[����
		else return IScene::SceneID::CLEAR;// �G�����Ȃ��Ȃ�N���A�V�[����
	}
	return IScene::SceneID::NONE;// �V�[���ύX���Ȃ��ꍇ�͉������Ȃ�
}

