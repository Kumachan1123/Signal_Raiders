/*
*	@file	BulletManager.cpp
*	@brief	�e�Ǘ��N���X
*/
#include <pch.h>
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
	// �v���C���[�̒e���폜
	for (auto& bullet : m_pPlayerBullets)bullet.reset();
	// �v���C���[�̒e���N���A
	m_pPlayerBullets.clear();
	// �G�̒e���폜
	for (auto& bullet : m_pEnemyBullets)bullet.reset();
	// �G�̒e���N���A
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
	// �v���C���[�̃|�C���^�[���擾
	m_pPlayer = pPlayer;
	// �G�S�̂̃|�C���^�[���擾
	m_pEnemyManager = pEnemies;
	// �v���C���[�̒e�̐���ݒ�
	m_playerBulletCount = BulletParameters::MAX_PLAYER_BULLET_COUNT;
}

/*
*	@brief	�X�V
*	@details	�e�Ǘ��N���X�̍X�V����
*	@param	elapsedTime	�o�ߎ���
*	@return	�Ȃ�
*/
void BulletManager::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ��X�V
	m_elapsedTime = elapsedTime;
	// �v���C���[�̒e���X�V
	UpdatePlayerBullets(elapsedTime);
	// �G�̒e���X�V
	for (auto& enemy : m_pEnemyManager->GetEnemies())UpdateEnemyBullets(elapsedTime, enemy);
	// �e���m�̓����蔻��
	CheckCollisionWithBullets();

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
	// �J�������擾
	auto camera = m_pPlayer->GetCamera();
	// �r���[�s��
	Matrix view = camera->GetViewMatrix();
	// �v���W�F�N�V�����s��
	Matrix proj = camera->GetProjectionMatrix();
	// �v���C���[�e�̕`��
	for (const auto& bullet : m_pPlayerBullets)
	{
		// �e�̕`��
		bullet->RenderShadow(view, proj);
		// �e�̕`��
		bullet->Render(view, proj);
	}
	// �G�e�̕`��
	for (const auto& bullet : m_pEnemyBullets)
	{
		// �e�̕`��
		bullet->RenderShadow(view, proj);
		// �e�̕`��
		bullet->Render(view, proj);
	}
}

/*
*	@brief	�v���C���[�̒e�𐶐�
*	@details	�v���C���[�̒e�𐶐�����
*	@param	position	�����ʒu
*	@param	direction	�i�s����
*	@return	�Ȃ�
*/
void BulletManager::CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction)
{
	// �v���C���[�e�̐���
	auto bullet = std::make_unique<PlayerBullet>();
	// ������
	bullet->Initialize(m_pCommonResources);
	// �e�𐶐�
	bullet->MakeBall(position, direction);
	// �v���C���[�e��ǉ�
	m_pPlayerBullets.push_back(std::move(bullet));
	// �e�����t���O�𗧂Ă�
	SetIsPlayerShoot(true);
	// SE�̍Đ�
	m_pCommonResources->GetAudioManager()->PlaySound("Shoot", m_pPlayer->GetVolume());
}

