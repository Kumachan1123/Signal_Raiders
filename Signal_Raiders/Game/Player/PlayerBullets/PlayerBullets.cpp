/*
	@file	PlayerBullets.cpp
	@brief	�v���C���[�̒e�S�̂��Ǘ�����N���X
*/
#include "pch.h"
#include "PlayerBullets.h"
using namespace DirectX::SimpleMath;

PlayerBullets::PlayerBullets(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemyManager{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
{
}

PlayerBullets::~PlayerBullets()
{
	// �v���C���[�̒e���폜
	for (auto& bullet : m_bullets)bullet.reset();
	m_bullets.clear();
}


void PlayerBullets::Initialize(Player* pPlayer, EnemyManager* pEnemies)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���擾
	m_pEnemyManager = pEnemies;// �G�S�̂̃|�C���^�[���擾
	// ���ʉ��̏�����
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");
}
//---------------------------------------------------------
// �v���C���[�̒e���X�V����
//---------------------------------------------------------
void PlayerBullets::Update(float elapsedTime)
{
	//	�J�������擾
	auto camera = m_pPlayer->GetCamera();
	for (auto it = m_bullets.begin(); it != m_bullets.end();)
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
			it = m_bullets.erase(it);
		}
		else
		{
			++it;
		}
	}


}
//---------------------------------------------------------
// �G�ƃv���C���[�̒e�̓����蔻��
//---------------------------------------------------------
bool PlayerBullets::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
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
//---------------------------------------------------------
// �v���C���[�̒e��`�悷��
//---------------------------------------------------------
void PlayerBullets::Render()
{
	auto camera = m_pPlayer->GetCamera();
	Matrix view = camera->GetViewMatrix();// �v���C���[�̃J��������r���[�s����擾
	Matrix proj = camera->GetProjectionMatrix();// �v���C���[�̃J��������ˉe�s����擾
	for (auto& bullet : m_bullets)
	{
		bullet->Render(view, proj);// �v���C���[�̒e��`��
		bullet->RenderShadow(view, proj);// �v���C���[�̒e�̉e��`��
		bullet->DrawCollision(view, proj);// �v���C���[�̒e�̓����蔻���`��
	}
}

//---------------------------------------------------------
// �e�𐶐�����
//---------------------------------------------------------
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
