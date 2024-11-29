/*
	@file	PlayerBullets.cpp
	@brief	�v���C���[�̒e�S�̂��Ǘ�����N���X
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
		if ((*it)->IsExpired() || (*it)->GetBulletPosition().y <= -0.25f)
		{
			it = m_playerBullet.erase(it);// �e���������}���邩�n�ʂɒ�������폜
		}
		else
		{
			bool isHit = false;// �q�b�g�t���O��������
			for (auto& enemy : m_pEnemies->GetEnemies())
			{

				// �G�ƃv���C���[�̒e�̓����蔻��
				if ((*it)->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
				{

					isHit = true;// �q�b�g�t���O�𗧂Ă�
					enemy->SetEnemyHP((*it)->Damage());// �G��HP�����炷
					if (auto boss = dynamic_cast<Boss*>(enemy.get()))
					{
						m_pEnemies->GetEffect().push_back(std::make_unique<Effect>(m_commonResources,
							Effect::ParticleType::ENEMY_HIT,
							enemy->GetPosition(),
							10.0f,
							enemy->GetMatrix()));

					}
					else
					{
						// �G�t�F�N�g��ǉ�
						m_pEnemies->GetEffect().push_back(std::make_unique<Effect>(m_commonResources,
							Effect::ParticleType::ENEMY_HIT,
							enemy->GetPosition(),
							3.0f,
							enemy->GetMatrix()));
					}

					// �v���C���[�̒e���G�ɓ��������t���O�𗧂Ă�
					enemy->SetHitToPlayerBullet(true);
					m_audioManager->PlaySound("Hit", m_pPlayer->GetVolume() * 0.8f);// �q�b�gSE���Đ�
					break;
				}
			}
			// �q�b�g���Ă��Ȃ��Ȃ玟�̒e��
			if (isHit) it = m_playerBullet.erase(it);
			else it++;
		}
	}
}

void PlayerBullets::Render()
{
	using namespace DirectX::SimpleMath;
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();// �v���C���[�̃J��������r���[�s����擾
	Matrix proj = m_pPlayer->GetCamera()->GetProjectionMatrix();// �v���C���[�̃J��������ˉe�s����擾
	for (auto& bullet : m_playerBullet)bullet->Render(view, proj);// �v���C���[�̒e��`��
}


void PlayerBullets::CreateBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{	// SE�̍Đ�
	m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());
	// �e�𐶐�
	auto bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(m_commonResources);// ������
	bullet->MakeBall(position, direction);// �e�𐶐�
	GetPlayerBullet().push_back(std::move(bullet));// �v���C���[�̒e��ǉ�
	SetIsBullet(true);// �e�����t���O�𗧂Ă�
}