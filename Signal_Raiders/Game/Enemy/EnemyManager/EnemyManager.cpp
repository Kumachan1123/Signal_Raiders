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
#include "Game/Enemy/BossBase/BossBase.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
EnemyManager::EnemyManager(CommonResources* commonResources)
	: m_commonResources{ commonResources }
	, m_enemies{}
	, m_isEnemyBorn{ false }
	, m_isBorned{ false }
	, m_isHitPlayerToEnemy{ false }
	, m_isBossBorn{ false }
	, m_isBossBorned{ false }
	, m_isBossAlive{ true }
	, m_isBossAppear{ false }
	, m_enemyIndex{ 0 }
	, m_stageNumber{ 0 }
	, m_enemyMax{ 0 }
	, m_enemyBornInterval{ EnemyParameters::ENEMY_SPAWN_INTERVAL }
	, m_enemyBornTimer{ 0.0f }
	, m_bossHP{ 0 }
	, m_bossBulletType{ BossBase::BossBulletType::STAGE_1 }
	, m_specialAttackCount{ 0 }
	, m_startTime{ 0.0f }
	, m_bossBornWaitTime{ 0.0f }
	, m_pWifi{ nullptr }
	, m_pWall{ nullptr }
	, m_pPlayer{ nullptr }
	, m_pBulletManager{ nullptr }
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
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^���擾
	SetEnemyMax();// �G�̐�������ݒ�
}