/*
*	@brief	�G�̒e�𐶐�
*	@details	�G�̒e�𐶐�����
*	@param	position	�����ʒu
*	@param	direction	�i�s����
*	@return	�Ȃ�
*/
void BulletManager::CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	�J�������擾
	auto camera = m_pPlayer->GetCamera();
	// �v���C���[����J�����̏����擾
	Vector3 playerPos = camera->GetEyePosition();
	// �e������e�Ȃ�
	if (m_enemyBulletType == BulletType::SPECIAL)
	{
		// ����U���̐�������]�e�𐶐�
		for (int i = 0; i < GetSpecialAttackCount(); i++)
		{
			// �G�e�̐���
			auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);
			// SE���ʂ�ݒ�
			bullet->SetSEVolume(m_pPlayer->GetVolume());
			// �e�̎�ސݒ�
			bullet->SetBulletType(m_enemyBulletType);
			// ������
			bullet->Initialize(m_pCommonResources);
			// ���ˌ���ݒ�
			bullet->SetShooter(m_pShooter);
			// �p�x��ݒ�
			bullet->SetAngle(XM_2PI / GetSpecialAttackCount() * i);
			// ������ݒ�
			bullet->SetDistance(0.0f);
			// �W�J�t���O�𗧂Ă�
			bullet->SetIsExpand(true);
			// �e�𐶐�
			bullet->MakeBall(position, direction, playerPos);
			// �G�e��ǉ�
			m_pEnemyBullets.push_back(std::move(bullet));
		}
	}
	else // ��]�e�ȊO�̒e�̏ꍇ
	{
		// �G�e�̐���
		auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);
		// �e�̎�ސݒ�
		bullet->SetBulletType(m_enemyBulletType);
		// ������
		bullet->Initialize(m_pCommonResources);
		// �e�𐶐�
		bullet->MakeBall(position, direction, playerPos);
		// ���ˌ���ݒ�
		bullet->SetShooter(m_pShooter);
		// �G�e��ǉ�
		m_pEnemyBullets.push_back(std::move(bullet));
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
	// �����[�h�^�C�}�[���X�V
	m_reloadTimer += m_elapsedTime;
	// �v���C���[�̒e���ő吔�����ŁA�����[�h�^�C�}�[�������[�h�Ԋu�𒴂�����
	if (m_playerBulletCount < BulletParameters::MAX_PLAYER_BULLET_COUNT && m_reloadTimer >= BulletParameters::RELOAD_INTERVAL)
	{
		// �v���C���[�̒e���[
		m_playerBulletCount++;
	}
	// �����[�h�^�C�}�[�������[�h�Ԋu�𒴂�����
	if (m_reloadTimer > BulletParameters::RELOAD_INTERVAL)
	{
		m_reloadTimer = 0.0f;// �����[�h�^�C�}�[�����Z�b�g
	}
	// �����[�h������������
	if (m_playerBulletCount == BulletParameters::MAX_PLAYER_BULLET_COUNT)
	{
		// �����[�h���t���O��������
		m_isReloading = false;
	}
}

/*
*	@brief	�v���C���[�̒e������
*	@details	�v���C���[�̒e�������
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BulletManager::ConsumePlayerBullet()
{
	// �v���C���[�̒e��1�ȏ゠��ꍇ
	if (m_playerBulletCount > 0)
	{
		// �v���C���[�̒e������
		m_playerBulletCount--;
	}
}

/*
*	@brief	�ǉ��_���[�W��ݒ�
*	@details	�v���C���[�̒e�ɒǉ��_���[�W��ݒ肷��
*	@param	additionalDamage	�ǉ��_���[�W
*	@return	�Ȃ�
*/
void BulletManager::SetAdditionalDamage(int additionalDamage)
{
	// �v���C���[�̒e�ɒǉ��_���[�W��ݒ�
	for (auto& bullet : m_pPlayerBullets)
	{
		// �ǉ��_���[�W��ݒ�
		bullet->SetAdditionalDamage(additionalDamage);
	}
}

