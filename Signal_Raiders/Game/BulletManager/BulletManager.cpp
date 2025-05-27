/*
*	@file	BulletManager.cpp
*	@brief	�e�Ǘ��N���X
*/
#include "pch.h"
#include "BulletManager.h"
/*
*	@brief	�R���X�g���N�^
*	@details	�e�Ǘ��N���X�̏�����
*	@param	commonResources	���ʃ��\�[�X
*/
BulletManager::BulletManager(CommonResources* commonResources)
	:m_pCommonResources(commonResources)// ���ʃ��\�[�X
	, m_pPlayer(nullptr) // �v���C���[
	, m_pEnemyManager(nullptr) // �G�Ǘ�
	, m_pShooter(nullptr) // ���ˌ�
	, m_isPlayerShoot(false) // �v���C���[�����������ǂ���
	, m_isReloading(false) // �����[�h�����ǂ���
	, m_enemyBulletType(BulletType::NORMAL) // �G�̒e�̎��
	, m_enemyBulletSize(0.0f) // �G�̒e�̃T�C�Y
	, m_playerBulletCount(0) // �v���C���[�̒e�̐�
	, m_reloadTimer(0.0f) // �����[�h�^�C�}�[
	, m_elapsedTime(0.0f) // �o�ߎ���
	, m_specialAttackCount(0) // ����U���̐�
{
}
/*
*	@brief	�f�X�g���N�^
*	@details	�e�Ǘ��N���X�̏I������
*	@param	�Ȃ�
*	@return �Ȃ�
*/
BulletManager::~BulletManager()
{
	for (auto& bullet : m_pPlayerBullets)bullet.reset();	// �v���C���[�̒e���폜
	m_pPlayerBullets.clear();
	for (auto& bullet : m_pEnemyBullets)bullet.reset();	// �G�̒e���폜
	m_pEnemyBullets.clear();
}
/*
*	@brief	������
*	@details	�e�Ǘ��N���X�̏�����
*	@param	pPlayer		�v���C���[
*	@param	pEnemies	�G�Ǘ�
*	@return	�Ȃ�
*/
void BulletManager::Initialize(Player* pPlayer, EnemyManager* pEnemies)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���擾
	m_pEnemyManager = pEnemies;// �G�S�̂̃|�C���^�[���擾
	m_playerBulletCount = BulletParameters::MAX_PLAYER_BULLET_COUNT;// �v���C���[�̒e�̐���ݒ�
}

/*
*	@brief	�X�V
*	@details	�e�Ǘ��N���X�̍X�V����
*	@param	elapsedTime	�o�ߎ���
*	@return	�Ȃ�
*/
void BulletManager::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime;// �o�ߎ��Ԃ��X�V
	UpdatePlayerBullets(elapsedTime);// �v���C���[�̒e���X�V
	for (auto& enemy : m_pEnemyManager->GetEnemies())UpdateEnemyBullets(elapsedTime, enemy);	// �G�̒e���X�V
	CheckCollisionWithBullets();	// �e���m�̓����蔻��

}

/*
*	@brief	�`��
*	@details �e�Ǘ��N���X�̕`�揈��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BulletManager::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto camera = m_pPlayer->GetCamera();// �J�������擾
	Matrix view = camera->GetViewMatrix();// �r���[�s��
	Matrix proj = camera->GetProjectionMatrix();// �v���W�F�N�V�����s��
	for (const auto& bullet : m_pPlayerBullets)// �v���C���[�e�̕`��
	{
		bullet->RenderShadow(view, proj);// �e�̕`��
		bullet->Render(view, proj);// �e�̕`��
	}
	for (const auto& bullet : m_pEnemyBullets)// �G�e�̕`��
	{
		bullet->RenderShadow(view, proj);// �e�̕`��
		bullet->Render(view, proj);// �e�̕`��
	}
}

/*
*	@brief	�v���C���[�̒e�𐶐�
*	@details	�v���C���[�̒e�𐶐�����
*	@param	position	�����ʒu
*	@param	direction	�i�s����
*	@return	�Ȃ�
*/
void BulletManager::CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{
	auto bullet = std::make_unique<PlayerBullet>();// �v���C���[�e�̐���
	bullet->Initialize(m_pCommonResources);// ������
	bullet->MakeBall(position, direction);// �e�𐶐�
	m_pPlayerBullets.push_back(std::move(bullet));// �v���C���[�e��ǉ�
	SetIsPlayerShoot(true);// �e�����t���O�𗧂Ă�
	m_pCommonResources->GetAudioManager()->PlaySound("Shoot", m_pPlayer->GetVolume());	// SE�̍Đ�
}

