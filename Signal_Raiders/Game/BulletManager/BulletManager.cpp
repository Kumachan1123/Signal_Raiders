/*
	@file	BulletManager.cpp
	@brief	�e�Ǘ��N���X
*/
#include "pch.h"
#include "BulletManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �e(Bullet)�̊Ǘ��N���X
/// �v���C���[�̒e�ƓG�̒e���Ǘ�����
/// </summary>
/// <param name="commonResources">
/// �Q�[���S�̂ŋ��L���郊�\�[�X
/// </param>
BulletManager::BulletManager(CommonResources* commonResources)
	:m_commonResources{ commonResources } // ���ʃ��\�[�X
	, m_pPlayer{ nullptr } // �v���C���[
	, m_pEnemyManager{ nullptr } // �G�Ǘ�
	, m_pShooter{ nullptr } // ���ˌ�
	, m_isPlayerShoot{ false } // �v���C���[�����������ǂ���
	, m_isReloading{ false } // �����[�h�����ǂ���
	, m_audioManager{ AudioManager::GetInstance() } // �I�[�f�B�I�}�l�[�W���[
	, m_enemyBulletType{ EnemyBullet::BulletType::STRAIGHT } // �G�̒e�̎��
	, m_enemyBulletSize{ 0.0f } // �G�̒e�̃T�C�Y
	, m_playerBulletCount{ 0 } // �v���C���[�̒e�̐�
	, m_reloadTimer{ 0.0f } // �����[�h�^�C�}�[
	, m_elapsedTime{ 0.0f } // �o�ߎ���
	, m_specialAttackCount{ 0 } // ����U���̐�
{
}
/// <summary>
/// �e(Bullet)�̊Ǘ��N���X
/// �v���C���[�̒e�ƓG�̒e���Ǘ�����
/// </summary>
BulletManager::~BulletManager()
{
	// �v���C���[�̒e���폜
	for (auto& bullet : m_playerBullets)bullet.reset();
	m_playerBullets.clear();
	// �G�̒e���폜
	for (auto& bullet : m_enemyBullets)bullet.reset();
	m_enemyBullets.clear();
}
/// <summary>
/// �e�̏�����
/// </summary>
/// <param name="pPlayer">�v���C���[�̃|�C���^�[ </param>
/// <param name="pEnemies">�G�}�l�[�W���[�̃|�C���^�[</param>
void BulletManager::Initialize(Player* pPlayer, EnemyManager* pEnemies)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���擾
	m_pEnemyManager = pEnemies;// �G�S�̂̃|�C���^�[���擾
	// ���ʉ��̏�����
	SetSound();
	// �v���C���[�̒e�̏�����
	m_playerBulletCount = BulletParameters::MAX_PLAYER_BULLET_COUNT;
}
/// <summary>
/// �e�̍X�V
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
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

/// <summary>
/// �e�̕`��
/// </summary>
void BulletManager::Render()
{
	auto camera = m_pPlayer->GetCamera();// �J�������擾
	Matrix view = camera->GetViewMatrix();// �r���[�s��
	Matrix proj = camera->GetProjectionMatrix();// �v���W�F�N�V�����s��
	// �v���C���[�e�̕`��
	for (const auto& bullet : m_playerBullets)
	{
		bullet->RenderShadow(view, proj);// �e�̕`��
		bullet->Render(view, proj);// �e�̕`��
	}

	// �G�e�̕`��
	for (const auto& bullet : m_enemyBullets)
	{
		bullet->RenderShadow(view, proj);// �e�̕`��
		bullet->Render(view, proj);// �e�̕`��
	}
}

/// <summary>
/// �v���C���[�̒e�𐶐�
/// </summary>
/// <param name="position">�v���C���[�̍��W</param>
/// <param name="direction">�v���C���[�̌���</param>
void BulletManager::CreatePlayerBullet(const Vector3& position, Vector3& direction)
{
	// �v���C���[�e�̐���
	auto bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(m_commonResources);// ������
	bullet->MakeBall(position, direction);// �e�𐶐�
	m_playerBullets.push_back(std::move(bullet));// �v���C���[�e��ǉ�
	SetIsPlayerShoot(true);// �e�����t���O�𗧂Ă�
	// SE�̍Đ�
	m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());
}