/*
*	@brief	�v���C���[�̒e���X�V
*	@details	�v���C���[�̒e���X�V����
*	@param	elapsedTime	�o�ߎ���
*	@return	�Ȃ�
*/
void BulletManager::UpdatePlayerBullets(float elapsedTime)
{
	// �J�������擾
	auto camera = m_pPlayer->GetCamera();
	// �v���C���[�̒e���X�V
	for (auto it = m_pPlayerBullets.begin(); it != m_pPlayerBullets.end();)
	{
		// �z�񂩂�e���擾
		auto& bullet = *it;
		// �J�����̈ʒu���擾���Ēe�ɐݒ�
		bullet->SetCameraEye(camera->GetEyePosition());
		// �J�����̒����_���擾���Ēe�ɐݒ�
		bullet->SetCameraTarget(camera->GetTargetPosition());
		// �J�����̏�������擾���Ēe�ɐݒ�
		bullet->SetCameraUp(camera->GetUpVector());
		// �e���X�V
		bullet->Update(elapsedTime);
		if (CheckCollisionWithEnemies(bullet) ||// �G�Ƃ̓����蔻���true��
			bullet->IsExpired() ||// �������}������
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// �n�ʂɒ�������
		{
			// �e���폜
			it = m_pPlayerBullets.erase(it);
		}
		else// ������Ȃ������ꍇ
		{
			// ���̗v�f��
			++it;
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
	// �J�������擾
	auto camera = enemy->GetCamera();
	// �G�̒e���X�V
	for (auto it = m_pEnemyBullets.begin(); it != m_pEnemyBullets.end();)
	{
		// �z�񂩂�e���擾
		auto& bullet = *it;
		// ���ˌ����擾
		IEnemy* shooter = bullet->GetShooter();
		// ���ˌ������݂��Ȃ��ꍇ�͍폜
		if (!shooter) { it = m_pEnemyBullets.erase(it); continue; }
		// ���ˌ��������łȂ��ꍇ�͎��̗v�f��
		if (shooter != enemy.get()) { ++it; continue; }
		// �J�����̈ʒu���擾���Ēe�ɐݒ�
		bullet->SetCameraEye(camera->GetEyePosition());
		// �J�����̒����_���擾���Ēe�ɐݒ�
		bullet->SetCameraTarget(camera->GetTargetPosition());
		// �J�����̏�������擾���Ēe�ɐݒ�
		bullet->SetCameraUp(camera->GetUpVector());
		// �G�̈ʒu���擾���Ēe�ɐݒ�
		bullet->SetEnemyPosition(shooter->GetPosition());
		// �e�̕������擾���Ēe�ɐݒ�
		bullet->SetCurrentTarget(m_pPlayer->GetPlayerPos());
		// �e���X�V
		bullet->Update(elapsedTime);
		if (CheckCollisionWithPlayer(bullet, enemy) ||// �v���C���[�Ƃ̓����蔻���true��
			bullet->IsExpired() ||// �������}������
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// �n�ʂɒ�������
		{
			// �폜
			it = m_pEnemyBullets.erase(it);
		}
		// ������Ȃ������ꍇ�͎��̗v�f��
		else ++it;
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
	// �G�Ƃ̓����蔻��
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		// �G�ƃv���C���[�̒e�̓����蔻��
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			// �U���\�ȓG�̏ꍇ
			if (enemy->GetCanAttack() == true)
			{
				// �G��HP�����炷
				enemy->ApplyDamageToEnemy(bullet->Damage());
			}
			// �G�t�F�N�g��ǉ��@�{�X�̏ꍇ�͑傫�߂ɂ���
			float effectSize = dynamic_cast<Boss*>(enemy.get()) ? 10.0f : 3.0f;
			// �G�t�F�N�g��ǉ�
			m_pEnemyManager->GetEffects().push_back(std::make_unique<Effect>(
				m_pCommonResources,// ���ʃ��\�[�X
				Effect::EffectType::ENEMY_HIT,// �G�t�F�N�g�̎��
				enemy->GetPosition(),// �ʒu
				effectSize,// �T�C�Y
				enemy->GetMatrix()));// �s��
			// �v���C���[�̒e���G�ɓ���������q�b�g�t���O�𗧂Ă�
			enemy->SetEnemyHitByPlayerBullet(true);
			// SE���Đ�
			m_pCommonResources->GetAudioManager()->PlaySound("Hit", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);
			// ����������true��Ԃ�
			return true;
		}
	}
	// ������Ȃ�������false��Ԃ�
	return false;
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
	// ���ˌ����������e�ɋ��E����ݒ�
	enemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
	// �v���C���[�ƓG�̒e�̓����蔻��
	if (enemy->GetBulletBoundingSphere().Intersects(enemy->GetPlayerBoundingSphere()))
	{
		// �v���C���[���G�̒e�ɓ��������t���O�𗧂Ă�
		enemy->SetPlayerHitByEnemyBullet(true);
		// �v���C���[�̒e�̕������擾
		enemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetDirection());
		// �v���C���[���G�t�F�N�g���Đ�����t���O�𗧂Ă�
		enemy->GetPlayer()->SetisPlayEffect(true);
		// �v���C���[���_���[�W���󂯂��t���O�𗧂Ă�
		enemy->GetPlayer()->SetisPlayerDamage(true);
		// �Փ˂�����true��Ԃ�
		return true;
	}
	// �Փ˂��Ȃ����false��Ԃ�
	return false;
}

/*
*	@brief	�e���m�̓����蔻��
*	@details	�G�̒e�ƃv���C���[�̒e�̓����蔻����s��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BulletManager::CheckCollisionWithBullets()
{
	// �폜�Ώۂ̒e���L�^����Z�b�g
	std::unordered_set<PlayerBullet*> playerBulletsToRemove;// �v���C���[�̒e
	std::unordered_set<EnemyBullet*> enemyBulletsToRemove;// �G�̒e
	// �v���C���[�̒e�ƓG�̒e�̏Փ˔���
	for (auto& playerBullet : m_pPlayerBullets)
	{
		// ��������ŏ���
		for (auto& enemyBullet : m_pEnemyBullets)
		{
			// �Փ˂��������ꍇ
			if (playerBullet->GetBoundingSphere().Intersects(enemyBullet->GetBoundingSphere()))
			{
				// �e���m�̏Փˉ����Đ�
				m_pCommonResources->GetAudioManager()->PlaySound("BulletCollision", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);
				// �G�̒e������
				enemyBulletsToRemove.insert(enemyBullet.get());
				// �v���C���[�̒e������
				playerBulletsToRemove.insert(playerBullet.get());
			}
		}
	}
	// �G�̒e���܂Ƃ߂ď���
	// enemyBulletsToRemove�ɍ폜�Ώۂ̒e�������Ă���̂ŁA������g���č폜����
	// std::remove_if���g���āA�폜�Ώۂ̒e�������č폜
	// �e�̎��̂� std::unique_ptr �Ȃ̂ŁA�|�C���^�[���g���Ĕ�r����
	m_pEnemyBullets.erase(
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),// �폜�Ώۂ�T��
			[&enemyBulletsToRemove](const std::unique_ptr<EnemyBullet>& bullet)// �����_���ŏ������w��
			{
				// �폜�Ώۂ̒e���Z�b�g�Ɋ܂܂�Ă��邩�`�F�b�N
				return enemyBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pEnemyBullets.end()// �폜�͈͂̏I�[
	);
	// �v���C���[�̒e���܂Ƃ߂ď���
	// playerBulletsToRemove�ɍ폜�Ώۂ̒e�������Ă���̂ŁA������g���č폜����
	// std::remove_if���g���āA�폜�Ώۂ̒e�������č폜
	// �e�̎��̂� std::unique_ptr �Ȃ̂ŁA�|�C���^�[���g���Ĕ�r����
	m_pPlayerBullets.erase(
		std::remove_if(m_pPlayerBullets.begin(), m_pPlayerBullets.end(),// �폜�Ώۂ�T��
			[&playerBulletsToRemove](const std::unique_ptr<PlayerBullet>& bullet)// �����_���ŏ������w��
			{
				// �폜�Ώۂ̒e���Z�b�g�Ɋ܂܂�Ă��邩�`�F�b�N
				return playerBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pPlayerBullets.end()// �폜�͈͂̏I�[
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
	// ���ˌ��� nullptr �܂��͖����Ȃ牽�����Ȃ�
	if (!shooter || m_pEnemyBullets.empty()) return;
	// ���ˌ�����v����e�����ׂč폜
	m_pEnemyBullets.erase(
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),// �폜�Ώۂ�T��
			[shooter](const std::unique_ptr<EnemyBullet>& bullet)// �����_���ŏ������w��
			{
				// �e���������G���擾
				IEnemy* bulletShooter = bullet->GetShooter();
				// ���ˌ�����v���邩�A�����Ȃ�폜�Ώ�
				return (bulletShooter == shooter || bulletShooter == nullptr);
			}),
		m_pEnemyBullets.end()// �폜�͈͂̏I�[
	);
}