/*
*	@brief	�G�̒e�𐶐�
*	@details	�G�̒e�𐶐�����
*	@param	position	�����ʒu
*	@param	direction	�i�s����
*	@return	�Ȃ�
*/
void BulletManager::CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto camera = m_pPlayer->GetCamera();	//	�J�������擾
	Vector3 playerPos = camera->GetEyePosition();	// �v���C���[����J�����̏����擾
	if (m_enemyBulletType == BulletType::SPECIAL)	// �e������e�Ȃ�
	{
		for (int i = 0; i < GetSpecialAttackCount(); i++)// ����U���̐�������]�e�𐶐�
		{
			auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);// �G�e�̐���
			bullet->SetSEVolume(m_pPlayer->GetVolume());// SE���ʂ�ݒ�
			bullet->SetBulletType(m_enemyBulletType);// �e�̎�ސݒ�
			bullet->Initialize(m_pCommonResources);// ������
			bullet->SetShooter(m_pShooter);// ���ˌ���ݒ�
			bullet->SetAngle(XM_2PI / GetSpecialAttackCount() * i);// �p�x��ݒ�
			bullet->SetDistance(0.0f);// ������ݒ�
			bullet->SetIsExpand(true);// �W�J�t���O�𗧂Ă�
			bullet->MakeBall(position, direction, playerPos);// �e�𐶐�
			m_pEnemyBullets.push_back(std::move(bullet));// �G�e��ǉ�
		}
	}
	else // ��]�e�ȊO�̒e�̏ꍇ
	{
		auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);// �G�e�̐���
		bullet->SetBulletType(m_enemyBulletType);// �e�̎�ސݒ�
		bullet->Initialize(m_pCommonResources);// ������
		bullet->MakeBall(position, direction, playerPos);// �e�𐶐�
		bullet->SetShooter(m_pShooter);// ���ˌ���ݒ�
		m_pEnemyBullets.push_back(std::move(bullet));// �G�e��ǉ�
	}
}

/*
*	@brief	�v���C���[�̒e���[
*	@details	�v���C���[�̒e���[����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BulletManager::ReLoadPlayerBullet()
{
	m_reloadTimer += m_elapsedTime;// �����[�h�^�C�}�[���X�V
	if (m_playerBulletCount < BulletParameters::MAX_PLAYER_BULLET_COUNT &&
		m_reloadTimer >= BulletParameters::RELOAD_INTERVAL)m_playerBulletCount++;// �v���C���[�̒e���[
	if (m_reloadTimer > BulletParameters::RELOAD_INTERVAL)m_reloadTimer = 0.0f;// �����[�h�^�C�}�[�������[�h�Ԋu�𒴂����烊�Z�b�g
	if (m_playerBulletCount == BulletParameters::MAX_PLAYER_BULLET_COUNT)m_isReloading = false;// �����[�h�����������烊���[�h���t���O��������
}

/*
*	@brief	�v���C���[�̒e������
*	@details	�v���C���[�̒e�������
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BulletManager::ConsumePlayerBullet()
{
	if (m_playerBulletCount > 0)m_playerBulletCount--;	// �v���C���[�̒e������
}

/*
*	@brief	�ǉ��_���[�W��ݒ�
*	@details	�v���C���[�̒e�ɒǉ��_���[�W��ݒ肷��
*	@param	additionalDamage	�ǉ��_���[�W
*	@return	�Ȃ�
*/
void BulletManager::SetAdditionalDamage(int additionalDamage)
{
	for (auto& bullet : m_pPlayerBullets)bullet->SetAdditionalDamage(additionalDamage);// �ǉ��_���[�W��ݒ�
}