/// <summary>
/// �G�̒e�𐶐�
/// </summary>
/// <param name="position">�G�̍��W</param>
/// <param name="direction">�G�̌���</param>
void BulletManager::CreateEnemyBullet(const Vector3& position, Vector3& direction)
{
	//	�J�������擾
	auto camera = m_pPlayer->GetCamera();
	// �v���C���[����J�����̏����擾
	Vector3 playerPos = camera->GetEyePosition();
	// ����U���̐�������]�e�𐶐�
	if (m_enemyBulletType == EnemyBullet::BulletType::SPIRAL)
	{
		for (int i = 0; i < GetSpecialAttackCount(); i++)
		{
			auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);// �G�e�̐���
			bullet->Initialize(m_commonResources, m_enemyBulletType);// ������
			bullet->MakeBall(position, direction, playerPos);// �e�𐶐�
			bullet->SetShooter(m_pShooter);// ���ˌ���ݒ�
			bullet->SetAngle(XM_2PI / GetSpecialAttackCount() * i);// �p�x��ݒ�
			bullet->SetDistance(0.0f);// ������ݒ�
			bullet->SetIsExpand(true);// �W�J�t���O�𗧂Ă�
			m_enemyBullets.push_back(std::move(bullet));// �G�e��ǉ�
		}
	}
	else // ��]�e�ȊO�̒e�̏ꍇ
	{

		auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);// �G�e�̐���
		bullet->Initialize(m_commonResources, m_enemyBulletType);
		bullet->MakeBall(position, direction, playerPos);
		bullet->SetShooter(m_pShooter);
		m_enemyBullets.push_back(std::move(bullet));
	}

}

/// <summary>
/// �v���C���[�̒e�������[�h
/// </summary>
void BulletManager::ReLoadPlayerBullet()
{
	m_reloadTimer += m_elapsedTime;// �����[�h�^�C�}�[���X�V

	// �v���C���[�̒e���[
	if (m_playerBulletCount < BulletParameters::MAX_PLAYER_BULLET_COUNT &&
		m_reloadTimer >= BulletParameters::RELOAD_INTERVAL)
	{
		m_playerBulletCount++;// �e���[
	}
	// �����[�h�^�C�}�[�������[�h�Ԋu�𒴂����烊�Z�b�g
	if (m_reloadTimer > BulletParameters::RELOAD_INTERVAL)
	{
		m_reloadTimer = 0.0f;
	}
	// �����[�h�����������烊���[�h���t���O��������
	if (m_playerBulletCount == BulletParameters::MAX_PLAYER_BULLET_COUNT)
	{
		m_isReloading = false;
	}
}

/// <summary>
/// �v���C���[�̒e������
/// </summary>
void BulletManager::ConsumePlayerBullet()
{
	// �v���C���[�̒e������
	if (m_playerBulletCount > 0)
	{
		m_playerBulletCount--;
	}
}

/// <summary>
/// �ǉ��_���[�W��ݒ�
/// </summary>
/// <param name="additionalDamage">�ǉ��_���[�W��</param>
void BulletManager::SetAdditionalDamage(int additionalDamage)
{
	for (auto& bullet : m_playerBullets)
	{
		bullet->SetAdditionalDamage(additionalDamage);// �ǉ��_���[�W��ݒ�
	}
}

