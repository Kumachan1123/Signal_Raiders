/*
	@file	EnemyManager.cpp
	@brief	�G�����N���X
*/
#include "pch.h"
#include "EnemyManager.h"

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include "Game/KumachiLib/KumachiLib.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
EnemyManager::EnemyManager(CommonResources* commonResources)
	: m_commonResources{ commonResources }
	, m_enemies{}
	, m_boss{}
	, m_isEnemyBorn{ false }
	, m_isBorned{ false }
	, m_isHitPlayerToEnemy{ false }
	, m_isBossBorn{ false }
	, m_isBossBorned{ false }
	, m_isBossAlive{ true }
	, m_enemyIndex{ 0 }
	, m_stageNumber{ 0 }
	, m_enemyMax{ 0 }
	, m_enemyBornInterval{ 0.5f }
	, m_enemyBornTimer{ 0.0f }
	, m_bossHP{ 100 }
	, m_bossBulletType{ Boss::BossBulletType::NORMAL }
	, m_startTime{ 0.0f }
	, m_pWifi{ nullptr }
	, m_pWall{ nullptr }
	, m_pPlayer{ nullptr }
	, m_audioManager{ AudioManager::GetInstance() }
	, m_SEVolume{ 0.0f }

{
	// Wi-Fi������������
	m_pWifi = std::make_unique<Wifi>();
	m_pWifi->Initialize();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
EnemyManager::~EnemyManager()
{
}

//---------------------------------------------------------
// ������
//---------------------------------------------------------
void EnemyManager::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	InitializeFMOD();
	// �G�̐�������ݒ�
	SetEnemyMax();
}

//---------------------------------------------------------
// �X�V
//---------------------------------------------------------
void EnemyManager::Update(float elapsedTime)
{
	UpdateStartTime(elapsedTime);
	m_pWifi->Update(elapsedTime);

	// �G�t�F�N�g�̍X�V
	UpdateEffects(elapsedTime);

	// �G�̐�������
	HandleEnemySpawning(elapsedTime);

	// �G���m�̓����蔻��
	HandleEnemyCollisions();

	// �G�ƃv���C���[�̓����蔻��
	HandlePlayerCollisions(elapsedTime);

	// �G�ƕǂ̓����蔻��
	HandleWallCollision();

	// �U�R�G�̍폜����
	RemoveDeadEnemies();

	m_pWifi->Clear(); // Wi-Fi�̃N���A
}

//---------------------------------------------------------
// �`��
//---------------------------------------------------------
void EnemyManager::Render()
{
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	if (m_enemies.size() > 0)for (const auto& enemy : m_enemies)
	{
		enemy->Render(view, projection);//	�G��`�悷��
#ifdef _DEBUG

		enemy->DrawCollision(view, projection);//	�����蔻���`�悷��
#endif

	}

	// �G�t�F�N�g��`�悷��
	GetEffect().erase
	(std::remove_if(GetEffect().begin(), GetEffect().end(), [&](const std::unique_ptr<Effect>& effect)//	�Đ��I�������p�[�e�B�N�����폜����
		{
			if (!effect->IsPlaying()) return true;// �Đ��I�������p�[�e�B�N���͍폜����
			effect->Render(view, projection);// �p�[�e�B�N����`�悷��
			return false;//	�Đ����̃p�[�e�B�N���͍폜���Ȃ�
		}),
		GetEffect().end()//	�폜�Ώۂ̃p�[�e�B�N�����폜����
	);

}
//---------------------------------------------------------
// FMOD�V�X�e���̏�����
//---------------------------------------------------------
void EnemyManager::InitializeFMOD()
{
	// �I�[�f�B�I�}�l�[�W���[
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");// �e���ˉ�
	m_audioManager->LoadSound("Resources/Sounds/Barrier.mp3", "Barrier");// �V�[���h��
	m_audioManager->LoadSound("Resources/Sounds/BarrierBreak.mp3", "BarrierBreak");// �V�[���h�j��
	m_audioManager->LoadSound("Resources/Sounds/Explosion.mp3", "EnemyDead");
	m_audioManager->LoadSound("Resources/Sounds/damage.mp3", "Damage");
}
//---------------------------------------------------------
// �G�̐�������ݒ�
//---------------------------------------------------------
void EnemyManager::SetEnemyMax()
{
	// �X�e�[�W�ԍ��ɂ���ēG�̐��������ݒ�
	switch (m_stageNumber)
	{
	case 0:
		m_enemyMax = 5;// �G�̐��������ݒ�
		m_bossHP = 100;// �{�X�̗̑͂�ݒ�
		m_bossBulletType = Boss::BossBulletType::NORMAL;// �{�X�̒e���ꔭ�ɐݒ�
		break;
	case 1:
		m_enemyMax = 10;// �G�̐��������ݒ�
		m_bossHP = 200;// �{�X�̗̑͂�ݒ�
		m_bossBulletType = Boss::BossBulletType::NORMAL;// �{�X�̒e���ꔭ�ɐݒ�
		break;
	case 2:
		m_enemyMax = 20;// �G�̐��������ݒ�
		m_bossHP = 300;// �{�X�̗̑͂�ݒ�
		m_bossBulletType = Boss::BossBulletType::TWIN;// �{�X�̒e��񔭂ɐݒ�
		break;
	case 3:
		m_enemyMax = 30;// �G�̐��������ݒ�
		m_bossHP = 500;// �{�X�̗̑͂�ݒ�
		m_bossBulletType = Boss::BossBulletType::THREE;// �{�X�̒e���O���ɐݒ�
		break;
	case 4:
		m_enemyMax = 40;// �G�̐��������ݒ�
		m_bossHP = 1000;// �{�X�̗̑͂�ݒ�
		m_bossBulletType = Boss::BossBulletType::SPIRAL;// �{�X�̒e�𗆐��ɐݒ�
		break;
	}
}
//---------------------------------------------------------
// �Q�[���J�n���Ԃ��X�V
//---------------------------------------------------------
void EnemyManager::UpdateStartTime(float elapsedTime)
{
	m_startTime += elapsedTime; // �Q�[���J�n���Ԃ��X�V
}