//---------------------------------------------------------
// �X�V
//---------------------------------------------------------
void EnemyManager::Update(float elapsedTime)
{
	// �Q�[���J�n���Ԃ��X�V
	UpdateStartTime(elapsedTime);

	// Wi-Fi�̍X�V
	m_pWifi->Update(elapsedTime);

	// �G�t�F�N�g�̍X�V
	UpdateEffects(elapsedTime);

	// �G�̐�������
	HandleEnemySpawning(elapsedTime);

	// �G���m�̓����蔻��
	HandleEnemyCollisions();

	// �G�ƃv���C���[�̓����蔻��
	UpdateEnemies(elapsedTime);

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
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();//	�r���[�s��
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();//	�v���W�F�N�V�����s��
	if (m_enemies.size() > 0)for (const auto& enemy : m_enemies)//	�S�Ă̓G��`�悷��
	{
		enemy->Render(view, projection);//	�G��`�悷��
#ifdef _DEBUG
		enemy->DrawCollision(view, projection);//	�����蔻���`�悷��
#endif
	}

	// �G�t�F�N�g��`�悷��
	GetEffect().erase
	(std::remove_if(GetEffect().begin(),
		GetEffect().end(),
		[&](const std::unique_ptr<Effect>& effect)//	�Đ��I�������p�[�e�B�N�����폜����
		{
			if (!effect->IsPlaying()) return true;// �Đ��I�������p�[�e�B�N���͍폜����
			effect->Render(view, projection);// �p�[�e�B�N����`�悷��
			return false;//	�Đ����̃p�[�e�B�N���͍폜���Ȃ�
		}),
		GetEffect().end()//	�폜�Ώۂ̃p�[�e�B�N�����폜����
	);

}
//---------------------------------------------------------
// �G�̐�������ݒ�
//---------------------------------------------------------
void EnemyManager::SetEnemyMax()
{
	auto it = stageData.find(m_stageNumber);// �X�e�[�W�f�[�^���擾
	if (it != stageData.end())// �X�e�[�W�f�[�^�����݂�����
	{
		m_enemyMax = it->second.enemyMax;// �G�̐��������ݒ�
		m_bossHP = it->second.bossHP;// �{�X��HP��ݒ�
		m_specialAttackCount = it->second.specialAttackCount;// �{�X�̓���U���̐���ݒ�
		m_bossBulletType = it->second.bulletType;// �{�X�̒e�̎�ނ�ݒ�
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
	for (auto& effect : GetEffect())effect->Update(elapsedTime);// �S�ẴG�t�F�N�g���X�V
}

//---------------------------------------------------------
// �G�̐�������
//---------------------------------------------------------
void EnemyManager::HandleEnemySpawning(float elapsedTime)
{
	// �G�����^�C�}�[���X�V
	m_enemyBornTimer += elapsedTime;
	// �G�̐����擾
	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());
	// �G�̐����G�̐�������𒴂�����G�̐�������ɐݒ�
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;
	// �Q�[���J�n���Ԃ��G�����J�n���Ԃ𒴂�����
	if (m_startTime >= EnemyParameters::ENEMY_SPAWN_START_TIME) m_isEnemyBorn = true;// �U�R�G�����\�ɂ���
	// �U�R�G�����\���U�R�G�����������Ă��Ȃ��ꍇ
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// �G�����Ԋu�𒴂�����
		{
			EnemyType enemyType = static_cast<EnemyType>(m_pWifi->GetEnemyTypes()[m_enemyIndex]);// �G�̎�ނ��擾
			SpawnEnemy(enemyType);// �G�𐶐�(�w�肵���G�̎��)
		}
	}
	// �G��������ɒB������
	if (m_enemyIndex >= enemyNum)FinalizeEnemySpawn();// �G������������
	// �G�����Ȃ��Ȃ�����{�X�𐶐�
	if (m_enemies.empty() && m_isBorned && !m_isBossBorned)
	{
		m_isBossAppear = true; // �{�X�������o�t���O�𗧂Ă�
		m_bossBornWaitTime += elapsedTime;// �{�X�����ҋ@���Ԃ��X�V
		if (m_bossBornWaitTime >= EnemyParameters::BOSS_SPAWN_WAIT_TIME)// �{�X�����ҋ@���Ԃ𒴂�����
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
	enemy->Initialize();// �G��������
	enemy->SetBulletManager(m_pBulletManager);// �e�}�l�[�W���[��ݒ�
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
	auto boss = std::make_unique<BossBase>(m_pPlayer, m_commonResources, m_bossHP);// �{�X�𐶐�
	if (m_stageNumber >= 3) boss->SetBossType(BossType::LAST_BOSS);// �{�X�̎�ނ�ݒ�
	else boss->SetBossType(BossType::NORMAL_BOSS);// �{�X�̎�ނ�ݒ�

	boss->SetBulletManager(m_pBulletManager);// �e�}�l�[�W���[��ݒ�
	boss->SetBulletType(m_bossBulletType);// �{�X�̒e�̎�ނ�ݒ�
	boss->GetBulletManager()->SetSpecialAttackCount(m_specialAttackCount);// �{�X�̓���U���̐���ݒ�
	boss->Initialize();// �{�X��������


	m_enemies.push_back(std::move(boss)); // �{�X�� enemies �ɓ���
	m_isBossBorned = true; // �{�X��������
	m_isBossAppear = false; // �{�X�������o�t���O�����낷
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
		for (int i = 0; i < m_pWall->GetWallNum(); i++)
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
// �G�S�̂̍X�V����
//---------------------------------------------------------
void EnemyManager::UpdateEnemies(float elapsedTime)
{
	// �U�����̓G�z����N���A
	m_attackingEnemies.clear();

	for (auto& enemy : m_enemies)
	{
		enemy->Update(elapsedTime);// �G�̍X�V

		HandleEnemyBulletCollision(enemy);// �G�̒e���v���C���[�ɓ���������

		HandleEnemyPlayerCollision(enemy);// �G���v���C���[�ɓ���������


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
		m_commonResources->GetAudioManager()->PlaySound("Damage", m_pPlayer->GetVolume());
	}
}

//---------------------------------------------------------
// �G���v���C���[�ɓ�����������
//---------------------------------------------------------
void EnemyManager::HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy)
{
	m_isHitPlayerToEnemy = false; // �t���O��������

	if (enemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
	{
		m_isHitPlayerToEnemy = true;
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();// �v���C���[�̓����蔻����擾
		playerSphere.Radius /= 3.0f;// �v���C���[�̓����蔻����k��
		// �G���v���C���[��F������͈� / 3.0f = �v���C���[�̓����蔻��̔��a
		if (enemy->GetBoundingSphere().Intersects(playerSphere))
		{
			auto& enemySphere = enemy->GetBoundingSphere();
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
	// �G�����񂾏ꍇ�̏���
	float effectScale;// �����l
	auto pBoss = dynamic_cast<BossBase*>(enemy.get());// IEnemy����Boss�̃|�C���^�[�𒊏o
	// �����{�X��������
	if (pBoss)
	{
		effectScale = pBoss->GetDeadEffectSize();// �{�X�̏ꍇ�̓G�t�F�N�g�̃X�P�[����傫������
		m_isBossAlive = false; // �����t���O��false�ɂ���
	}
	else
	{
		effectScale = EnemyParameters::ENEMY_DEADEFFECT_SCALE;// �U�R�G�̏ꍇ�̓G�t�F�N�g�̃X�P�[��������������
	}
	m_effect.push_back(std::make_unique<Effect>(m_commonResources,
		Effect::EffectType::ENEMY_DEAD,
		enemy->GetPosition(),
		effectScale,
		enemy->GetMatrix()));
	// �G��SE���Đ�(���������Ȃ��������特�����������特�ʕ␳)
	m_commonResources->GetAudioManager()->PlaySound("EnemyDead", m_pPlayer->GetVolume() + m_pPlayer->GetVolumeCorrection());
}