/// <summary>
/// �v���C���[�̒e���X�V
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void BulletManager::UpdatePlayerBullets(float elapsedTime)
{
	//	�J�������擾
	auto camera = m_pPlayer->GetCamera();
	for (auto it = m_playerBullets.begin(); it != m_playerBullets.end();)
	{
		auto& bullet = *it;// �e���擾
		// �J��������e�ɓK�p
		bullet->SetCameraEye(camera->GetEyePosition());// �J�����̈ʒu
		bullet->SetCameraTarget(camera->GetTargetPosition());// �J�����̒����_
		bullet->SetCameraUp(camera->GetUpVector());// �J�����̏����

		// �e���X�V
		bullet->Update(elapsedTime);
		// �e���������}���邩�n�ʂɒ�������폜
		if (CheckCollisionWithEnemies(bullet) ||
			bullet->IsExpired() ||
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)
		{
			it = m_playerBullets.erase(it);// �폜
		}
		else
		{
			++it;// ���̗v�f��
		}
	}

}

/// <summary>
/// �G�̒e���X�V
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="enemy">�G�̃|�C���^�[</param>
void BulletManager::UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy)
{
	//	�J�������擾
	auto camera = enemy->GetCamera();
	for (auto it = m_enemyBullets.begin(); it != m_enemyBullets.end();)
	{
		auto& bullet = *it;// �e���擾
		// ���ˌ��̓G���擾
		IEnemy* shooter = bullet->GetShooter();
		// ���ˌ������݂��Ȃ��ꍇ�͍폜
		if (!shooter) { it = m_enemyBullets.erase(it); continue; }
		// ���ˌ����G�łȂ��ꍇ�͍폜
		if (shooter != enemy.get()) { ++it; continue; }

		// �e�ɓK�؂ȏ���K�p
		bullet->SetCameraEye(camera->GetEyePosition());// �J�����̈ʒu
		bullet->SetCameraTarget(camera->GetTargetPosition());// �J�����̒����_
		bullet->SetCameraUp(camera->GetUpVector());// �J�����̏����
		bullet->SetEnemyPosition(shooter->GetPosition());// �G�̈ʒu
		bullet->SetCurrentTarget(m_pPlayer->GetPlayerPos());// �v���C���[�̈ʒu
		bullet->Update(elapsedTime);// �e���X�V
		// �v���C���[�Ƃ̓����蔻��
		if (CheckCollisionWithPlayer(bullet, enemy) ||
			bullet->IsExpired() ||
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)
		{
			it = m_enemyBullets.erase(it);// �폜
		}
		else
		{
			++it;// ���̗v�f��
		}
	}
}

/// <summary>
/// �I�[�f�B�I�}�l�[�W���[�̏�������SE�̓ǂݍ���
/// </summary>
void BulletManager::SetSound()
{
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");// �v���C���[�̒e��SE
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");// �q�b�g��SE
}

/// <summary>
/// �v���C���[�̒e���G�ɓ����������ǂ���
/// </summary>
/// <param name="bullet">�`�F�b�N�Ώۂ̃v���C���[�̒e</param>
/// <returns>�������true�A������Ȃ����false</returns>
bool BulletManager::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
{
	// �G�Ƃ̓����蔻��
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		// �G�ƃv���C���[�̒e�̓����蔻��
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			if (enemy->GetCanAttack() == true)// �U���\�ȓG�̏ꍇ
				enemy->SetEnemyHP(bullet->Damage());// �G��HP�����炷
			// �G�t�F�N�g��ǉ�
			float effectSize = dynamic_cast<Boss*>(enemy.get()) ? 10.0f : 3.0f;// �{�X�̏ꍇ�͑傫�߂�
			m_pEnemyManager->GetEffect().push_back(std::make_unique<Effect>(
				m_commonResources,
				Effect::EffectType::ENEMY_HIT,
				enemy->GetPosition(),
				effectSize,
				enemy->GetMatrix()));
			// �v���C���[�̒e���G�ɓ��������t���O�𗧂Ă�
			enemy->SetEnemyHitByPlayerBullet(true);// �q�b�g�t���O
			m_audioManager->PlaySound("Hit", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);// SE���Đ�
			return true;// ����������true��Ԃ�
		}
	}
	return false;// ������Ȃ�������false��Ԃ�
}

