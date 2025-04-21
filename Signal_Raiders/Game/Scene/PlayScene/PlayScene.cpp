/*
	@file	PlayScene.cpp
	@brief	�v���C�V�[���N���X
*/
#include "pch.h"
#include "Game/Scene/PlayScene/PlayScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/Screen.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;
//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
PlayScene::PlayScene(IScene::SceneID sceneID)
	:
	m_commonResources{},
	m_projection{},
	m_isChangeScene{ false },
	m_isResetHP{ false },
	m_pStage{  },
	m_pEffect{},
	m_pFade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 },
	//m_audioManager{ AudioManager::GetInstance() },
	m_BGMvolume{ BGM_VOLUME },
	m_SEvolume{ SE_VOLUME },
	m_mouseSensitivity{ },
	m_nowSceneID{ sceneID },
	m_timer{ 0.0f },
	m_waitTime{ 0.0f },
	m_stageNumber{ 0 },
	m_pBloom{ Bloom::GetInstance() }

{

}
//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
PlayScene::~PlayScene() { Finalize(); }
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void PlayScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	auto DR = m_commonResources->GetDeviceResources();
	// �ݒ�f�[�^���擾����
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = BGM_VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * 0.1f;// BGM�̉��ʂ�ݒ肷��(�኱���ʂ�␳)
	m_SEvolume = SE_VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * 0.1f;// SE�̉��ʂ�ݒ肷��
	m_mouseSensitivity = static_cast<float>(m_pSettingData->GetMouseSensitivity());// �}�E�X���x��ݒ肷��
	// �n�ʁi�X�e�[�W�����j
	m_pStage = std::make_unique<Stage>();
	m_pStage->Initialize(resources);
	// ��
	m_pWall = std::make_unique<Wall>(resources);
	m_pWall->Create(DR);
	// �X�J�C�{�b�N�X����
	m_skybox = std::make_unique<Sky>(m_stageNumber);
	m_skybox->Initialize(resources);
	// �v���C���[������������
	m_pPlayer = std::make_unique<Player>(resources);
	// �G�S�̂�����������
	m_pEnemyManager = std::make_unique<EnemyManager>(resources);
	m_pEnemyManager->SetStageNumber(m_stageNumber);// �X�e�[�W�ԍ���ݒ肷��
	m_pPlayer->SetVolume(m_SEvolume);// �v���C���[���o�����ʉ��̉��ʂ�ݒ肷��
	if (m_SEvolume > m_BGMvolume)m_pPlayer->SetVolumeCorrection(m_SEvolume - m_BGMvolume);// �v���C���[���o�����ʉ��̉��ʂ�␳����
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);// �}�E�X���x��ݒ肷��
	m_pPlayer->Initialize(m_pEnemyManager.get());// �v���C���[������������
	m_pEnemyManager->Initialize(m_pPlayer.get());// �G������������
	m_pEnemyManager->SetVolume(m_SEvolume);// �G���o�����ʉ��̉��ʂ�ݒ肷��
	m_pEnemyManager->SetWall(m_pWall.get());// �G�ɕǂ̏���n��
	// �e�}�l�[�W���[������������
	m_pBulletManager = std::make_unique<BulletManager>(resources);
	m_pBulletManager->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// �v���C���[�ƓG�}�l�[�W���[�ɒe�}�l�[�W���[��n��
	m_pPlayer->SetBulletManager(m_pBulletManager.get());
	m_pEnemyManager->SetBulletManager(m_pBulletManager.get());
	// �G�J�E���^�[
	m_pEnemyCounter = std::make_unique<EnemyCounter>();
	m_pEnemyCounter->Initialize(resources);
	// ����
	m_pGoal = std::make_unique<Goal>(m_commonResources);
	m_pGoal->Create(DR);
	// Wi-Fi���[�f�B���O
	m_pWifiLoading = std::make_unique<WifiLoading>();
	m_pWifiLoading->Initialize(DR, Screen::WIDTH, Screen::HEIGHT);
	// �댯���
	m_pCrisis = std::make_unique<Crisis>(m_commonResources);
	m_pCrisis->Create(DR);
	// HP�Q�[�W�쐬
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// �_�b�V���Q�[�W�쐬
	m_pDashGauge = std::make_unique<DashGauge>();
	m_pDashGauge->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// �e�Q�[�W�쐬
	m_pBulletGauge = std::make_unique<BulletGauge>();
	m_pBulletGauge->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// �Ə��쐬
	m_pReticle = std::make_unique<Reticle>();
	m_pReticle->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// �������
	m_pPlayGuide = std::make_unique<PlayGuide>();
	m_pPlayGuide->Initialize(DR);
	// �t�F�[�h�̏�����
	m_pFade = std::make_unique<Fade>(m_commonResources);
	m_pFade->Create(DR);
	m_pFade->SetState(Fade::FadeState::FadeIn);
	m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// ���[�_�[������������
	m_pRadar = std::make_unique<Radar>(resources);
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// �{�X�o�ꉉ�o������������
	m_pBossAppear = std::make_unique<BossAppear>();
	m_pBossAppear->Initialize(m_commonResources);
	// �u���[���G�t�F�N�g�̐���
	m_pBloom->CreatePostProcess(resources);



}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{

	// �o�ߎ���
	m_timer += elapsedTime;
	// ��d�Đ����Ȃ�
	m_commonResources->GetAudioManager()->PlaySound("PlayBGM", m_BGMvolume);
	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();

	m_pWall->Update(elapsedTime);
	m_commonResources->GetAudioManager()->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V

	m_pEnemyManager->Update(elapsedTime);// �G�̍X�V
	m_pPlayer->Update(elapsedTime);// �v���C���[�̍X�V
	m_pBulletManager->Update(elapsedTime);// �e�̍X�V
	if (m_timer <= 5.0f)// �Q�[���J�n����5�b�Ԃ�
	{
		m_pGoal->Update(elapsedTime);// �w���摜���X�V
		m_pWifiLoading->Update(elapsedTime);// Wi-Fi���[�f�B���O���X�V
	}
	else// 5�b�ȏ�o�߂�����
	{
		// HP�X�V
		m_pPlayer->SetMaxPlayerHP((float)(m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality()));
		// HP������ݒ肳�ꂽ�̂ŉ��߂Ĉ�x����HP��ݒ肵�Ȃ���
		if (m_isResetHP == false)
		{
			m_pPlayerHP->SetMaxHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			m_pPlayer->SetPlayerHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			m_isResetHP = true;
		}
		// HP�Q�[�W�X�V
		m_pPlayerHP->Update(m_pPlayer->GetPlayerHP());
		// �̗͂�10�ȉ��ɂȂ������@��ԍX�V
		if (m_pPlayer->GetPlayerHP() <= 20.0f)m_pCrisis->Update(elapsedTime);
		// �_�b�V���Q�[�W�X�V
		m_pDashGauge->Update(m_pPlayer->GetDashTime());
		// �e�Q�[�W�X�V
		m_pBulletGauge->Update(float(m_pPlayer->GetBulletManager()->GetPlayerBulletCount()));
		// �Ə��X�V
		m_pReticle->Update();
		// ��������X�V
		m_pPlayGuide->Update();
		// �G�J�E���^�[�̍X�V
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
		if (m_waitTime >= 1.0f)// �҂����Ԃ�1�b�ȏ�Ȃ�
		{
			m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
			m_pFade->SetState(Fade::FadeState::FadeOut);
		}
		// ���[�_�[���X�V����
		m_pRadar->Update(elapsedTime);
		// �{�X�o�ꉉ�o���X�V����
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Update(elapsedTime);
	}


	// ��ʑJ�ڃt�F�[�h����
	m_pFade->Update(elapsedTime);
	// �t�F�[�h�A�E�g���I��������
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void PlayScene::Render()
{
	// �J��������r���[�s��Ǝˉe�s����擾����
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	Matrix skyWorld = Matrix::Identity * Matrix::CreateScale(10);
	Matrix world = Matrix::Identity;
	// �I�t�X�N���[���ɃI�u�W�F�N�g��`�悷��
	m_pBloom->ChangeOffScreenRT();
	// �V���`��
	m_skybox->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// �n�ʕ`��
	m_pStage->Render(view, projection, world, Vector3(0, 0, 0));
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
	// �Q�[���J�n����5�b�ȏ�o�߂�����
	if (m_timer >= 5.0f)
	{
		if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Render();// HP��10�ȉ��Ŋ�@��ԕ`��
		// �G�J�E���^�[��`�悷��
		m_pEnemyCounter->Render();
		m_pPlayGuide->Render();// ��������`��
		// ���[�_�[��`�悷��
		m_pRadar->Render();
		m_pPlayerHP->Render();// HP�`��
		m_pDashGauge->Render();// �_�b�V���Q�[�W�`��
		m_pBulletGauge->Render();// �e�Q�[�W�`��
		m_pReticle->Render();// �Ə��`��
		// �{�X�o�ꉉ�o���X�V����
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Render();
	}
	else // �Q�[���J�n����5�b��
	{
		m_pGoal->Render();// �w���摜��\��
		m_pWifiLoading->Render();// Wi-Fi���[�f�B���O��\��
	}
	// �t�F�[�h�̕`��
	m_pFade->Render();

#ifdef _DEBUG
	// �f�o�b�O����\������
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("Power:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality());
	debugString->AddString("SSID:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());
	debugString->AddString("MAXHP:%f", m_pPlayer->GetMaxPlayerHP());
#endif
}
//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void PlayScene::Finalize()
{

	m_skybox.reset();
	//m_audioManager->Shutdown();
}
//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		m_commonResources->GetAudioManager()->StopSound("PlayBGM");// BGM���~����
		// �v���C���[��HP��0�ȉ��Ȃ�
		if (m_pPlayer->GetPlayerHP() <= 0.0f)return IScene::SceneID::GAMEOVER;// �Q�[���I�[�o�[�V�[����
		// �G�����Ȃ��Ȃ�
		else return IScene::SceneID::CLEAR;// �N���A�V�[����
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;// �������Ȃ�
}

