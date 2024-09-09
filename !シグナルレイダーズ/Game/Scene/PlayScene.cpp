/*
	@file	PlayScene.cpp
	@brief	�v���C�V�[���N���X
*/
#include "pch.h"
#include "Game/Scene/PlayScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/FPS_Camera/FPS_Camera.h"
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
	m_gridFloor{},
	m_projection{},
	m_isChangeScene{},
	m_model{},
	m_angle{},
	m_camera{},
	m_wifi{ nullptr },
	m_stage1{ nullptr },
	m_particles{},
	m_isFade{},
	m_volume{},
	m_counter{},
	m_enemyBornTimer{ 0.0f },
	m_enemyBornInterval{ 0.5f },
	m_enemyIndex{ 0 },
	m_fade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 }
{
}
//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
PlayScene::~PlayScene()
{
	// do nothing.
	Finalize();
}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void PlayScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	auto DR = m_commonResources->GetDeviceResources();

	m_wifi = std::make_unique<Wifi>();
	m_wifi->Initialize();
	// �O���b�h�����쐬����
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
	m_gridFloor->SetColor(DirectX::Colors::Yellow);
	// �n�ʁi�X�e�[�W�P�����j
	m_stage1 = std::make_unique<Stage1>();
	m_stage1->Initialize(resources);

	// FPS�J�������쐬����
	m_camera = std::make_unique<FPS_Camera>();
	// �R���g���[���[����
	m_playerController = std::make_unique<PlayerController>();
	m_playerController->Initialize(resources);
	m_playerController->SetPlayetPosition(m_camera->GetEyePosition());
	// ��]�p������������i�x�j
	m_angle = 0;
	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
	// �X�J�C�{�b�N�X����
	m_skybox = std::make_unique<SkyBox>();
	m_skybox->Initialize(resources);
	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);
	m_inPlayerArea.Radius = 20.0f;
	m_PlayerSphere.Radius = 2.0f;
	// HP�Q�[�W�쐬
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, 1280, 720);
	// �Ə��쐬
	m_pPlayerPointer = std::make_unique<PlayerPointer>();
	m_pPlayerPointer->Initialize(DR, 1280, 720);

	// �t�F�[�h�̏�����
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::GO));

	/*
		�f�o�b�O�h���[�̕\���p�I�u�W�F�N�g�𐶐�����
	*/
	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);


	// Sound�p�̃I�u�W�F�N�g������������
	InitializeFMOD();

	// �t�F�[�h�Ɋւ��鏀��
	m_isFade = false;
	m_volume = 1.0f;
	m_counter = 0;
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kb = m_commonResources->GetInputManager()->GetKeyboardTracker();
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// �I�[�f�B�I�}�l�[�W���[�̃C���X�^���X���擾
	auto audioManager = AudioManager::GetInstance();
	// ��d�Đ����Ȃ�
	audioManager->PlaySound("BGM", 0.3);

	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_camera->GetDirection();
	m_playerController->Update(kb, cameraDirection, elapsedTime);

	// �Z�Z����]����
	m_angle += 0.025f;
	if (m_angle > 360)m_angle = 0;
	// FPS�J�����ʒu���X�V����
	m_camera->Update(m_playerController->GetPlayerPosition(), m_playerController->GetYawX());
	m_camera->SetTargetPositionY(m_playerController->GetPitch());
	m_pPlayerHP->Update(m_playerHP);
	m_pPlayerPointer->Update();

#ifdef _DEBUG
	// �f�o�b�O�p
   // �E�N���b�N�œG����|
	if (mtracker->GetLastState().rightButton)
	{
		for (auto& enemy : m_enemy)
		{
			enemy->SetEnemyHP(0);
		}
	}
	// �X�y�[�X�L�[�Ńv���C���[��HP��0�ɂ���
	if (kb->pressed.Space)
	{
		m_playerHP = 0.0f;
	}
