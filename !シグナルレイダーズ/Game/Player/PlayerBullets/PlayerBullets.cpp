/*
	@file	PlayerBullets.cpp
	@brief	�v���C���[�̒e�S�̂��Ǘ�����N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "PlayerBullets.h"

PlayerBullets::PlayerBullets(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemies{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
{
}

PlayerBullets::~PlayerBullets()
{
	// �v���C���[�̒e���폜
	for (auto& bullet : m_playerBullet)bullet.reset();
	m_playerBullet.clear();
}


void PlayerBullets::Initialize(Player* pPlayer, Enemies* pEnemies)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���擾
	m_pEnemies = pEnemies;// �G�S�̂̃|�C���^�[���擾

	// �I�[�f�B�I�}�l�[�W���[������������
	m_audioManager->Initialize();
	// ���ʉ��̏�����
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");
}

void PlayerBullets::Update(float elapsedTime)
{
	// �v���C���[�̒e���X�V����
	DirectX::SimpleMath::Vector3 dir = m_pPlayer->GetCamera()->GetDirection();
	DirectX::SimpleMath::Vector3 playerPos = m_pPlayer->GetCamera()->GetEyePosition();
	DirectX::SimpleMath::Vector3 playerTarget = m_pPlayer->GetCamera()->GetTargetPosition();
	DirectX::SimpleMath::Vector3 playerUp = m_pPlayer->GetCamera()->GetUpVector();
	for (auto it = m_playerBullet.begin(); it != m_playerBullet.end(); )
	{
		(*it)->SetCameraEye(playerPos);// �J�����̈ʒu��ݒ�
		(*it)->SetCameraTarget(playerTarget);// �J�����̒����_��ݒ�
		(*it)->SetCameraUp(playerUp);	// �J�����̏������ݒ�
		(*it)->Update(dir, elapsedTime);// �e���X�V
		if ((*it)->IsExpired())it = m_playerBullet.erase(it);// �e���������}������폜
		else
		{
			bool isHit = false;
			for (auto& enemy : m_pEnemies->GetEnemies())
			{
				if ((*it)->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
				{
					isHit = true;
					enemy->SetEnemyHP(enemy->GetHP() - (*it)->Damage());
					m_pEnemies->GetEffect().push_back(std::make_unique<Effect>(m_commonResources,
																			   Effect::ParticleType::ENEMY_HIT,
																			   enemy->GetPosition(),
																			   enemy->GetMatrix()));
					enemy->SetHitToPlayerBullet(true);
					m_audioManager->PlaySound("Hit", m_pPlayer->GetVolume());// �q�b�gSE���Đ�
					break;
				}
			}
			if (isHit) it = m_playerBullet.erase(it);
			else it++;
		}
	}
}

void PlayerBullets::Render()
{
	using namespace DirectX::SimpleMath;
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix proj = m_pPlayer->GetCamera()->GetProjectionMatrix();
	for (auto& bullet : m_playerBullet)bullet->Render(view, proj);
}


void PlayerBullets::CreateBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{	// SE�̍Đ�
	m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());
	auto bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(m_commonResources);
	bullet->MakeBall(position, direction);
	GetPlayerBullet().push_back(std::move(bullet));
	SetIsBullet(true);
}