/*
	@file	EnemyBullets.cpp
	@brief	�G�̒e�S�̂��Ǘ�����N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
EnemyBullets::EnemyBullets(IEnemy* pEnemy)
	:m_commonResources{}
	, m_pEnemy{ pEnemy }
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
	Vector3 playerPos = m_pEnemy->GetCamera()->GetEyePosition();
	Vector3 playerTarget = m_pEnemy->GetCamera()->GetTargetPosition();
	Vector3 playerUp = m_pEnemy->GetCamera()->GetUpVector();

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
				m_pEnemy->SetBulletHitToPlayer(m_pEnemy->GetBulletHitToPlayer());// �v���C���[�ɓ��������t���O��ݒ�
				m_pEnemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetBulletDirection());// �v���C���[�Ƀ_���[�W��^����G�̌�����ݒ�
				m_pEnemy->GetPlayer()->SetisPlayEffect(true);// �G�t�F�N�g�Đ��t���O��ݒ�
				m_pEnemy->GetPlayer()->SetisPlayerDamage(true);// �v���C���[�Ƀ_���[�W��^����
				continue;
			}
			// �e��Y���W��-1�ȉ��ɂȂ�����e������
			if (bullet->GetBulletPosition().y < 0.0f && bullet->GetBulletType() != EnemyBullet::BulletType::VERTICAL)continue;
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
void EnemyBullets::CreateBullet(Vector3 enemyPos, Vector3 dir, Vector3 playerPos, float size, EnemyBullet::BulletType type)
{
	m_position = enemyPos;
	// �e�𔭎˂���
	auto bullet = std::make_unique<EnemyBullet>(size);
	bullet->Initialize(m_commonResources, type);
	bullet->MakeBall(m_position, dir, playerPos);
	m_bullets.push_back(std::move(bullet));
}

// �����e�̉�]������ݒ�
void EnemyBullets::SetRotateDirection(int direction)
{
	for (auto& bullet : m_bullets)bullet->SetRotateDirection(direction);
}