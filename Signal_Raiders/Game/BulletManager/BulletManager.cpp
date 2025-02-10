#include "pch.h"
#include "BulletManager.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

using namespace DirectX::SimpleMath;

// �R���X�g���N�^
BulletManager::BulletManager(CommonResources* commonResources)
	:m_commonResources{ commonResources }
	, m_pPlayer{ nullptr }
	, m_pEnemyManager{ nullptr }
	, m_pShooter{ nullptr }
	, m_audioManager{ AudioManager::GetInstance() }
	, m_enemyBulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_enemyBulletSize{ 0.0f }
	, m_playerBulletCount{ 0 }
	, m_reloadTimer{ 0.0f }
	, m_elapsedTime{ 0.0f }
{
}
// �f�X�g���N�^
BulletManager::~BulletManager()
{
	// �v���C���[�̒e���폜
	for (auto& bullet : m_playerBullets)bullet.reset();
	m_playerBullets.clear();
	// �G�̒e���폜
	for (auto& bullet : m_enemyBullets)bullet.reset();
	m_enemyBullets.clear();
}
// ������
void BulletManager::Initialize(Player* pPlayer, EnemyManager* pEnemies)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���擾
	m_pEnemyManager = pEnemies;// �G�S�̂̃|�C���^�[���擾

	// ���ʉ��̏�����
	SetSound();

	// �v���C���[�̒e�̏�����
	m_playerBulletCount = MAX_PLAYER_BULLET_COUNT;
}

// �X�V
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
// �`��
void BulletManager::Render()
{
	auto camera = m_pPlayer->GetCamera();
	Matrix view = camera->GetViewMatrix();
	Matrix proj = camera->GetProjectionMatrix();
	// �v���C���[�e�̕`��
	for (const auto& bullet : m_playerBullets)
	{
		bullet->Render(view, proj);
		bullet->RenderShadow(view, proj);
	}

	// �G�e�̕`��
	for (const auto& bullet : m_enemyBullets)
	{
		bullet->Render(view, proj);
		bullet->RenderShadow(view, proj);
	}
}
void BulletManager::CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{
	// �v���C���[�e�̐���
	auto bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(m_commonResources);
	bullet->MakeBall(position, direction);
	m_playerBullets.push_back(std::move(bullet));
	SetIsPlayerShoot(true);// �e�����t���O�𗧂Ă�
	// SE�̍Đ�
	m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());
}

// �e�𐶐�
void BulletManager::CreateEnemyBullet(const Vector3& position, Vector3& direction)
{
	//	�J�������擾
	auto camera = m_pPlayer->GetCamera();
	// �v���C���[����J�����̏����擾
	Vector3 playerPos = camera->GetEyePosition();

	// �G�e�̐���
	auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize); // �T�C�Y1.0f�̗�
	bullet->Initialize(m_commonResources, m_enemyBulletType);
	bullet->MakeBall(position, direction, playerPos);
	bullet->SetShooter(m_pShooter);
	m_enemyBullets.push_back(std::move(bullet));
}

// �e�̕�[
void BulletManager::ReLoadPlayerBullet()
{
	m_reloadTimer += m_elapsedTime;

	if (m_playerBulletCount < MAX_PLAYER_BULLET_COUNT && m_reloadTimer >= 0.1f)
	{
		m_playerBulletCount++;
	}
	if (m_reloadTimer > BulletParameters::RELOAD_INTERVAL)
	{
		m_reloadTimer = 0.0f;
	}
}

// �e������
void BulletManager::ConsumePlayerBullet()
{
	// �v���C���[�̒e������
	if (m_playerBulletCount > 0)
	{
		m_playerBulletCount--;
	}
}

// �ǉ��_���[�W��ݒ�
void BulletManager::SetAdditionalDamage(int additionalDamage)
{
	for (auto& bullet : m_playerBullets)
	{
		bullet->SetAdditionalDamage(additionalDamage);
	}
}

