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
	, m_audioManager{ AudioManager::GetInstance() }
	, m_enemyBulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_enemyBulletSize{ 1.0f }
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
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");
}

// �X�V
void BulletManager::Update(float elapsedTime)
{


	// �v���C���[�̒e���X�V
	UpdatePlayerBullets(elapsedTime);
	// �G�̒e���X�V
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{

		// �G�̒e���X�V
		UpdateEnemyBullets(elapsedTime, enemy);

	}
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
	}

	// �G�e�̕`��
	for (const auto& bullet : m_enemyBullets)
	{
		bullet->Render(view, proj);
	}
}
// �e�𐶐�
void BulletManager::CreateBullet(const Vector3& position, Vector3& direction, BulletType type)
{
	switch (type)
	{
	case BulletType::PLAYER:
	{
		// �v���C���[�e�̐���
		auto bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(m_commonResources);
		bullet->MakeBall(position, direction);
		m_playerBullets.push_back(std::move(bullet));
		// SE�̍Đ�
		m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());	}
	break;
	case BulletType::ENEMY:
	{
		//	�J�������擾
		auto camera = m_pPlayer->GetCamera();
		// �v���C���[����J�����̏����擾
		Vector3 playerPos = camera->GetEyePosition();
		// �G�e�̐���
		auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize); // �T�C�Y1.0f�̗�
		bullet->Initialize(m_commonResources, m_enemyBulletType);
		bullet->MakeBall(position, direction, playerPos);
		m_enemyBullets.push_back(std::move(bullet));
	}
	break;
	default:
		assert(false);
		break;
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
	auto camera = m_pPlayer->GetCamera();
	for (auto it = m_enemyBullets.begin(); it != m_enemyBullets.end();)
	{
		auto& bullet = *it;// �e���擾
		// �J��������e�ɓK�p
		bullet->SetCameraEye(camera->GetEyePosition());
		bullet->SetCameraTarget(camera->GetTargetPosition());
		bullet->SetCameraUp(camera->GetUpVector());
		// �e���X�V
		bullet->Update(elapsedTime);
		// �v���C���[�Ƃ̓����蔻��
		if (CheckCollisionWithPlayer(bullet, enemy))
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