//---------------------------------------------------------
// �G�t�F�N�g�̍X�V
//---------------------------------------------------------
void EnemyManager::UpdateEffects(float elapsedTime)
{
	for (auto& effect : GetEffect())
	{
		effect->Update(elapsedTime);
	}
}

//---------------------------------------------------------
// �G�̐�������
//---------------------------------------------------------
void EnemyManager::HandleEnemySpawning(float elapsedTime)
{
	m_enemyBornTimer += elapsedTime;

	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());// �G�̐����擾
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;// �G�̐����G�̐�������𒴂�����G�̐�������ɐݒ�

	if (m_startTime >= 5.0f) m_isEnemyBorn = true;// �U�R�G�����\�ɂ���

	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)// �U�R�G�����\���U�R�G�����������Ă��Ȃ��ꍇ
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// �G�����Ԋu�𒴂�����
		{
			EnemyType enemyType = static_cast<EnemyType>(m_pWifi->GetEnemyTypes()[m_enemyIndex]);// �G�̎�ނ��擾
			SpawnEnemy(enemyType);// �G�𐶐�(�w�肵���G�̎��)
		}
	}

	if (m_enemyIndex >= enemyNum)// �G��������ɒB������
	{
		FinalizeEnemySpawn();// �G������������
	}

	if (m_enemies.empty() && m_isBorned && !m_isBossBorned)// �G�����Ȃ��Ȃ�����{�X�𐶐�
	{
		SpawnBoss();// �{�X�𐶐�
	}
}


//---------------------------------------------------------
// �G�𐶐�����(�w�肵���G�̎��)
//---------------------------------------------------------
void EnemyManager::SpawnEnemy(EnemyType type)
{
	// �t�@�N�g���Ő���
	auto enemy = EnemyFactory::CreateEnemy(type, m_pPlayer, m_commonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	enemy->SetAudioManager(m_audioManager);// �I�[�f�B�I�}�l�[�W���[��ݒ�
	enemy->Initialize();// �G��������
	m_enemies.push_back(std::move(enemy));// �G���X�g�ɒǉ�

	m_enemyBornTimer = 0.0f;// �G�����^�C�}�[��������
	m_enemyIndex++; // �G�C���f�b�N�X�𑝉�

}



//---------------------------------------------------------
// �G�̐�����������
//---------------------------------------------------------
void EnemyManager::FinalizeEnemySpawn()
{
	m_enemyBornTimer = 0.0f; // �G�����^�C�}�[��������
	m_isEnemyBorn = false; // �U�R�G�����s�\�ɂ���
	m_isBorned = true; // �U�R�G��������
}

//---------------------------------------------------------
// �{�X�𐶐�����
//---------------------------------------------------------
void EnemyManager::SpawnBoss()
{
	auto boss = std::make_unique<Boss>(m_pPlayer, m_commonResources, m_bossHP);
	boss->SetAudioManager(m_audioManager);
	boss->Initialize();
	boss->SetBulletType(m_bossBulletType);

	m_enemies.push_back(std::move(boss)); // �{�X�� enemies �ɓ���
	m_isBossBorned = true;
}

//---------------------------------------------------------
// �G���m�̓����蔻�菈��
//---------------------------------------------------------
void EnemyManager::HandleEnemyCollisions()
{
	for (size_t i = 0; i < m_enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemies.size(); ++j)
		{
			bool hit = m_enemies[i]->GetBoundingSphere().Intersects(m_enemies[j]->GetBoundingSphere());// �����蔻����擾
			m_enemies[i]->SetHitToOtherEnemy(hit);// �����蔻�茋�ʂ�ݒ�
			m_enemies[j]->SetHitToOtherEnemy(hit);// �����蔻�茋�ʂ�ݒ�

			if (hit)// �����蔻�肪��������
			{
				// m_enemies[i]�̐V�������W
				auto& enemyA = m_enemies[i]->GetBoundingSphere();
				auto& enemyB = m_enemies[j]->GetBoundingSphere();
				Vector3 newPos = CheckHitOtherObject(enemyA, enemyB);
				m_enemies[i]->SetPosition(newPos);
			}
		}
	}
}
//---------------------------------------------------------
// �G�ƕǂ̓����蔻�菈��
//---------------------------------------------------------
void EnemyManager::HandleWallCollision()
{
	for (auto& enemy : m_enemies)
	{
		for (int i = 0; i < 4; i++)
		{
			if (enemy->GetBoundingSphere().Intersects(m_pWall->GetBoundingBox(i)))
			{
				Vector3 newPos = CheckHitWall(enemy->GetBoundingSphere(), m_pWall->GetBoundingBox(i));
				enemy->SetPosition(newPos);
			}
		}
	}
}

