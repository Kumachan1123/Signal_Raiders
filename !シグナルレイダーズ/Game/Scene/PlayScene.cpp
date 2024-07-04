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
	m_wifi{ nullptr }
{
}
//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
PlayScene::~PlayScene()
{
	// do nothing.
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


	m_wifi = std::make_unique<Wifi>();
	m_wifi->Initialize();
	// �O���b�h�����쐬����
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
	m_gridFloor->SetColor(DirectX::Colors::Yellow);
	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
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


}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kb = m_commonResources->GetInputManager()->GetKeyboardTracker();
	auto& mstate = m_commonResources->GetInputManager()->GetMouseState();
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();


	// �J�����������Ă���������擾����
	DirectX::SimpleMath::Vector3 cameraDirection = m_camera->GetDirection();
	m_playerController->Update(kb, cameraDirection, elapsedTime);
	// �V�[���`�F���W
	//m_isChangeScene = true;
	// �Z�Z����]����
	m_angle += 0.025f;
	if (m_angle > 360)m_angle = 0;
	// FPS�J�����ʒu���X�V����
	m_camera->Update(m_playerController->GetPlayerPosition(), m_playerController->GetYawX());
	m_camera->SetTargetPositionY(m_playerController->GetYawY());


	// ���N���b�N�Œe����
	if (mtracker->GetLastState().leftButton && !m_isBullet)
	{
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

	UpdateBullets(elapsedTime);
	UpdateEnemies(elapsedTime);
	m_wifi->Update(elapsedTime);
	if (!m_isEnemyBorn && m_wifi->GetPreWifiLevels().empty())m_isEnemyBorn = true;//�����\�ɂ���
	// �����\�Ȃ�
	if (m_isEnemyBorn && !m_isBorned)
	{
		for (int it = 0; it < 1; it++)// m_wifi->GetWifiLevels().size()
		{
			auto enemy = std::make_unique<Enemy>();// �G�𐶐�
			enemy->Initialize(m_commonResources, m_wifi->GetWifiLevels()[it]);  // ������
			m_enemy.push_back(std::move(enemy));   // �G�z��ɓo�^
		}
		// �����s�\�ɂ���
		m_isEnemyBorn = false;
		m_isBorned = true;
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
	// �i�q����`�悷��
	m_gridFloor->Render(context, view, projection);
	// �X�J�C�{�b�N�X�`��
	m_skybox->Render(view, projection, skyWorld, m_playerController->GetPlayerPosition());
	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);

	m_inPlayerArea.Center = m_playerController->GetPlayerPosition();
	m_PlayerSphere.Center = m_playerController->GetPlayerPosition();
	m_primitiveBatch->Begin();
	DX::Draw(m_primitiveBatch.get(), m_PlayerSphere, DirectX::Colors::PeachPuff);
	m_primitiveBatch->End();

	//m_model->Draw(context, *states, skyWorld, view, projection);
	// �e��`�悷��
	for (const auto& bullet : m_playerBullets)bullet->Render(view, projection);

	if (m_enemy.size() > 0)
	{
		for (const auto& enemy : m_enemy)
		{

			enemy->Render(view, projection);


		}
	}
	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = m_commonResources->GetDebugString();

	debugString->AddString("Play Scene");
	debugString->AddString("X:%f", m_playerController->GetPlayerPosition().x);
	debugString->AddString("Z:%f", m_playerController->GetPlayerPosition().z);
	m_wifi->Render(debugString);
}
//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void PlayScene::Finalize()
{
	// do nothing.
	m_playerBullets.clear();
	m_enemy.clear();
	m_wifi.reset();
	m_gridFloor.reset();
	m_model.reset();
	m_camera.reset();
	m_playerController.reset();
	m_skybox.reset();
}
//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)return IScene::SceneID::TITLE;
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}

void PlayScene::UpdateBullets(float elapsedTime)
{
	auto debugString = m_commonResources->GetDebugString();
	DirectX::SimpleMath::Vector3 dir = m_camera->GetDirection();
	debugString->AddString("HP:%i", m_playerHP);
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
	if (m_enemy.size() <= 0)
	{
		m_isBorned = false;
	}

	// �G���m�̓����蔻��
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
				m_enemy[i]->CheckHitOtherEnemy(m_enemy[i]->GetBoundingSphere(),
											   m_enemy[j]->GetBoundingSphere());
			}
		}
	}

	// �G�ƃv���C���[�̈��͈͂Ƃ̓����蔻��
	for (auto& enemy : m_enemy)
	{
		m_isHitPlayerToEnemy = false;
		enemy->Update(elapsedTime, m_playerController->GetPlayerPosition());

		bool hit = enemy->GetBulletHitToPlayer();
		if (hit)
		{
			enemy->SetPlayerHP(m_playerHP);

		}

		if (enemy->GetBoundingSphere().Intersects(m_inPlayerArea))	m_isHitPlayerToEnemy = true;


		enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
		enemy->SetPlayerBoundingSphere(m_PlayerSphere);
	}

	// �폜�Ώۂ�ێ�����x�N�^�[
	std::vector<std::unique_ptr<Enemy>> enemiesToRemove;

	// �폜�Ώۂ����W����
	for (auto it = m_enemy.begin(); it != m_enemy.end(); )
	{
		if ((*it)->GetEnemyIsDead())
		{
			// �����ō폜���̓��ʂȏ������s��
			// �Ⴆ�΁A�G�̏��ŃG�t�F�N�g���Đ�����Ȃ�
			//HandleEnemyDeath(it->get());

			// �폜�Ώۂɒǉ�
			enemiesToRemove.push_back(std::move(*it));

			it = m_enemy.erase(it);  // �폜���ăC�e���[�^���X�V
		}
		else
		{
			++it;  // ���̗v�f��
		}
	}

	// �폜�Ώۂɑ΂��ē��ʂȏ��������s
	for (auto& enemy : enemiesToRemove)
	{
		// ���ʂȏ��������s
		//HandleEnemyDeath(enemy.get());
	}
}