/*
	@file	Enemies.cpp
	@brief	�G�����N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "Enemies.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
Enemies::Enemies(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_enemy{},
	m_isEnemyBorn{ false },
	m_isBorned{ false },
	m_enemyIndex{ 0 },
	m_enemyBornInterval{ 0.0f },
	m_enemyBornTimer{ 0.0f }
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Enemies::~Enemies()
{
	// do nothing.
}

//---------------------------------------------------------
// �X�V
//---------------------------------------------------------
void Enemies::Update(Effect* pEffect, Wifi* pWifi, PlayerController* pPlayerController, float elapsedTime)
{
	//	// �G�����E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	//	// �G�����^�C�}�[���X�V
	//	m_enemyBornTimer += elapsedTime;
	//	if (m_startTime >= 5.0f)m_isEnemyBorn = true;//�����\�ɂ���
	//	// �����\�Ȃ�
	//	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < m_wifi->GetWifiLevels().size())//m_wifi->GetWifiLevels().size()
	//	{
	//		if (m_enemyBornTimer >= m_enemyBornInterval)// ��莞�Ԍo�߂�����
	//		{
	//			// �G�𐶐�����
	//			auto enemy = std::make_unique<Enemy>();
	//			enemy->Initialize(m_commonResources, m_wifi->GetWifiLevels()[m_enemyIndex]);
	//			m_enemy.push_back(std::move(enemy));
	//			// �^�C�}�[�����Z�b�g���A����Wi-Fi���x���̃C���f�b�N�X�ɐi��
	//			m_enemyBornTimer = 0.0f;
	//			m_enemyIndex++;
	//		}
	//	}
	//	// �������������琶���s�\�ɂ���
	//	if (m_enemyIndex >= m_wifi->GetWifiLevels().size())
	//	{
	//		m_enemyBornTimer = 0.0f;
	//		m_isEnemyBorn = false;
	//		m_isBorned = true;
	//	}
	//	// �G���m�̓����蔻��E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	//	// �G���m���d�Ȃ�Ȃ��悤�ɂ���
	//	for (size_t i = 0; i < m_enemy.size(); ++i)
	//	{
	//		for (size_t j = i + 1; j < m_enemy.size(); ++j)
	//		{
	//			bool hit = m_enemy[i]->GetBoundingSphere().Intersects(m_enemy[j]->GetBoundingSphere());
	//			m_enemy[i]->SetHitToOtherEnemy(hit);
	//			m_enemy[j]->SetHitToOtherEnemy(hit);
	//			if (hit)m_enemy[i]->CheckHitOtherEnemy(m_enemy[i]->GetBoundingSphere(), m_enemy[j]->GetBoundingSphere());
	//		}
	//	}
	//
	//	// �G�ƃv���C���[�̈��͈͂Ƃ̓����蔻��E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	//	for (auto& enemy : m_enemy)
	//	{
	//		m_isHitPlayerToEnemy = false;
	//		// �G���X�V
	//		enemy->Update(elapsedTime, m_playerController->GetPlayerPosition());
	//		// �G�̒e���v���C���[�ɓ���������
	//		bool hit = enemy->GetBulletHitToPlayer();
	//		if (hit)
	//		{
	//			enemy->SetPlayerHP(m_playerHP);
	//			enemy->SetBulletHitToPlayer(false);
	//			m_audioManager->PlaySound("Damage", 0.5);// �_���[�WSE���Đ�
	//		}
	//		// �G���v���C���[�ɓ���������
	//		if (enemy->GetBoundingSphere().Intersects(m_inPlayerArea))
	//		{
	//			m_isHitPlayerToEnemy = true;
	//			// �����Ԃ��p�Ƀv���C���[�̈��͈͋��E�������[�J���ϐ��ɃR�s�[
	//			BoundingSphere playerSphere = m_inPlayerArea;
	//			// ���E���͈̔͂𒲐�
	//			playerSphere.Radius /= 3.0f;
	//			// �v���C���[�̋��E���ƓG�̋��E�����d�Ȃ��Ă�����
	//			if (enemy->GetBoundingSphere().Intersects(playerSphere)) enemy->CheckHitOtherEnemy(enemy->GetBoundingSphere(), playerSphere);
	//		}
	//		// �v���C���[�ƓG�̓����蔻���ݒ�
	//		enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	//		enemy->SetPlayerBoundingSphere(m_playerSphere);
	//	}
	//	// �G�̍폜�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
	//	// �폜�Ώۂ�ێ�����x�N�^�[
	//	std::vector<std::unique_ptr<Enemy>> enemiesToRemove;
	//	// �폜�Ώۂ����W����
	//	for (auto it = m_enemy.begin(); it != m_enemy.end(); )
	//	{
	//		// �G������ł�����
	//		if ((*it)->GetEnemyIsDead())
	//		{
	//			// �G�̍��W��n���Ĕ��j�G�t�F�N�g���Đ�
	//			m_effect.push_back(std::make_unique<Effect>(m_commonResources,
	//														Effect::ParticleType::ENEMY_DEAD,
	//														(*it)->GetPosition(),
	//														(*it)->GetMatrix()));
	//			m_audioManager->PlaySound("EnemyDead", 2);// �G��SE���Đ�
	//			enemiesToRemove.push_back(std::move(*it));// �폜�Ώۂɒǉ�
	//			it = m_enemy.erase(it);  // �폜���ăC�e���[�^���X�V
	//		}
	//		else it++;  // ���̗v�f��
	//	}
}

//---------------------------------------------------------
// �`��
//---------------------------------------------------------
void Enemies::Draw()
{
	// do nothing.
}