void BulletManager::UpdatePlayerBullets(float elapsedTime)
{
	//	�J�������擾
	auto camera = m_pPlayer->GetCamera();
	for (auto it = m_playerBullets.begin(); it != m_playerBullets.end();)
	{
		auto& bullet = *it;// �e���擾
		// �J��������e�ɓK�p
		bullet->SetCameraEye(camera->GetEyePosition());
		bullet->SetCameraTarget(camera->GetTargetPosition());
		bullet->SetCameraUp(camera->GetUpVector());

		// �e���X�V
		bullet->Update(elapsedTime);
		// �e���������}���邩�n�ʂɒ�������폜
		if (CheckCollisionWithEnemies(bullet) || bullet->IsExpired() || bullet->GetBulletPosition().y <= DELETE_BULLET_POSITION)
		{
			it = m_playerBullets.erase(it);
		}
		else
		{
			++it;
		}
	}

}

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
		bullet->SetCameraEye(camera->GetEyePosition());
		bullet->SetCameraTarget(camera->GetTargetPosition());
		bullet->SetCameraUp(camera->GetUpVector());
		bullet->SetEnemyPosition(shooter->GetPosition());		// �e���X�V
		bullet->Update(elapsedTime);
		// �v���C���[�Ƃ̓����蔻��
		if (CheckCollisionWithPlayer(bullet, enemy) || bullet->IsExpired() || bullet->GetBulletPosition().y <= DELETE_BULLET_POSITION)
		{
			// �Փˏ����i�����ł̓G�t�F�N�g��SE�Đ��A�v���C���[�ւ̃_���[�W�����Ȃǁj
			it = m_enemyBullets.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BulletManager::SetSound()
{
	// �I�[�f�B�I�}�l�[�W���[�̏�����
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");

}

BulletManager::GridKey BulletManager::GetGridKey(const DirectX::SimpleMath::Vector3& position)
{
	return
	{
		static_cast<int>(position.x / BulletParameters::GRID_CELL_SIZE),
		static_cast<int>(position.y / BulletParameters::GRID_CELL_SIZE)
	};
}

bool BulletManager::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
{
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			if (enemy->GetCanAttack() == true)
				enemy->SetEnemyHP(bullet->Damage());// �G��HP�����炷
			// �G�t�F�N�g��ǉ�
			float effectSize = dynamic_cast<Boss*>(enemy.get()) ? 10.0f : 3.0f;
			m_pEnemyManager->GetEffect().push_back(std::make_unique<Effect>(
				m_commonResources,
				Effect::ParticleType::ENEMY_HIT,
				enemy->GetPosition(),
				effectSize,
				enemy->GetMatrix()));
			// �v���C���[�̒e���G�ɓ��������t���O�𗧂Ă�
			enemy->SetEnemyHitByPlayerBullet(true);// �q�b�g�t���O
			m_audioManager->PlaySound("Hit", m_pPlayer->GetVolume() * HIT_VOLUME);// SE���Đ�
			return true;// ����������true��Ԃ�
		}
	}
	return false;// ������Ȃ�������false��Ԃ�
}


bool BulletManager::CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy)
{
	// �v���C���[�Ƃ̓����蔻��
	enemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
	if (enemy->GetBulletBoundingSphere().Intersects(enemy->GetPlayerBoundingSphere()))
	{
		enemy->SetPlayerHitByEnemyBullet(true);
		enemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetBulletDirection());
		enemy->GetPlayer()->SetisPlayEffect(true);
		enemy->GetPlayer()->SetisPlayerDamage(true);
		return true; // �Փ˂�����true��Ԃ�
	}
	return false; // �Փ˂��Ȃ����false��Ԃ�}

}

void BulletManager::CheckCollisionWithBullets()
{
	// �O���b�h���N���A
	m_playerBulletGrid.clear();
	m_enemyBulletGrid.clear();
	// �v���C���[�̒e���O���b�h�ɓo�^
	for (auto& bullet : m_playerBullets)
	{
		GridKey key = GetGridKey(bullet->GetBulletPosition());
		m_playerBulletGrid[key].push_back(bullet.get());
	}
	// �G�̒e���O���b�h�ɓo�^
	for (auto& bullet : m_enemyBullets)
	{
		GridKey key = GetGridKey(bullet->GetBulletPosition());
		m_enemyBulletGrid[key].push_back(bullet.get());
	}
	// �Փ˔���
	for (auto& pair : m_enemyBulletGrid)
	{
		const GridKey& enemyKey = pair.first;
		std::vector<EnemyBullet*>& enemyBullets = pair.second;
		// 9�Z��(���Z���Ǝ���8�Z��)�𒲂ׁA���O�Ƀ��X�g��
		std::vector<GridKey> neighborKeys;
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				neighborKeys.push_back({ enemyKey.x + x, enemyKey.y + y });
			}
		}
		// ���O�ɍ�������X�g���g���ďՓ˔���
		for (const auto& neighborKey : neighborKeys)
		{
			auto it = m_playerBulletGrid.find(neighborKey);
			if (it != m_playerBulletGrid.end())
			{
				std::vector<PlayerBullet*>& playerBullets = it->second;
				for (auto& playerBullet : playerBullets)
				{
					for (auto& enemyBullet : enemyBullets)
					{
						if (playerBullet->GetBoundingSphere().Intersects(enemyBullet->GetBoundingSphere()))
						{
							// �G�̒e���폜
							m_enemyBullets.erase(
								std::remove_if(m_enemyBullets.begin(), m_enemyBullets.end(),
									[&enemyBullet](const std::unique_ptr<EnemyBullet>& bullet)
									{
										return bullet.get() == enemyBullet; // �|�C���^����v������̂��폜
									}),
								m_enemyBullets.end()
							);
							// �v���C���[�̒e���폜
							m_playerBullets.erase(
								std::remove_if(m_playerBullets.begin(), m_playerBullets.end(),
									[&playerBullet](const std::unique_ptr<PlayerBullet>& bullet)
									{
										return bullet.get() == playerBullet; // �|�C���^����v������̂��폜
									}),
								m_playerBullets.end()
							);

							return;
						}
					}
				}
			}
		}
	}
}

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
