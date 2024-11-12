/*
	@file	Enemies.cpp
	@brief	�G�����N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "Enemies.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include "Game/Template/Template.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
Enemies::Enemies(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_enemies{},
	m_boss{},
	m_isEnemyBorn{ false },
	m_isBorned{ false },
	m_enemyIndex{ 0 },
	m_stageNumber{ 0 },
	m_enemyMax{ 0 },
	m_enemyBornInterval{ 0.5f },
	m_enemyBornTimer{ 0.0f },
	m_bossHP{ 100 },
	m_bossBulletType{ Boss::BossBulletType::NORMAL },
	m_startTime{ 0.0f },
	m_pWifi{ nullptr },
	m_pPlayer{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
{
	// Wi-Fi������������
	m_pWifi = std::make_unique<Wifi>();
	m_pWifi->Initialize();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Enemies::~Enemies()
{
}

//---------------------------------------------------------
// ������
//---------------------------------------------------------
void Enemies::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	InitializeFMOD();
	// �G�̐�������ݒ�
	SetEnemyMax();
}

//---------------------------------------------------------
// �X�V
//---------------------------------------------------------
void Enemies::Update(float elapsedTime)
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

	// �U�R�G�̍폜����
	RemoveDeadEnemies();

	m_pWifi->Clear(); // Wi-Fi�̃N���A
}

//---------------------------------------------------------
// �`��
//---------------------------------------------------------
void Enemies::Render()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	if (m_enemies.size() > 0)for (const auto& enemy : m_enemies)enemy->Render(view, projection);
	// �G�t�F�N�g��`�悷��
	GetEffect().erase
	(std::remove_if(GetEffect().begin(), GetEffect().end(), [&](const std::unique_ptr<Effect>& effect)//	�Đ��I�������p�[�e�B�N�����폜����
		{
			if (!effect->IsPlaying()) return true;// �Đ��I�������p�[�e�B�N���͍폜����
			effect->Render(context, view, projection);// �p�[�e�B�N����`�悷��
			return false;//	�Đ����̃p�[�e�B�N���͍폜���Ȃ�
		}),
		GetEffect().end()//	�폜�Ώۂ̃p�[�e�B�N�����폜����
	);

}

void Enemies::InitializeFMOD()
{

	// FMOD�V�X�e���̏�����
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/Explosion.mp3", "EnemyDead");
	m_audioManager->LoadSound("Resources/Sounds/damage.mp3", "Damage");
}

// �G�̐�������ݒ�
void Enemies::SetEnemyMax()
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
void Enemies::UpdateStartTime(float elapsedTime)
{
	m_startTime += elapsedTime; // �Q�[���J�n���Ԃ��X�V
}

//---------------------------------------------------------
// �G�t�F�N�g�̍X�V
//---------------------------------------------------------
void Enemies::UpdateEffects(float elapsedTime)
{
	for (auto& effect : GetEffect())
	{
		effect->Update(elapsedTime);
	}
}

//---------------------------------------------------------
// �G�̐�������
//---------------------------------------------------------
void Enemies::HandleEnemySpawning(float elapsedTime)
{
	m_enemyBornTimer += elapsedTime;

	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;

	if (m_startTime >= 5.0f) m_isEnemyBorn = true;// �U�R�G�����\�ɂ���

	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)// �U�R�G�����\���U�R�G�����������Ă��Ȃ��ꍇ
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// �G�����Ԋu�𒴂�����
		{
			if (m_pWifi->GetEnemyTypes()[m_enemyIndex] == 0)SpawnEnemy();// �G�𐶐�
			else SpawnAreaAttacker();// �͈͍U���^�C�v�̓G�𐶐�
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
// �G�𐶐�����(�ʏ�^�C�v)
//---------------------------------------------------------
void Enemies::SpawnEnemy()
{
	auto enemy = std::make_unique<Enemy>(m_pPlayer);
	enemy->Initialize(m_commonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	m_enemies.push_back(std::move(enemy));

	m_enemyBornTimer = 0.0f;
	m_enemyIndex++;
}

//---------------------------------------------------------
// �G�𐶐�����(�͈͍U���^�C�v)
//---------------------------------------------------------
void Enemies::SpawnAreaAttacker()
{
	auto areaAttacker = std::make_unique<AreaAttacker>(m_pPlayer);
	areaAttacker->Initialize(m_commonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	m_enemies.push_back(std::move(areaAttacker));

	m_enemyBornTimer = 0.0f;
	m_enemyIndex++;
}


//---------------------------------------------------------
// �G�̐�����������
//---------------------------------------------------------
void Enemies::FinalizeEnemySpawn()
{
	m_enemyBornTimer = 0.0f;
	m_isEnemyBorn = false; // �U�R�G�����s�\�ɂ���
	m_isBorned = true; // �U�R�G��������
}

//---------------------------------------------------------
// �{�X�𐶐�����
//---------------------------------------------------------
void Enemies::SpawnBoss()
{
	m_isBossBorned = true; // �{�X�����\�ɂ���
	m_enemies.clear(); // �U�R�G���폜

	m_boss = std::make_unique<Boss>(m_pPlayer); // �{�X�𐶐�
	m_boss->Initialize(m_commonResources, m_bossHP); // �{�X��������
	m_boss->SetBulletType(m_bossBulletType); // �{�X�̒e�̎�ނ�ݒ�
	m_enemies.push_back(std::move(m_boss)); // �{�X��G���X�g�ɒǉ�
}

//---------------------------------------------------------
// �G���m�̓����蔻�菈��
//---------------------------------------------------------
void Enemies::HandleEnemyCollisions()
{
	for (size_t i = 0; i < m_enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemies.size(); ++j)
		{
			bool hit = m_enemies[i]->GetBoundingSphere().Intersects(m_enemies[j]->GetBoundingSphere());
			m_enemies[i]->SetHitToOtherEnemy(hit);// �����蔻�茋�ʂ�ݒ�
			m_enemies[j]->SetHitToOtherEnemy(hit);// �����蔻�茋�ʂ�ݒ�

			if (hit)// �����蔻�肪��������
			{
				m_enemies[i]->CheckHitOtherObject(m_enemies[i]->GetBoundingSphere(), m_enemies[j]->GetBoundingSphere());
			}
		}
	}
}

//---------------------------------------------------------
// �G�ƃv���C���[�̓����蔻�菈��
//---------------------------------------------------------
void Enemies::HandlePlayerCollisions(float elapsedTime)
{
	for (auto& enemy : m_enemies)
	{
		m_isHitPlayerToEnemy = false; // �t���O��������

		enemy->Update(elapsedTime, m_pPlayer->GetCamera()->GetEyePosition());// �G�̍X�V

		// �G�̒e���v���C���[�ɓ���������
		HandleEnemyBulletCollision(enemy);

		// �G���v���C���[�ɓ���������
		HandleEnemyPlayerCollision(enemy);
	}
}

//---------------------------------------------------------
// �G�̒e���v���C���[�ɓ�����������
//---------------------------------------------------------
void Enemies::HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& enemy)
{
	bool hit = enemy->GetBulletHitToPlayer();
	if (hit)
	{
		float playerHP = m_pPlayer->GetPlayerHP() - enemy->GetToPlayerDamage();
		m_pPlayer->SetPlayerHP(playerHP);
		enemy->SetBulletHitToPlayer(false);
		m_audioManager->PlaySound("Damage", m_pPlayer->GetVolume());
	}
}

//---------------------------------------------------------
// �G���v���C���[�ɓ�����������
//---------------------------------------------------------
void Enemies::HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy)
{
	if (enemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
	{
		m_isHitPlayerToEnemy = true;
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();
		playerSphere.Radius /= 3.0f;

		if (enemy->GetBoundingSphere().Intersects(playerSphere))
		{
			enemy->CheckHitOtherObject(enemy->GetBoundingSphere(), playerSphere);
		}
	}

	enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	enemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere());
}

//---------------------------------------------------------
// �U�R�G�̍폜����
//---------------------------------------------------------
void Enemies::RemoveDeadEnemies()
{
	std::vector<std::unique_ptr<IEnemy>> enemiesToRemove;

	for (auto it = m_enemies.begin(); it != m_enemies.end(); )
	{
		if ((*it)->GetEnemyIsDead())
		{
			HandleEnemyDeath(*it);
			enemiesToRemove.push_back(std::move(*it));
			it = m_enemies.erase(it); // �폜���ăC�e���[�^���X�V
		}
		else
		{
			++it; // ���̗v�f��
		}
	}
}

//---------------------------------------------------------
// �G�����񂾏ꍇ�̏���
//---------------------------------------------------------
void Enemies::HandleEnemyDeath(std::unique_ptr<IEnemy>& enemy)
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

	m_audioManager->PlaySound("EnemyDead", m_pPlayer->GetVolume() * 10); // �G��SE���Đ�(������������10�{)
}