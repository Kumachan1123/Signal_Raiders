#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
EnemyBullets::EnemyBullets(Enemy* m_pEnemy)
	:m_commonResources{}
	, m_pEnemy{ m_pEnemy }
{
}

// �f�X�g���N�^
EnemyBullets::~EnemyBullets()
{
}

// ������
void EnemyBullets::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
}

// �X�V
void EnemyBullets::Update(float elapsedTime, DirectX::SimpleMath::Vector3 enemyPos)
{
	std::vector<std::unique_ptr<EnemyBullet>> newBullets;
	Vector3 position = m_pEnemy->GetPosition();
	Vector3 playerPos = m_pEnemy->GetPlayer()->GetCamera()->GetEyePosition();
	Vector3 playerTarget = m_pEnemy->GetPlayer()->GetCamera()->GetTargetPosition();
	Vector3 playerUp = m_pEnemy->GetPlayer()->GetCamera()->GetUpVector();

	// �e�̍X�V�ƗL���Ȓe��V�������X�g�Ɉړ�����
	for (auto& bullet : m_bullets)
	{
		bullet->SetCameraEye(playerPos);// �J�����̈ʒu��ݒ�
		bullet->SetCameraTarget(playerTarget);// �J�����̒����_��ݒ�
		bullet->SetCameraUp(playerUp);	// �J�����̏������ݒ�
		bullet->Update(enemyPos, elapsedTime); // �e�̍X�V

		// �������}���Ă��Ȃ��e������V�������X�g�ɒǉ�����
		if (!bullet->IsExpired() && !m_pEnemy->GetBulletHitToPlayer())
		{
			m_pEnemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
			m_pEnemy->SetBulletHitToPlayer(m_pEnemy->GetBulletBoundingSphere().Intersects(m_pEnemy->GetPlayerBoundingSphere()));
			if (m_pEnemy->GetBulletHitToPlayer())
			{
				m_pEnemy->SetBulletHitToPlayer(m_pEnemy->GetBulletHitToPlayer());

				continue;
			}
			newBullets.push_back(std::move(bullet));
		}
	}

	// m_bullets ��V�������X�g�Œu��������
	m_bullets = std::move(newBullets);
}

// �`��
void EnemyBullets::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)

{
	for (const auto& bullet : m_bullets)bullet->Render(view, proj);
}

// �e�̐���
void EnemyBullets::CreateBullet(Vector3 enemyPos, Vector3 dir, Vector3 playerPos)
{
	m_position = enemyPos;
	// �e�𔭎�
	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(m_commonResources);
	bullet->MakeBall(m_position, dir, playerPos);
	m_bullets.push_back(std::move(bullet));
}