/// <summary>
/// �G�̒e���v���C���[�ɓ����������ǂ���
/// </summary>
/// <param name="bullet">�`�F�b�N�Ώۂ̓G�̒e</param>
/// <param name="enemy">�G�̃|�C���^�[</param>
/// <returns>�������true�A������Ȃ����false</returns>
bool BulletManager::CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy)
{
	// �v���C���[�Ƃ̓����蔻��
	enemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
	// �v���C���[�ƓG�̒e�̓����蔻��
	if (enemy->GetBulletBoundingSphere().Intersects(enemy->GetPlayerBoundingSphere()))
	{
		enemy->SetPlayerHitByEnemyBullet(true);// �v���C���[���G�̒e�ɓ��������t���O�𗧂Ă�
		enemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetBulletDirection());// �v���C���[�̒e�̕������擾
		enemy->GetPlayer()->SetisPlayEffect(true);// �v���C���[���G�t�F�N�g���Đ�����t���O�𗧂Ă�
		enemy->GetPlayer()->SetisPlayerDamage(true);// �v���C���[���_���[�W���󂯂��t���O�𗧂Ă�
		return true; // �Փ˂�����true��Ԃ�
	}
	return false; // �Փ˂��Ȃ����false��Ԃ�}

}

/// <summary>
/// �e���m�̓����蔻��
/// </summary>
void BulletManager::CheckCollisionWithBullets()
{
	// �폜�Ώۂ̒e���L�^����u�Z�b�g�v(�|�C���^�[��������΂悭�A�l�͕s�v�Ȃ���unordered_set���g�p)
	std::unordered_set<PlayerBullet*> playerBulletsToRemove;// �v���C���[�̒e
	std::unordered_set<EnemyBullet*> enemyBulletsToRemove;// �G�̒e

	// �v���C���[�̒e�ƓG�̒e�̏Փ˔���(��������)
	for (auto& playerBullet : m_playerBullets)
	{
		for (auto& enemyBullet : m_enemyBullets)
		{
			// �e���m�̓����蔻��
			if (playerBullet->GetBoundingSphere().Intersects(enemyBullet->GetBoundingSphere()))
			{
				// �Փ˂��������ꍇ�A�폜�Ώۂɒǉ�
				enemyBulletsToRemove.insert(enemyBullet.get());
				playerBulletsToRemove.insert(playerBullet.get());
			}
		}
	}

	// �܂Ƃ߂ď���(�G�̒e)
	m_enemyBullets.erase(
		std::remove_if(m_enemyBullets.begin(), m_enemyBullets.end(),
			[&enemyBulletsToRemove](const std::unique_ptr<EnemyBullet>& bullet)
			{
				return enemyBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_enemyBullets.end()
	);

	// �܂Ƃ߂ď���(�v���C���[�̒e)
	m_playerBullets.erase(
		std::remove_if(m_playerBullets.begin(), m_playerBullets.end(),
			[&playerBulletsToRemove](const std::unique_ptr<PlayerBullet>& bullet)
			{
				return playerBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_playerBullets.end()
	);

}

/// <summary>
/// �e�̔��ˌ������񂾂Ƃ��A�e�����ׂč폜
/// </summary>
/// <param name="shooter">���ˌ��̓G�̃|�C���^�[</param>
void BulletManager::RemoveBulletsByShooter(IEnemy* shooter)
{
	// ���ˌ��� nullptr �܂��͖����Ȃ牽�����Ȃ�
	if (!shooter || m_enemyBullets.empty())
		return;
	// ���ˌ�����v����e�����ׂč폜
	m_enemyBullets.erase(
		std::remove_if(m_enemyBullets.begin(), m_enemyBullets.end(),
			[shooter](const std::unique_ptr<EnemyBullet>& bullet)
			{
				IEnemy* bulletShooter = bullet->GetShooter();
				return (bulletShooter == shooter || bulletShooter == nullptr);
			}),
		m_enemyBullets.end()
	);
}