#endif


	// ���N���b�N�Œe����
	if (mtracker->GetLastState().leftButton && !m_isBullet)
	{
		// SE�̍Đ�
		audioManager->PlaySound("SE", .3);
		auto bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(m_commonResources);
		bullet->MakeBall(m_playerController->GetPlayerPosition(), cameraDirection);
		m_playerBullets.push_back(std::move(bullet));
		m_isBullet = true;
	}
	if (!mtracker->GetLastState().leftButton)
	{
		m_isBullet = false;
	}
	m_wifi->Update(elapsedTime);
	UpdateBullets(elapsedTime);


	UpdateEnemies(elapsedTime);
	m_inPlayerArea.Center = m_playerController->GetPlayerPosition();// �v���C���[�̈ʒu���擾
	m_PlayerSphere.Center = m_playerController->GetPlayerPosition();// �v���C���[�̈ʒu���擾
	// �p�[�e�B�N���̍X�V
	for (auto& particle : m_particles) particle->Update(elapsedTime);
	// �v���C���[��HP��0�ȉ��Ȃ�
	if (m_playerHP <= 0.0f)
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);


	}
	// ��ʑJ�ڃt�F�[�h����
	m_fade->Update(elapsedTime);
	// �t�F�[�h�A�E�g���I��������
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)
	{
		m_isChangeScene = true;
	}
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void PlayScene::Render()
{


	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �J��������r���[�s��Ǝˉe�s����擾����
	Matrix view = m_camera->GetViewMatrix();
	Matrix projection = m_camera->GetProjectionMatrix();
	Matrix skyWorld = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	skyWorld *= Matrix::CreateScale(10);
#ifdef _DEBUG
	// �i�q����`�悷��
	m_gridFloor->Render(context, view, projection);
#endif
	// �X�J�C�{�b�N�X�`��
	m_skybox->Render(view, projection, skyWorld, m_playerController->GetPlayerPosition());
	// �n�ʕ`��
	m_stage1->Render(view, projection);
	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);


#ifdef _DEBUG
	m_primitiveBatch->Begin();
	DX::Draw(m_primitiveBatch.get(), m_PlayerSphere, DirectX::Colors::PeachPuff);
	m_primitiveBatch->End();
#endif

	// �e��`�悷��
	for (const auto& bullet : m_playerBullets)bullet->Render(view, projection);

	// �G��`�悷��
	if (m_enemy.size() > 0)
	{
		for (const auto& enemy : m_enemy)
		{

			enemy->Render(view, projection);

		}
	}

	// �p�[�e�B�N����`�悷��
	m_particles.erase
	(std::remove_if(m_particles.begin(), m_particles.end(), [&](const std::unique_ptr<Particle>& particle)//	�Đ��I�������p�[�e�B�N�����폜����
					{
						if (!particle->IsPlaying()) return true;// �Đ��I�������p�[�e�B�N���͍폜����
						particle->Render(context, view, projection);// �p�[�e�B�N����`�悷��
						return false;//	�Đ����̃p�[�e�B�N���͍폜���Ȃ�
					}),
	 m_particles.end()//	�폜�Ώۂ̃p�[�e�B�N�����폜����
	);
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = m_commonResources->GetDebugString();
	m_wifi->Render(debugString);
	m_pPlayerHP->Render();
	m_pPlayerPointer->Render();

	// �t�F�[�h�̕`��
	m_fade->Render();
}
//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void PlayScene::Finalize()
{
	// �I�[�f�B�I�}�l�[�W���[�̃C���X�^���X���擾
	auto audioManager = AudioManager::GetInstance();;
	// do nothing.
	m_playerBullets.clear();
	m_enemy.clear();
	m_wifi.reset();
	m_gridFloor.reset();
	m_model.reset();
	m_camera.reset();
	m_playerController.reset();
	m_skybox.reset();

	audioManager->Shutdown();
}
//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �I�[�f�B�I�}�l�[�W���[�̃C���X�^���X���擾
	auto audioManager = AudioManager::GetInstance();
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		audioManager->StopSound("BGM");// BGM���~����
		audioManager->StopSound("SE");// SE���~����

		if (m_playerHP <= 0.0f)// �v���C���[��HP��0�ȉ��Ȃ�
		{
			return IScene::SceneID::GAMEOVER;// �Q�[���I�[�o�[�V�[����
		}
		else
		{
			return IScene::SceneID::CLEAR;// �N���A�V�[����
		}

	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;// �������Ȃ�
}

