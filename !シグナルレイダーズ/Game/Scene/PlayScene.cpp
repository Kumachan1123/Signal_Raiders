/*
	@file	PlayScene.cpp
	@brief	�v���C�V�[���N���X
*/
#include "pch.h"
#include "Game/Scene/PlayScene.h"
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
PlayScene::PlayScene()
	:
	m_commonResources{},
	m_projection{},
	m_isChangeScene{ false },
	m_angle{ 0.0f },
	m_pStage{ nullptr },
	m_pEffect{},
	m_isFade{ false },
	m_volume{ 1.0f },
	m_counter{ 0 },
	m_fade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 },
	m_audioManager{ AudioManager::GetInstance() }
{}
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
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto DR = m_commonResources->GetDeviceResources();
	// �v���C���[������������
	m_pPlayer = std::make_unique<Player>(resources);

	// �G�S�̂�����������
	m_pEnemies = std::make_unique<Enemies>(resources);
	m_pPlayer->Initialize(m_pEnemies.get());
	m_pEnemies->Initialize(m_pPlayer.get());
	// �n�ʁi�X�e�[�W�P�����j
	m_pStage = std::make_unique<Stage>();
	m_pStage->Initialize(resources);
	// �X�J�C�{�b�N�X����
	m_skybox = std::make_unique<SkyBox>();
	m_skybox->Initialize(resources);
	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);
	// �t�F�[�h�̏�����
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::GO));
	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>
		(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);
	// Sound�p�̃I�u�W�F�N�g������������
	InitializeFMOD();
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kb = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// ��d�Đ����Ȃ�
	m_audioManager->PlaySound("BGM", 0.3);
	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();
	m_pPlayer->Update(kb, elapsedTime);

	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_pEnemies->Update(elapsedTime);// �G�̍X�V

	// �v���C���[��HP��0�ȉ��A�܂��͓G�����Ȃ��Ȃ�
	if (m_pPlayer->GetPlayerHP() <= 0.0f || (m_pEnemies->GetEnemy().size() <= 0 && m_pEnemies->GetisBorned()))
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);
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
	// �V���`��
	m_skybox->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// �n�ʕ`��
	m_pStage->Render(view, projection);
	// �G��`�悷��
	m_pEnemies->Render();
	// �v���C���[��`�悷��
	m_pPlayer->Render();
	// �t�F�[�h�̕`��
	m_fade->Render();
#ifdef _DEBUG// �v���C���[��HP��\������
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());
#endif
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
	// FMOD�V�X�e���̏�����
	m_audioManager->Initialize();
	// �����ŕK�v�ȉ����f�[�^��AudioManager�Ƀ��[�h������
	m_audioManager->LoadSound("Resources/Sounds/playbgm.mp3", "BGM");
}
