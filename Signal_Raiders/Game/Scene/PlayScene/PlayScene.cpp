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
	m_pStage{ nullptr },
	m_pEffect{},
	m_fade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 },
	m_audioManager{ AudioManager::GetInstance() },
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
	m_BGMvolume = BGM_VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGM�̉��ʂ�ݒ肷��(�኱���ʂ�␳)
	m_SEvolume = SE_VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SE�̉��ʂ�ݒ肷��
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
	m_pEnemies = std::make_unique<Enemies>(resources);
	m_pEnemies->SetStageNumber(m_stageNumber);// �X�e�[�W�ԍ���ݒ肷��
	m_pPlayer->SetVolume(m_SEvolume);// �v���C���[���o�����ʉ��̉��ʂ�ݒ肷��
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);// �}�E�X���x��ݒ肷��
	m_pPlayer->Initialize(m_pEnemies.get());// �v���C���[������������
	m_pEnemies->Initialize(m_pPlayer.get());// �G������������
	m_pEnemies->SetVolume(m_SEvolume);// �G���o�����ʉ��̉��ʂ�ݒ肷��
	m_pEnemies->SetWall(m_pWall.get());// �G�ɕǂ̏���n��
	// �G�J�E���^�[
	m_pEnemyCounter = std::make_unique<EnemyCounter>();
	m_pEnemyCounter->Initialize(resources);
	// ����
	m_pGoal = std::make_unique<Goal>(m_commonResources);
	m_pGoal->Create(DR);
	// �댯���
	m_pCrisis = std::make_unique<Crisis>(m_commonResources);
	m_pCrisis->Create(DR);
	// HP�Q�[�W�쐬
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, 1280, 720);
	// �Ə��쐬
	m_pReticle = std::make_unique<Reticle>();
	m_pReticle->Initialize(DR, 1280, 720);
	// �������
	m_pPlayGuide = std::make_unique<PlayGuide>();
	m_pPlayGuide->Initialize(DR);
	// �t�F�[�h�̏�����
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// ���[�_�[������������
	m_pRadar = std::make_unique<Radar>(resources);
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemies.get());
	// �u���[���G�t�F�N�g�̐���
	m_pBloom->CreatePostProcess(resources);

	// Sound�p�̃I�u�W�F�N�g������������
	InitializeFMOD();

}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	ShowCursor(FALSE);//�J�[�\���������Ȃ��悤�ɂ���
	// �o�ߎ���
	m_timer += elapsedTime;
	// ��d�Đ����Ȃ�
	m_audioManager->PlaySound("BGM", m_BGMvolume);
	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();

	m_pWall->Update(elapsedTime);
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V

	m_pEnemies->Update(elapsedTime);// �G�̍X�V
	m_pPlayer->Update(elapsedTime);
	if (m_timer <= 5.0f)// �Q�[���J�n����5�b�Ԃ͎w���摜��\��
	{
		m_pGoal->Update(elapsedTime);
	}
	else// 5�b�ȏ�o�߂�����
	{
		// HP�X�V
		m_pPlayerHP->Update(m_pPlayer->GetPlayerHP());
		// �̗͂�10�ȉ��ɂȂ������@��ԍX�V
		if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Update(elapsedTime);
		// �Ə��X�V
		m_pReticle->Update();
		// ��������X�V
		m_pPlayGuide->Update();
		// �G�J�E���^�[�̍X�V
		m_pEnemyCounter->SetEnemyIndex(m_pEnemies->GetEnemyIndex());// �G�̑������擾
		m_pEnemyCounter->SetNowEnemy(m_pEnemies->GetEnemySize());// ���݂̓G�̐����擾
		m_pEnemyCounter->Update(elapsedTime);// �G�J�E���^�[�̍X�V

		if (m_pPlayer->GetPlayerHP() <= 0.0f ||// �v���C���[��HP��0�ȉ����A
			(m_pEnemies->GetEnemySize() <= 0 &&// �G�����Ȃ���
				m_pEnemies->GetisBorned() &&// �������������Ă���
				m_pEnemies->GetIsBossAlive() == false))// �{�X�����Ȃ��Ȃ�
		{
			m_waitTime += elapsedTime;// �҂����Ԃ����Z����
		}
		if (m_waitTime >= 1.0f)// �҂����Ԃ�5�b�ȏ�Ȃ�
		{
			m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
			m_fade->SetState(Fade::FadeState::FadeOut);
		}
		// ���[�_�[���X�V����
		m_pRadar->Update(elapsedTime);
	}


	// ��ʑJ�ڃt�F�[�h����
	m_fade->Update(elapsedTime);
	// �t�F�[�h�A�E�g���I��������
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
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
	m_pEnemies->Render();

	// �v���C���[��`�悷��
	m_pPlayer->Render();

	// �u���[���G�t�F�N�g��������
	m_pBloom->PostProcess();

	// �����_�[�^�[�Q�b�g�����ɖ߂�
	m_pBloom->ChangeDefaultRT();

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
		m_pReticle->Render();// �Ə��`��
	}
	else // �Q�[���J�n����5�b�Ԃ͎w���摜��\��
	{
		m_pGoal->Render();
	}


	// �t�F�[�h�̕`��
	m_fade->Render();

}
//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void PlayScene::Finalize()
{
	m_playerBullet.clear();
	m_enemy.clear();
	m_skybox.reset();
	m_audioManager->Shutdown();
}
//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		m_audioManager->StopSound("BGM");// BGM���~����
		// �v���C���[��HP��0�ȉ��Ȃ�
		if (m_pPlayer->GetPlayerHP() <= 0.0f)return IScene::SceneID::GAMEOVER;// �Q�[���I�[�o�[�V�[����
		// �G�����Ȃ��Ȃ�
		else return IScene::SceneID::CLEAR;// �N���A�V�[����
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;// �������Ȃ�
}

void PlayScene::InitializeFMOD()
{
	// �����ŕK�v�ȉ����f�[�^��AudioManager�Ƀ��[�h������
	m_audioManager->LoadSound("Resources/Sounds/playbgm.mp3", "BGM");
}


