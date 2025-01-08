/*
	@file	EnemyBullets.cpp
	@brief	�G�̒e�S�̂��Ǘ�����N���X
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
void EnemyBullets::Update(float elapsedTime)
{
	std::vector<std::unique_ptr<EnemyBullet>> newBullets;
	Vector3 enemyPosition = m_pEnemy->GetPosition();
	Vector3 playerPos = m_pEnemy->GetCamera()->GetEyePosition();
	Vector3 playerTarget = m_pEnemy->GetCamera()->GetTargetPosition();
	Vector3 playerUp = m_pEnemy->GetCamera()->GetUpVector();

	// �e�̍X�V�ƗL���Ȓe��V�������X�g�Ɉړ�����
	for (auto& bullet : m_bullets)
	{
		bullet->SetCameraEye(playerPos);// �J�����̈ʒu��ݒ�
		bullet->SetCameraTarget(playerTarget);// �J�����̒����_��ݒ�
		bullet->SetCameraUp(playerUp);	// �J�����̏������ݒ�
		bullet->SetEnemyPosition(enemyPosition);// �G�̈ʒu��ݒ�
		bullet->Update(elapsedTime); // �e�̍X�V

		// �������}���Ă��Ȃ��e������V�������X�g�ɒǉ�����
		if (!bullet->IsExpired() && !m_pEnemy->GetPlayerHitByEnemyBullet())
		{
			m_pEnemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
			m_pEnemy->SetPlayerHitByEnemyBullet(m_pEnemy->GetBulletBoundingSphere().Intersects(m_pEnemy->GetPlayerBoundingSphere()));
			if (m_pEnemy->GetPlayerHitByEnemyBullet())
			{
				m_pEnemy->SetPlayerHitByEnemyBullet(m_pEnemy->GetPlayerHitByEnemyBullet());// �v���C���[�ɓ��������t���O��ݒ�
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
	for (const auto& bullet : m_bullets)
	{
		bullet->RenderShadow(view, proj);// �e�̕`��
		bullet->Render(view, proj);// �e�̕`��
		bullet->RenderBoundingSphere(view, proj);// �e�̋��E���`��
	}

}

// �e�̐���
void EnemyBullets::CreateBullet(float size, EnemyBullet::BulletType type)
{
	// �v���C���[����J�����̏����擾
	Vector3 playerPos = m_pEnemy->GetCamera()->GetEyePosition();

	// �e�𔭎˂���
	auto bullet = std::make_unique<EnemyBullet>(size);
	bullet->Initialize(m_commonResources, type);
	bullet->MakeBall(m_enemyPosition, m_direction, playerPos);
	m_bullets.push_back(std::move(bullet));
}

// �����e�̉�]������ݒ�
void EnemyBullets::SetRotateDirection(int direction)
{
	for (auto& bullet : m_bullets)bullet->SetRotateDirection(direction);
}