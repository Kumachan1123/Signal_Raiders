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
	:m_commonResources{ nullptr }
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
	// ���ׂĂ̒e���X�V
	for (auto& bullet : m_bullets)
	{
		bullet->SetCameraEye(playerPos);
		bullet->SetCameraTarget(playerTarget);
		bullet->SetCameraUp(playerUp);
		bullet->SetEnemyPosition(enemyPosition);
		bullet->Update(elapsedTime);

		// �v���C���[�Ƃ̓����蔻��
		m_pEnemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
		if (m_pEnemy->GetBulletBoundingSphere().Intersects(m_pEnemy->GetPlayerBoundingSphere()))
		{
			m_pEnemy->SetPlayerHitByEnemyBullet(true);
			m_pEnemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetBulletDirection());
			m_pEnemy->GetPlayer()->SetisPlayEffect(true);
			m_pEnemy->GetPlayer()->SetisPlayerDamage(true);
			continue; // ���������e�͏������Ȃ�
		}
	}

	// ���������e���폜
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
		[this](const std::unique_ptr<EnemyBullet>& bullet)
		{	// �������X�g
			// �@ �e���������}����
			// �A Y���W��0�ȉ��i�����������e�͗�O�j
			// �B �v���C���[�ɓ�������
			return bullet->IsExpired() ||
				(bullet->GetBulletPosition().y < 0.0f && bullet->GetBulletType() != EnemyBullet::BulletType::VERTICAL) ||
				m_pEnemy->GetPlayerHitByEnemyBullet();
		}),
		m_bullets.end());
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
	bullet->MakeBall(m_bulletSpawnPos, m_direction, playerPos);
	m_bullets.push_back(std::move(bullet));
}

// �����e�̉�]������ݒ�
void EnemyBullets::SetRotateDirection(int direction)
{
	for (auto& bullet : m_bullets)bullet->SetRotateDirection(direction);
}