/*
*	@brief	�v���C���[�̒e���X�V
*	@details	�v���C���[�̒e���X�V����
*	@param	elapsedTime	�o�ߎ���
*	@return	�Ȃ�
*/
void BulletManager::UpdatePlayerBullets(float elapsedTime)
{
	auto camera = m_pPlayer->GetCamera();// �J�������擾
	for (auto it = m_pPlayerBullets.begin(); it != m_pPlayerBullets.end();)
	{
		auto& bullet = *it;// �e���擾
		bullet->SetCameraEye(camera->GetEyePosition());// �J�����̈ʒu
		bullet->SetCameraTarget(camera->GetTargetPosition());// �J�����̒����_
		bullet->SetCameraUp(camera->GetUpVector());// �J�����̏����
		bullet->Update(elapsedTime);// �e���X�V
		if (CheckCollisionWithEnemies(bullet) ||// �G�Ƃ̓����蔻��
			bullet->IsExpired() ||// �������}������
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// �n�ʂɒ�������
		{
			it = m_pPlayerBullets.erase(it);// �폜
		}
		else
		{
			++it;// ���̗v�f��
		}
	}
}

/*
*	@brief	�G�̒e���X�V
*	@details	�G�̒e���X�V����
*	@param	elapsedTime	�o�ߎ���
*	@param	enemy		�G
*	@return	�Ȃ�
*/
void BulletManager::UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy)
{
	auto camera = enemy->GetCamera();// �J�������擾
	for (auto it = m_pEnemyBullets.begin(); it != m_pEnemyBullets.end();)
	{
		auto& bullet = *it;// �e���擾
		IEnemy* shooter = bullet->GetShooter();// ���ˌ����擾
		if (!shooter) { it = m_pEnemyBullets.erase(it); continue; }	// ���ˌ������݂��Ȃ��ꍇ�͍폜
		if (shooter != enemy.get()) { ++it; continue; }	// ���ˌ��������łȂ��ꍇ�͎��̗v�f��
		bullet->SetCameraEye(camera->GetEyePosition());// �J�����̈ʒu
		bullet->SetCameraTarget(camera->GetTargetPosition());// �J�����̒����_
		bullet->SetCameraUp(camera->GetUpVector());// �J�����̏����
		bullet->SetEnemyPosition(shooter->GetPosition());// �G�̈ʒu
		bullet->SetCurrentTarget(m_pPlayer->GetPlayerPos());// �v���C���[�̈ʒu
		bullet->Update(elapsedTime);// �e���X�V
		if (CheckCollisionWithPlayer(bullet, enemy) ||// �v���C���[�Ƃ̓����蔻��
			bullet->IsExpired() ||// �������}������
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// �n�ʂɒ�������
		{
			it = m_pEnemyBullets.erase(it);// �폜
		}
		else ++it;// ���̗v�f��
	}
}



/*
*	@brief	�v���C���[�̒e���G�ɓ����������ǂ���
*	@details	�v���C���[�̒e�ƓG�̓����蔻����s��
*	@param	bullet	�`�F�b�N�Ώۂ̃v���C���[�̒e
*	@return	�������true�A������Ȃ����false
*/
bool BulletManager::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
{
	for (auto& enemy : m_pEnemyManager->GetEnemies())	// �G�Ƃ̓����蔻��
	{
		// �G�ƃv���C���[�̒e�̓����蔻��
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			if (enemy->GetCanAttack() == true)// �U���\�ȓG�̏ꍇ
				enemy->ApplyDamageToEnemy(bullet->Damage());// �G��HP�����炷
			float effectSize = dynamic_cast<Boss*>(enemy.get()) ? 10.0f : 3.0f;// �G�t�F�N�g��ǉ��@�{�X�̏ꍇ�͑傫�߂ɂ���
			m_pEnemyManager->GetEffects().push_back(std::make_unique<Effect>(// �G�t�F�N�g��ǉ�
				m_pCommonResources,// ���ʃ��\�[�X
				Effect::EffectType::ENEMY_HIT,// �G�t�F�N�g�̎��
				enemy->GetPosition(),// �ʒu
				effectSize,// �T�C�Y
				enemy->GetMatrix()));// �s��
			enemy->SetEnemyHitByPlayerBullet(true);// �v���C���[�̒e���G�ɓ���������q�b�g�t���O�𗧂Ă�
			m_pCommonResources->GetAudioManager()->PlaySound("Hit", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);// SE���Đ�
			return true;// ����������true��Ԃ�
		}
	}
	return false;// ������Ȃ�������false��Ԃ�
}


