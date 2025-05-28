/*
*	@file	EnemyManager.cpp
*	@brief	�G�}�l�[�W���[�N���X
*/
#include <pch.h>
#include "EnemyManager.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G�}�l�[�W���[�N���X�̃R���X�g���N�^
*	@param	CommonResources* pCommonResources ���ʃ��\�[�X
*	@return �Ȃ�
*/
EnemyManager::EnemyManager(CommonResources* pCommonResources)
	: m_pCommonResources{ pCommonResources }// ���ʃ��\�[�X
	, m_pEnemies{}// �G���X�g
	, m_isEnemyBorn{ false }// �G�����t���O
	, m_isBorned{ false }// �G�����ς݃t���O
	, m_isHitPlayerToEnemy{ false }// �v���C���[�ƓG�̓����蔻��t���O
	, m_isBossBorn{ false }// �{�X�����t���O
	, m_isBossBorned{ false }// �{�X�����ς݃t���O
	, m_isBossAlive{ true }// �{�X�����t���O
	, m_isBossAppear{ false }// �{�X�������o�t���O
	, m_enemyIndex{ 0 }// �G�̐�
	, m_stageNumber{ 0 }// �X�e�[�W�ԍ�
	, m_enemyMax{ 0 }// �G�̐������
	, m_enemyBornInterval{ EnemyParameters::ENEMY_SPAWN_INTERVAL }// �G�̐����Ԋu
	, m_enemyBornTimer{ 0.0f }// �G�̐����^�C�}�[
	, m_bossHP{ 0 }// �{�X��HP
	, m_bossBulletType{ BossBase::BossBulletType::STAGE_1 }// �{�X�̒e�̎��
	, m_specialAttackCount{ 0 }// �{�X�̓���U���̐�
	, m_startTime{ 0.0f }// �Q�[���J�n����
	, m_bossBornWaitTime{ 0.0f }// �{�X�����ҋ@����
	, m_pWifi{ nullptr }	// Wi-Fi
	, m_wifiThread{}	// Wi-Fi���擾���邽�߂̃X���b�h
	, m_pWall{ nullptr }// ��
	, m_pPlayer{ nullptr }// �v���C���[
	, m_pBulletManager{ nullptr }// �e�}�l�[�W���[
	, m_SEVolume{ 0.0f }// SE�̉���

{
	m_pWifi = std::make_unique<Wifi>();	// Wi-Fi�N���X�𐶐�
}
/*
*	@brief	�f�X�g���N�^
*	@details �G�}�l�[�W���[�N���X�̃f�X�g���N�^
*	@param	�Ȃ�
*	@return �Ȃ�
*/
EnemyManager::~EnemyManager() {/*do nothing*/ }
/*
*	@brief	������
*	@details �G�}�l�[�W���[�N���X�̏�����
*	@param	Player* pPlayer �v���C���[
*	@return �Ȃ�
*/
void EnemyManager::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^���擾
	SetEnemyMax();// �G�̐�������ݒ�
}
/*
*	@brief	�X�V
*	@details �G�}�l�[�W���[�N���X�̍X�V
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void EnemyManager::Update(float elapsedTime)
{
	UpdateStartTime(elapsedTime);	// �Q�[���J�n���Ԃ��X�V
	m_pWifi->Update(elapsedTime);	// Wi-Fi�̍X�V
	UpdateEffects(elapsedTime);	// �G�t�F�N�g�̍X�V
	HandleEnemySpawning(elapsedTime);	// �G�̐�������
	HandleEnemyCollisions();	// �G���m�̓����蔻��
	UpdateEnemies(elapsedTime);	// �G�ƃv���C���[�̓����蔻��
	HandleWallCollision();	// �G�ƕǂ̓����蔻��
	RemoveDeadEnemies();	// �U�R�G�̍폜����
}
/*
*	@brief	�`��
*	@details �G�}�l�[�W���[�N���X�̕`��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyManager::Render()
{
	using namespace DirectX::SimpleMath;
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();//	�r���[�s��擾
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();//	�v���W�F�N�V�����s��
	if (m_pEnemies.size() > 0)for (const auto& enemy : m_pEnemies)//	�S�Ă̓G��`�悷��
	{
		enemy->Render(view, projection);//	�G��`�悷��
#ifdef _DEBUG// �f�o�b�O���[�h�Ȃ�
		enemy->DrawCollision(view, projection);//	�����蔻���`�悷��
#endif
	}
	GetEffects().erase(std::remove_if(GetEffects().begin(), GetEffects().end(),	// �G�t�F�N�g��`�悷��
		[&](const std::unique_ptr<Effect>& pEffect)//	�Đ��I�������p�[�e�B�N�����폜����
		{
			if (!pEffect->IsPlaying()) return true;// �Đ��I�������p�[�e�B�N���͍폜����
			pEffect->Render(view, projection);// �p�[�e�B�N����`�悷��
			return false;//	�Đ����̃p�[�e�B�N���͍폜���Ȃ�
		}),
		GetEffects().end()//	�폜�Ώۂ̃p�[�e�B�N�����폜����
	);
}
/*
*	@brief	�G�̐�������ݒ�
*	@details �X�e�[�W�f�[�^����G�̐��������ݒ肷��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
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
/*
*	@brief	�o�ߎ��Ԃ����Z
*	@details �G�����J�n���Ԃ��X�V����
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void EnemyManager::UpdateStartTime(float elapsedTime) { m_startTime += elapsedTime; }
/*
*	@brief	�S�G�t�F�N�g�̍X�V
*	@details �G�t�F�N�g�̍X�V���s��
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void EnemyManager::UpdateEffects(float elapsedTime) { for (auto& effect : GetEffects())effect->Update(elapsedTime); }
/*
*	@brief	�G��������
*	@details �G�̐����������s��
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void EnemyManager::HandleEnemySpawning(float elapsedTime)
{
	m_enemyBornTimer += elapsedTime;	// �G�����^�C�}�[���X�V
	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());	// �G�̐����擾
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;	// �G�̐����G�̐�������𒴂�����G�̐�������ɐݒ�
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
	if (m_enemyIndex >= enemyNum)FinalizeEnemySpawn();// ��������ɒB������G������������
	if (m_pEnemies.empty() && m_isBorned && !m_isBossBorned)	// �G�����Ȃ��Ȃ�����{�X�𐶐�
	{
		m_isBossAppear = true; // �{�X�������o�t���O�𗧂Ă�
		m_bossBornWaitTime += elapsedTime;// �{�X�����ҋ@���Ԃ��X�V
		if (m_bossBornWaitTime >= EnemyParameters::BOSS_SPAWN_WAIT_TIME)// �{�X�����ҋ@���Ԃ𒴂�����
			SpawnBoss();// �{�X�𐶐�
	}
}
/*
*	@brief	�G��������
*	@details �w�肵���G�̎�ނ𐶐�����
*	@param	EnemyType type �G�̎��
*	@return �Ȃ�
*/
void EnemyManager::SpawnEnemy(EnemyType type)
{
	auto enemy = EnemyFactory::CreateEnemy(type, m_pPlayer, m_pCommonResources,
		m_pWifi->GetWifiLevels()[m_enemyIndex]);// �t�@�N�g���œG����
	enemy->Initialize();// �G��������
	enemy->SetBulletManager(m_pBulletManager);// �e�}�l�[�W���[��ݒ�
	m_pEnemies.push_back(std::move(enemy));// �G���X�g�ɒǉ�
	m_enemyBornTimer = 0.0f;// �G�����^�C�}�[��������
	m_enemyIndex++; // �G�C���f�b�N�X�𑝉�
}
/*
*	@brief	�G������������
*	@details �G���������������s��
*	@param	�Ȃ�
*	@return �Ȃ�
*
*/
void EnemyManager::FinalizeEnemySpawn()
{
	m_enemyBornTimer = 0.0f; // �G�����^�C�}�[��������
	m_isEnemyBorn = false; // �U�R�G�����s�\�ɂ���
	m_isBorned = true; // �U�R�G��������
}
/*
*	@brief	�{�X��������
*	@details �{�X�𐶐�����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyManager::SpawnBoss()
{
	auto boss = std::make_unique<BossBase>(m_pPlayer, m_pCommonResources, m_bossHP);// �{�X�𐶐�
	if (m_stageNumber >= 3) boss->SetBossType(BossType::LAST_BOSS);// �{�X�̎�ނ�ݒ�
	else boss->SetBossType(BossType::NORMAL_BOSS);// �{�X�̎�ނ�ݒ�
	boss->SetBulletManager(m_pBulletManager);// �e�}�l�[�W���[��ݒ�
	boss->SetBulletType(m_bossBulletType);// �{�X�̒e�̎�ނ�ݒ�
	boss->GetBulletManager()->SetSpecialAttackCount(m_specialAttackCount);// �{�X�̓���U���̐���ݒ�
	boss->Initialize();// �{�X��������
	m_pEnemies.push_back(std::move(boss)); // �{�X�� enemies �ɓ���
	m_isBossBorned = true; // �{�X��������
	m_isBossAppear = false; // �{�X�������o�t���O�����낷
}
/*
*	@brief	�G���m�̓����蔻�菈��
*	@details �G���m�̓����蔻�����������
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyManager::HandleEnemyCollisions()
{
	using namespace DirectX::SimpleMath;
	for (size_t i = 0; i < m_pEnemies.size(); ++i)// �G�̐��������[�v
	{
		for (size_t j = i + 1; j < m_pEnemies.size(); ++j)// �G�̐��������[�v
		{
			if (i == j) { continue; } // �����G���m�͓����蔻�肵�Ȃ�
			bool hit = m_pEnemies[i]->GetBoundingSphere().Intersects(m_pEnemies[j]->GetBoundingSphere());// �����蔻����擾
			m_pEnemies[i]->SetHitToOtherEnemy(hit);// �����蔻�茋�ʂ�ݒ�
			m_pEnemies[j]->SetHitToOtherEnemy(hit);// �����蔻�茋�ʂ�ݒ�
			if (hit)// �����蔻�肪��������
			{
				// m_pEnemies[i]�̐V�������W
				auto& enemyA = m_pEnemies[i]->GetBoundingSphere();// �GA�̓����蔻����擾
				auto& enemyB = m_pEnemies[j]->GetBoundingSphere();// �GB�̓����蔻����擾
				Vector3 newPos = CheckHitOtherObject(enemyA, enemyB);// �����߂��l��
				m_pEnemies[i]->SetPosition(newPos);// �GA�̈ʒu��ݒ�
			}
		}
	}
}
/*
*	@brief	�G�ƕǂ̓����蔻�菈��
*	@details �G�ƕǂ̓����蔻�����������
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyManager::HandleWallCollision()
{
	using namespace DirectX::SimpleMath;
	for (auto& enemy : m_pEnemies)// �G�̐��������[�v
	{
		for (int i = 0; i < m_pWall->GetWallNum(); i++)// �ǂ̐��������[�v
		{
			if (enemy->GetBoundingSphere().Intersects(m_pWall->GetBoundingBox(i)))// �G�̓����蔻��ƕǂ̓����蔻�肪����������
			{
				Vector3 newPos = CheckHitWall(enemy->GetBoundingSphere(), m_pWall->GetBoundingBox(i));// �ǂɓ��������牟���߂�
				enemy->SetPosition(newPos);// �G�̈ʒu��ݒ�
			}
		}
	}
}
/*
*	@brief	�G�S�̂̍X�V����
*	@details �G�̍X�V�������s��
*	@param	float elapsedTime �o�ߎ���
* 	@return �Ȃ�
*/
void EnemyManager::UpdateEnemies(float elapsedTime)
{
	m_pAttackingEnemies.clear();// �U�����̓G�z����N���A
	for (auto& enemy : m_pEnemies)// �G�̐�����
	{
		enemy->Update(elapsedTime);// �G�̍X�V
		HandleEnemyBulletCollision(enemy);// �G�̒e���v���C���[�ɓ���������
		HandleEnemyPlayerCollision(enemy);// �G���v���C���[�ɓ���������
	}
}
/*
*	@brief	�G�̒e�ƃv���C���[�̓����蔻�菈��
*	@details �G�̒e���v���C���[�ɓ����������ǂ�������������
*	@param	std::unique_ptr<IEnemy>& pEnemy �G�̃|�C���^
*	@return �Ȃ�
*/
void EnemyManager::HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& pEnemy)
{
	bool hit = pEnemy->GetPlayerHitByEnemyBullet();// �G�̒e���v���C���[�ɓ����������ǂ���
	if (hit)// �G�e���v���C���[�ɓ���������
	{
		float playerHP = m_pPlayer->GetPlayerHP() - pEnemy->GetToPlayerDamage();// �v���C���[��HP������
		m_pPlayer->SetPlayerHP(playerHP);// �v���C���[��HP��ݒ�
		pEnemy->SetPlayerHitByEnemyBullet(false); // �v���C���[��HP������������t���O�����낷
		m_pCommonResources->GetAudioManager()->PlaySound("Damage", m_pPlayer->GetVolume()); // SE���Đ�
	}
}
/*
*	@brief	�G�ƃv���C���[�̓����蔻�菈��
*	@details �G���v���C���[�ɓ����������ǂ�������������
*	@param	std::unique_ptr<IEnemy>& pEnemy �G�̃|�C���^
*	@return �Ȃ�
*/
void EnemyManager::HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& pEnemy)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_isHitPlayerToEnemy = false; // �t���O��������
	if (pEnemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))// �G�̓����蔻��ƃv���C���[�̓����蔻�肪����������
	{
		m_isHitPlayerToEnemy = true;// �t���O�𗧂Ă�
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();// �v���C���[�̓����蔻����擾
		playerSphere.Radius /= 3.0f;// �v���C���[�̓����蔻����k��
		// �G���v���C���[��F������͈� / 3.0f = �v���C���[�̓����蔻��̔��a
		if (pEnemy->GetBoundingSphere().Intersects(playerSphere))// �G�̓����蔻��ƃv���C���[�̓����蔻�肪����������
		{
			auto& enemySphere = pEnemy->GetBoundingSphere(); // �G�̓����蔻����擾�@
			Vector3 newPos = CheckHitOtherObject(enemySphere, playerSphere); // �����߂��l��
			pEnemy->SetPosition(newPos); // �G�̈ʒu��ݒ�
		}
	}
	pEnemy->SetHitToPlayer(m_isHitPlayerToEnemy);	//�@�G���v���C���[�ɓ����������ǂ����̃t���O��ݒ�
	pEnemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere()); // �v���C���[�̓����蔻���ݒ�
}
/*
*	@brief	���S�����G���폜
*	@details �G�̃��X�g���玀�S�����G���폜����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyManager::RemoveDeadEnemies()
{
	m_pEnemies.erase(std::remove_if(m_pEnemies.begin(), m_pEnemies.end(),		// ���S�����G���폜����
		[this](std::unique_ptr<IEnemy>& pEnemy)
		{
			if (pEnemy->GetEnemyIsDead())// �G���|���ꂽ��
			{
				HandleEnemyDeath(pEnemy);// ���S����
				return true; // �폜
			}
			return false; // �c��
		}),
		m_pEnemies.end());// �폜�Ώۂ̓G���폜����
}

/*
*	@brief	�G�̎��S����
*	@details �G�����S�������̏������s��
*	@param	std::unique_ptr<IEnemy>& pEnemy �G�̃|�C���^
*	@return �Ȃ�
*/
void EnemyManager::HandleEnemyDeath(std::unique_ptr<IEnemy>& pEnemy)
{
	float effectScale;// �����l
	auto pBoss = dynamic_cast<BossBase*>(pEnemy.get());// IEnemy����Boss�̃|�C���^�[�𒊏o
	if (pBoss)	// �����{�X��������
	{
		effectScale = pBoss->GetDeadEffectSize();// �{�X�̏ꍇ�̓G�t�F�N�g�̃X�P�[����傫������
		m_isBossAlive = false; // �����t���O��false�ɂ���
	}
	else // �����{�X����Ȃ�������
	{
		effectScale = EnemyParameters::ENEMY_DEADEFFECT_SCALE;// �U�R�G�̏ꍇ�̓G�t�F�N�g�̃X�P�[��������������
	}
	m_pEffects.push_back(std::make_unique<Effect>(m_pCommonResources,// �G�t�F�N�g�𐶐�
		Effect::EffectType::ENEMY_DEAD,// �G�t�F�N�g�̎�ގw��
		pEnemy->GetPosition(), // ���W�ݒ�
		effectScale,// �X�P�[���ݒ�
		pEnemy->GetMatrix()));// ���[���h�s��쐬
	m_pCommonResources->GetAudioManager()->PlaySound("EnemyDead", m_pPlayer->GetVolume() + m_pPlayer->GetVolumeCorrection());// �G��SE���Đ�

}