void PlayScene::UpdateBullets(float elapsedTime)
{
#ifdef _DEBUG
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("HP:%f", m_playerHP);
#endif
	DirectX::SimpleMath::Vector3 dir = m_camera->GetDirection();
	for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); )
	{
		(*it)->Update(dir, elapsedTime);

		if ((*it)->IsExpired())
		{
			it = m_playerBullets.erase(it);
		}
		else
		{
			bool isHit = false;
			for (auto& enemy : m_enemy)
			{
				if ((*it)->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
				{
					isHit = true;
					m_count++;//debug
					enemy->SetEnemyHP(enemy->GetHP() - (*it)->Damage());
					m_particles.push_back(std::make_unique<Particle>(m_commonResources,
																	 Particle::ParticleType::ENEMY_HIT,
																	 enemy->GetPosition(),
																	 enemy->GetMatrix()));
					enemy->SetHitToPlayerBullet(true);
					break;
				}
			}
			if (isHit)
			{

				it = m_playerBullets.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void PlayScene::UpdateEnemies(float elapsedTime)
{
	// �G���S�ł�����V�[����ύX����E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	if (m_enemy.size() <= 0 && m_isBorned)
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);
	}
	// �G�����E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	// �G�����^�C�}�[���X�V
	m_enemyBornTimer += elapsedTime;
	if (!m_isEnemyBorn && m_wifi->GetPreWifiLevels().empty())m_isEnemyBorn = true;//�����\�ɂ���
	// �����\�Ȃ�
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < m_wifi->GetWifiLevels().size())
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// ��莞�Ԍo�߂�����
		{
			// �G�𐶐�����
			auto enemy = std::make_unique<Enemy>();
			enemy->Initialize(m_commonResources, m_wifi->GetWifiLevels()[m_enemyIndex]);


			m_enemy.push_back(std::move(enemy));
			// �^�C�}�[�����Z�b�g���A����Wi-Fi���x���̃C���f�b�N�X�ɐi��
			m_enemyBornTimer = 0.0f;
			m_enemyIndex++;
		}
	}
	// ��������������
	if (m_enemyIndex >= m_wifi->GetWifiLevels().size())
	{
		m_enemyBornTimer = 0.0f;
		// �����s�\�ɂ���
		m_isEnemyBorn = false;
		m_isBorned = true;
	}
	// �G���m�̓����蔻��E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	// �G���m���d�Ȃ�Ȃ��悤�ɂ���
	for (size_t i = 0; i < m_enemy.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemy.size(); ++j)
		{
			bool hit = m_enemy[i]->GetBoundingSphere().Intersects(m_enemy[j]->GetBoundingSphere());
			m_enemy[i]->SetHitToOtherEnemy(hit);
			m_enemy[j]->SetHitToOtherEnemy(hit);

			if (hit)
			{
				m_enemy[i]->CheckHitOtherEnemy(m_enemy[i]->GetBoundingSphere(), m_enemy[j]->GetBoundingSphere());
			}
		}
	}

	// �G�ƃv���C���[�̈��͈͂Ƃ̓����蔻��E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	for (auto& enemy : m_enemy)
	{
		m_isHitPlayerToEnemy = false;
		// �G���X�V
		enemy->Update(elapsedTime, m_playerController->GetPlayerPosition());

		// �G�̒e���v���C���[�ɓ���������
		bool hit = enemy->GetBulletHitToPlayer();
		if (hit)
		{
			enemy->SetPlayerHP(m_playerHP);
			enemy->SetBulletHitToPlayer(false);
		}
		// �G���v���C���[�ɓ���������
		if (enemy->GetBoundingSphere().Intersects(m_inPlayerArea))	m_isHitPlayerToEnemy = true;

		// �v���C���[�ƓG�̓����蔻���ݒ�
		enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
		enemy->SetPlayerBoundingSphere(m_PlayerSphere);
	}

	// �G�̍폜�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

	// �폜�Ώۂ�ێ�����x�N�^�[
	std::vector<std::unique_ptr<Enemy>> enemiesToRemove;

	// �폜�Ώۂ����W����
	for (auto it = m_enemy.begin(); it != m_enemy.end(); )
	{
		// �G������ł�����
		if ((*it)->GetEnemyIsDead())
		{

			// �G�̍��W��n���Ĕ��j�G�t�F�N�g���Đ�
			m_particles.push_back(std::make_unique<Particle>(m_commonResources,
															 Particle::ParticleType::ENEMY_DEAD,
															 (*it)->GetPosition(),
															 (*it)->GetMatrix()));

			// �폜�Ώۂɒǉ�
			enemiesToRemove.push_back(std::move(*it));

			it = m_enemy.erase(it);  // �폜���ăC�e���[�^���X�V
		}
		else
		{
			++it;  // ���̗v�f��
		}
	}


}


//---------------------------------------------------------
// FMOD�̃V�X�e���̏������Ɖ����f�[�^�̃��[�h
//---------------------------------------------------------
void PlayScene::InitializeFMOD()
{
	// �V���O���g���̃I�[�f�B�I�}�l�[�W���[
		// AudioManager�̃V���O���g���C���X�^���X���擾
	AudioManager* audioManager = AudioManager::GetInstance();

	// FMOD�V�X�e���̏�����
	audioManager->Initialize();

	// �����f�[�^�̃��[�h
	// �����ŕK�v�ȉ����f�[�^��AudioManager�Ƀ��[�h������
	audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "SE");
	audioManager->LoadSound("Resources/Sounds/playbgm.mp3", "BGM");


}