/*
*	@brief	�v���C���[�ƓG�̒e�̓����蔻��
*	@details	�v���C���[�ƓG�̒e�̓����蔻����s��
*	@param	bullet	�`�F�b�N�Ώۂ̓G�̒e
*	@param	enemy	�G
*	@return	�������true�A������Ȃ����false
*/
bool BulletManager::CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy)
{
	enemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());// �v���C���[�Ƃ̓����蔻��
	if (enemy->GetBulletBoundingSphere().Intersects(enemy->GetPlayerBoundingSphere()))// �v���C���[�ƓG�̒e�̓����蔻��
	{
		enemy->SetPlayerHitByEnemyBullet(true);// �v���C���[���G�̒e�ɓ��������t���O�𗧂Ă�
		enemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetDirection());// �v���C���[�̒e�̕������擾
		enemy->GetPlayer()->SetisPlayEffect(true);// �v���C���[���G�t�F�N�g���Đ�����t���O�𗧂Ă�
		enemy->GetPlayer()->SetisPlayerDamage(true);// �v���C���[���_���[�W���󂯂��t���O�𗧂Ă�
		return true; // �Փ˂�����true��Ԃ�
	}
	return false; // �Փ˂��Ȃ����false��Ԃ�}
}

/*
*	@brief	�e���m�̓����蔻��
*	@details	�G�̒e�ƃv���C���[�̒e�̓����蔻����s��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BulletManager::CheckCollisionWithBullets()
{
	// �폜�Ώۂ̒e���L�^����u�Z�b�g�v(�|�C���^�[��������΂悭�A�l�͕s�v�Ȃ���unordered_set���g�p)
	std::unordered_set<PlayerBullet*> playerBulletsToRemove;// �v���C���[�̒e
	std::unordered_set<EnemyBullet*> enemyBulletsToRemove;// �G�̒e
	for (auto& playerBullet : m_pPlayerBullets)	// �v���C���[�̒e�ƓG�̒e�̏Փ˔���(��������)
	{
		for (auto& enemyBullet : m_pEnemyBullets)
		{
			if (playerBullet->GetBoundingSphere().Intersects(enemyBullet->GetBoundingSphere()))	// �Փ˂��������ꍇ�A�폜�Ώۂɒǉ�
			{
				m_pCommonResources->GetAudioManager()->PlaySound("BulletCollision", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);// �e���m�̏Փˉ����Đ�
				enemyBulletsToRemove.insert(enemyBullet.get());	// �G�̒e������
				playerBulletsToRemove.insert(playerBullet.get());// �v���C���[�̒e������
			}
		}
	}
	m_pEnemyBullets.erase(	// �܂Ƃ߂ď���(�G�̒e)
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),
			[&enemyBulletsToRemove](const std::unique_ptr<EnemyBullet>& bullet)
			{
				return enemyBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pEnemyBullets.end()
	);
	m_pPlayerBullets.erase(	// �܂Ƃ߂ď���(�v���C���[�̒e)
		std::remove_if(m_pPlayerBullets.begin(), m_pPlayerBullets.end(),
			[&playerBulletsToRemove](const std::unique_ptr<PlayerBullet>& bullet)
			{
				return playerBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pPlayerBullets.end()
	);
}

/*
*	@brief	�w�肵���G�̒e�����ׂč폜�i�G�����S�����Ƃ��Ȃǁj
*	@details	�w�肵���G�����˂����e�����ׂč폜����
*	@param	shooter	�폜�Ώۂ̓G
*	@return	�Ȃ�
*/
void BulletManager::RemoveBulletsByShooter(IEnemy* shooter)
{
	if (!shooter || m_pEnemyBullets.empty())	return;// ���ˌ��� nullptr �܂��͖����Ȃ牽�����Ȃ�
	m_pEnemyBullets.erase(	// ���ˌ�����v����e�����ׂč폜
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),// �폜�Ώۂ�T��
			[shooter](const std::unique_ptr<EnemyBullet>& bullet)
			{
				IEnemy* bulletShooter = bullet->GetShooter();// �e���������G���擾
				return (bulletShooter == shooter || bulletShooter == nullptr);// ���ˌ�����v���邩�A�����Ȃ�폜�Ώ�
			}),
		m_pEnemyBullets.end()
	);
}