//---------------------------------------------------------
// �G�ƃv���C���[�̓����蔻�菈��
//---------------------------------------------------------
void EnemyManager::HandlePlayerCollisions(float elapsedTime)
{
	for (auto& enemy : m_enemies)
	{
		m_isHitPlayerToEnemy = false; // �t���O��������
		enemy->Update(elapsedTime);// �G�̍X�V

		// �G�̒e���v���C���[�ɓ���������
		HandleEnemyBulletCollision(enemy);

		// �G���v���C���[�ɓ���������
		HandleEnemyPlayerCollision(enemy);
	}
}

//---------------------------------------------------------
// �G�̒e���v���C���[�ɓ�����������
//---------------------------------------------------------
void EnemyManager::HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& enemy)
{
	bool hit = enemy->GetPlayerHitByEnemyBullet();
	if (hit)
	{
		float playerHP = m_pPlayer->GetPlayerHP() - enemy->GetToPlayerDamage();
		m_pPlayer->SetPlayerHP(playerHP);
		enemy->SetPlayerHitByEnemyBullet(false);
		m_audioManager->PlaySound("Damage", m_pPlayer->GetVolume());
	}
}

//---------------------------------------------------------
// �G���v���C���[�ɓ�����������
//---------------------------------------------------------
void EnemyManager::HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy)
{
	if (enemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
	{
		m_isHitPlayerToEnemy = true;
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();// �v���C���[�̓����蔻����擾
		playerSphere.Radius /= 3.0f;// �v���C���[�̓����蔻����k��
		// �G���v���C���[��F������͈� / 3.0f = �v���C���[�̓����蔻��̔��a
		if (enemy->GetBoundingSphere().Intersects(playerSphere))
		{
			auto enemySphere = enemy->GetBoundingSphere();
			Vector3 newPos = CheckHitOtherObject(enemySphere, playerSphere);
			enemy->SetPosition(newPos);
		}
	}

	enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	enemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere());
}

//---------------------------------------------------------
// �U�R�G�̍폜����
//---------------------------------------------------------
void EnemyManager::RemoveDeadEnemies()
{
	m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
		[this](std::unique_ptr<IEnemy>& enemy)
		{
			if (enemy->GetEnemyIsDead())
			{
				HandleEnemyDeath(enemy);
				return true; // �폜�Ώ�
			}
			return false; // �c��
		}),
		m_enemies.end());
}

//---------------------------------------------------------
// �G�����񂾏ꍇ�̏���
//---------------------------------------------------------
void EnemyManager::HandleEnemyDeath(std::unique_ptr<IEnemy>& enemy)
{
	if (auto boss = dynamic_cast<Boss*>(enemy.get()))
	{
		m_effect.push_back(std::make_unique<Effect>(m_commonResources,
			Effect::ParticleType::ENEMY_DEAD,
			enemy->GetPosition(),
			10.0f,
			enemy->GetMatrix()));
		m_isBossAlive = false; // �����t���O��false�ɂ���
	}
	else
	{
		m_effect.push_back(std::make_unique<Effect>(m_commonResources,
			Effect::ParticleType::ENEMY_DEAD,
			enemy->GetPosition(),
			3.0f,
			enemy->GetMatrix()));
	}

	m_audioManager->PlaySound("EnemyDead", m_pPlayer->GetVolume() * 10); // �G��SE���Đ�(���������Ȃ��������特�����������特��10�{)
}