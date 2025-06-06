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
	// Wi-Fi�N���X�𐶐�
	m_pWifi = std::make_unique<Wifi>();
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
	// �v���C���[�̃|�C���^���擾
	m_pPlayer = pPlayer;
	// �G�̐�������ݒ�
	SetEnemyMax();
}
/*
*	@brief	�X�V
*	@details �G�}�l�[�W���[�N���X�̍X�V
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
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
	// �r���[�s��擾
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	// �v���W�F�N�V�����s��
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	// �S�Ă̓G��`�悷��
	if (m_pEnemies.size() > 0)for (const auto& enemy : m_pEnemies)
	{
		// �G��`�悷��
		enemy->Render(view, projection);
#ifdef _DEBUG// �f�o�b�O���[�h�Ȃ�
		//	�����蔻���`�悷��
		enemy->DrawCollision(view, projection);
#endif
	}
	// �G�t�F�N�g��`�悷��
	GetEffects().erase(std::remove_if(GetEffects().begin(), GetEffects().end(),
		[&](const std::unique_ptr<Effect>& pEffect)// �Đ��I�������p�[�e�B�N�����폜����
		{
			// �Đ��I�������p�[�e�B�N���͍폜����
			if (!pEffect->IsPlaying()) return true;
			// �p�[�e�B�N����`�悷��
			pEffect->Render(view, projection);
			//	�Đ����̃p�[�e�B�N���͍폜���Ȃ�
			return false;
		}),
		GetEffects().end()// �폜�Ώۂ̃p�[�e�B�N�����폜����
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
	// �X�e�[�W�f�[�^���擾
	auto it = stageData.find(m_stageNumber);
	// �X�e�[�W�f�[�^�����݂�����
	if (it != stageData.end())
	{
		// �G�̐��������ݒ�
		m_enemyMax = it->second.enemyMax;
		// �{�X��HP��ݒ�
		m_bossHP = it->second.bossHP;
		// �{�X�̓���U���̐���ݒ�
		m_specialAttackCount = it->second.specialAttackCount;
		// �{�X�̒e�̎�ނ�ݒ�
		m_bossBulletType = it->second.bulletType;
	}
}
/*
*	@brief	�o�ߎ��Ԃ����Z
*	@details �G�����J�n���Ԃ��X�V����
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void EnemyManager::UpdateStartTime(float elapsedTime)
{
	// �Q�[���J�n���Ԃ��X�V
	m_startTime += elapsedTime;
}
/*
*	@brief	�S�G�t�F�N�g�̍X�V
*	@details �G�t�F�N�g�̍X�V���s��
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void EnemyManager::UpdateEffects(float elapsedTime)
{
	// �S�ẴG�t�F�N�g���X�V����
	for (auto& effect : GetEffects())effect->Update(elapsedTime);
}
/*
*	@brief	�G��������
*	@details �G�̐����������s��
*	@param	float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void EnemyManager::HandleEnemySpawning(float elapsedTime)
{
	// �G�����^�C�}�[���X�V
	m_enemyBornTimer += elapsedTime;
	// �G�̐����擾
	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());
	// �G�̐����G�̐�������𒴂�����G�̐�������ɐݒ�
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;
	// �Q�[���J�n���Ԃ��G�����J�n���Ԃ𒴂�����U�R�G�𐶐��\�ɂ���
	if (m_startTime >= EnemyParameters::ENEMY_SPAWN_START_TIME) m_isEnemyBorn = true;
	// �U�R�G�����\���U�R�G�����������Ă��Ȃ��ꍇ
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)
	{
		// �G�����Ԋu�𒴂�����
		if (m_enemyBornTimer >= m_enemyBornInterval)
		{
			// �G�̎�ނ��擾
			EnemyType enemyType = static_cast<EnemyType>(m_pWifi->GetEnemyTypes()[m_enemyIndex]);
			// �G�𐶐�(�w�肵���G�̎��)
			SpawnEnemy(enemyType);
		}
	}
	// ��������ɒB������G���������������s��
	if (m_enemyIndex >= enemyNum)FinalizeEnemySpawn();
	// �G�����Ȃ��Ȃ�����{�X�𐶐�
	if (m_pEnemies.empty() && m_isBorned && !m_isBossBorned)
	{
		// �{�X�������o�t���O�𗧂Ă�
		m_isBossAppear = true;
		// �{�X�����ҋ@���Ԃ��X�V
		m_bossBornWaitTime += elapsedTime;
		// �{�X�����ҋ@���Ԃ𒴂�����{�X�𐶐�
		if (m_bossBornWaitTime >= EnemyParameters::BOSS_SPAWN_WAIT_TIME)SpawnBoss();
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
	// �t�@�N�g���œG�𐶐�
	auto enemy = EnemyFactory::CreateEnemy(type, m_pPlayer, m_pCommonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	// �G��������
	enemy->Initialize();
	// �e�}�l�[�W���[��ݒ�
	enemy->SetBulletManager(m_pBulletManager);
	// �G���X�g�ɒǉ�
	m_pEnemies.push_back(std::move(enemy));
	// �G�����^�C�}�[��������
	m_enemyBornTimer = 0.0f;
	// �G�C���f�b�N�X�𑝉�
	m_enemyIndex++;
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
	// �G�����^�C�}�[��������
	m_enemyBornTimer = 0.0f;
	// �U�R�G�𐶐��s�\�ɂ���
	m_isEnemyBorn = false;
	// �U�R�G��������
	m_isBorned = true;
}
/*
*	@brief	�{�X��������
*	@details �{�X�𐶐�����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyManager::SpawnBoss()
{
	// �{�X�𐶐��i�v���C���[�A���ʃ��\�[�X�AHP�������ɂ��č쐬�j
	auto boss = std::make_unique<BossBase>(m_pPlayer, m_pCommonResources, m_bossHP);
	// �X�e�[�W�ԍ���3�ȏ�Ȃ�u���X�{�X�v�A����ȊO�́u�ʏ�{�X�v�Ƃ��Đݒ�
	if (m_stageNumber >= 3)// ���X�{�X�ɐݒ�
		boss->SetBossType(BossType::LAST_BOSS);
	else// �ʏ�{�X�ɐݒ�
		boss->SetBossType(BossType::NORMAL_BOSS);
	// �e�}�l�[�W���[���{�X�ɓn��
	boss->SetBulletManager(m_pBulletManager);
	// �{�X�̒e�̎�ނ�ݒ�
	boss->SetBulletType(m_bossBulletType);
	// �e�}�l�[�W���[�ɁA�{�X��p�̓���U���̉񐔂�`����
	boss->GetBulletManager()->SetSpecialAttackCount(m_specialAttackCount);
	// �{�X�̏���������
	boss->Initialize();
	// �G���X�g�Ƀ{�X��ǉ�
	m_pEnemies.push_back(std::move(boss));
	// �{�X���o�������t���O��ON�ɂ���
	m_isBossBorned = true;
	// �{�X�o�ꉉ�o���t���O��OFF�ɂ���
	m_isBossAppear = false;
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
	// �o�^����Ă���G�̐��������[�v
	for (size_t i = 0; i < m_pEnemies.size(); ++i)
	{
		// i�Ԗڂ̓G�ȍ~�̓G�ƏՓ˃`�F�b�N
		for (size_t j = i + 1; j < m_pEnemies.size(); ++j)
		{
			// �����C���f�b�N�X���m�͔��肵�Ȃ�
			if (i == j) { continue; }
			// �GA�ƓGB�̓����蔻��
			bool hit = m_pEnemies[i]->GetBoundingSphere().Intersects(m_pEnemies[j]->GetBoundingSphere());
			// �����̓G�ɓ������Ă邩�ǂ����̃t���O��ݒ�
			m_pEnemies[i]->SetHitToOtherEnemy(hit);
			m_pEnemies[j]->SetHitToOtherEnemy(hit);
			// �Փ˂��Ă�����ʒu�̉����߂��������s��
			if (hit)
			{
				// �GA�̓����蔻����擾
				auto& enemyA = m_pEnemies[i]->GetBoundingSphere();
				// �GB�̓����蔻����擾
				auto& enemyB = m_pEnemies[j]->GetBoundingSphere();
				// �GA�̐V�����ʒu���v�Z
				Vector3 newPos = CheckHitOtherObject(enemyA, enemyB);
				// �GA�̈ʒu���X�V
				m_pEnemies[i]->SetPosition(newPos);
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
	// �o�^����Ă���G�̐��������[�v
	for (auto& enemy : m_pEnemies)
	{
		// �ǂ̐��������[�v
		for (int i = 0; i < m_pWall->GetWallNum(); i++)
		{
			// �G�̓����蔻��ƕǂ̓����蔻�肪����������
			if (enemy->GetBoundingSphere().Intersects(m_pWall->GetBoundingBox(i)))
			{
				// �ǂɓ��������牟���߂�
				Vector3 newPos = CheckHitWall(enemy->GetBoundingSphere(), m_pWall->GetBoundingBox(i));
				// �G�̈ʒu��ݒ�
				enemy->SetPosition(newPos);
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
	// �U�����̓G�z����N���A
	m_pAttackingEnemies.clear();
	// �o�^����Ă���G�̐��������[�v
	for (auto& enemy : m_pEnemies)
	{
		// �G�̍X�V
		enemy->Update(elapsedTime);
		// �G�̒e���v���C���[�ɓ��������ꍇ�̏���
		HandleEnemyBulletCollision(enemy);
		// �G���v���C���[�ɓ��������ꍇ�̏���
		HandleEnemyPlayerCollision(enemy);
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
	// �G�̒e���v���C���[�ɓ����������ǂ������擾
	bool hit = pEnemy->GetPlayerHitByEnemyBullet();
	// �G�e���v���C���[�ɓ���������
	if (hit)
	{
		// �V�����v���C���[��HP���v�Z
		float playerHP = m_pPlayer->GetPlayerHP() - pEnemy->GetToPlayerDamage();
		// �V����HP��ݒ�
		m_pPlayer->SetPlayerHP(playerHP);
		// �v���C���[��HP������������t���O�����낷
		pEnemy->SetPlayerHitByEnemyBullet(false);
		// �v���C���[���������������Đ�����
		m_pCommonResources->GetAudioManager()->PlaySound("Damage", m_pPlayer->GetVolume());
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
	// �q�b�g�t���O��������
	m_isHitPlayerToEnemy = false;
	// �G�̓����蔻��ƃv���C���[�̓����蔻�肪����������
	if (pEnemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
	{
		// �q�b�g�t���O�𗧂Ă�
		m_isHitPlayerToEnemy = true;
		// �v���C���[�����m���铖���蔻����擾
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();
		// �v���C���[�����m���铖���蔻����k�����A�v���C���[�̓����蔻��Ƃ���
		playerSphere.Radius /= 3.0f;
		// �G�̓����蔻��ƃv���C���[�̓����蔻�肪����������
		if (pEnemy->GetBoundingSphere().Intersects(playerSphere))
		{
			// �G�̓����蔻����擾�@
			auto& enemySphere = pEnemy->GetBoundingSphere();
			// �����߂��l��
			Vector3 newPos = CheckHitOtherObject(enemySphere, playerSphere);
			// �G�̈ʒu��ݒ�
			pEnemy->SetPosition(newPos);
		}
	}
	// �G���v���C���[�ɓ����������ǂ����̃t���O��ݒ�
	pEnemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	// �v���C���[�̓����蔻���ݒ�
	pEnemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere());
}
/*
*	@brief	���S�����G���폜
*	@details �G�̃��X�g���玀�S�����G���폜����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void EnemyManager::RemoveDeadEnemies()
{
	// remove_if�ō폜���ׂ��G�����Ɋ񂹂āAerase�ł܂Ƃ߂č폜
	m_pEnemies.erase(
		std::remove_if(
			m_pEnemies.begin(),
			m_pEnemies.end(),
			[this](std::unique_ptr<IEnemy>& pEnemy)
			{
				// �G���|����Ă�����
				if (pEnemy->GetEnemyIsDead())
				{
					// ���S���������s
					HandleEnemyDeath(pEnemy);

					// ���̓G�͍폜�ΏۂȂ̂�true��Ԃ�
					return true;
				}
				// �����Ă���G�͂��̂܂܎c���Ă���
				return false;
			}
		),
		m_pEnemies.end() // remove_if�Ō��Ɋ񂹂��폜�Ώۂ��܂Ƃ߂ď���
	);

}

/*
*	@brief	�G�̎��S����
*	@details �G�����S�������̏������s��
*	@param	std::unique_ptr<IEnemy>& pEnemy �G�̃|�C���^
*	@return �Ȃ�
*/
void EnemyManager::HandleEnemyDeath(std::unique_ptr<IEnemy>& pEnemy)
{
	// �G�t�F�N�g�̃X�P�[����ݒ肷�邽�߂̕ϐ�
	float effectScale;
	// IEnemy����Boss�̃|�C���^�[�𒊏o
	auto pBoss = dynamic_cast<BossBase*>(pEnemy.get());
	// �������o�����|�C���^�[���{�X��������
	if (pBoss)
	{
		// �{�X�̏ꍇ�̓G�t�F�N�g�̃X�P�[����傫������
		effectScale = pBoss->GetDeadEffectSize();
		// �����t���O��false�ɂ���
		m_isBossAlive = false;
	}
	else // �����{�X����Ȃ�������
	{
		// �U�R�G�̏ꍇ�̓G�t�F�N�g�̃X�P�[��������������
		effectScale = EnemyParameters::ENEMY_DEADEFFECT_SCALE;
	}
	// �G�t�F�N�g�𐶐����ă��X�g�ɒǉ�
	m_pEffects.push_back(std::make_unique<Effect>(m_pCommonResources,// �G�t�F�N�g�𐶐�
		Effect::EffectType::ENEMY_DEAD,// �G�t�F�N�g�̎�ގw��
		pEnemy->GetPosition(), // ���W�ݒ�
		effectScale,// �X�P�[���ݒ�
		pEnemy->GetMatrix()));// ���[���h�s��쐬
	// �G�̎��S�����Đ�
	m_pCommonResources->GetAudioManager()->PlaySound("EnemyDead", m_pPlayer->GetVolume() + m_pPlayer->GetVolumeCorrection());
}