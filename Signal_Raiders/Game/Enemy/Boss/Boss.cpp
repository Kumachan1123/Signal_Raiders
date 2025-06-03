/*
*	@file	Boss.cpp
*	@brief	�{�X�N���X
*/
#include <pch.h>
#include "Boss.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/BossAI/BossAI.h"
/*
*	@brief	�R���X�g���N�^
*	@details �{�X�N���X�̃R���X�g���N�^
*	@param pBoss		�{�X�x�[�X�̃|�C���^
*	@param resources	���ʃ��\�[�X
*	@return	�Ȃ�
*/
Boss::Boss(BossBase* pBoss, CommonResources* commonResources)
	: m_pCommonResources(commonResources)// ���ʃ��\�[�X
	, m_pBossBase(pBoss)// �{�X�̃|�C���^
	, m_bossBS{}// ���E���̏�����
	, m_pBossModel{}// �{�X���f��
	, m_pHPBar{}// HP�o�[
	, m_time(0.0f)// ����
	, m_position{}// �ʒu
	, m_velocity{}// ���x
	, m_rotate{}// ��]
	, m_matrix{}// �}�g���N�X
	, m_bulletType(BulletType::NORMAL)// �e�̎��
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �{�X�N���X�̃f�X�g���N�^
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
Boss::~Boss()
{
	// �{�X���f���̉��
	m_pBossModel.reset();
	// HP�o�[�̉��
	m_pHPBar.reset();
	// ���ʃ��\�[�X�̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief	����������
*	@details �{�X�N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void Boss::Initialize()
{
	// �{�X���f������
	m_pBossModel = std::make_unique<BossModel>();
	// �{�X���f��������
	m_pBossModel->Initialize(m_pCommonResources);
	// ���X�{�X�̏����ʒu��ݒ�
	m_position = EnemyParameters::INITIAL_BOSS_POSITION;
	// �x�[�X�N���X�ɏ����ʒu��ݒ�
	m_pBossBase->SetPosition(m_position);
	// �ʏ펞�{�X�̓����蔻���ݒ�
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_BOSS_RADIUS);
	// �V�[���h�W�J���̃{�X�̓����蔻���ݒ�
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::BOSS_SHIELD_RADIUS);
	// �e�̃T�C�Y��ݒ�
	m_pBossBase->SetBulletSize(EnemyParameters::BOSS_BULLET_SIZE);
	// �{�X���v���C���[�ɗ^����_���[�W��ݒ�
	m_pBossBase->SetToPlayerDamage(EnemyParameters::BOSS_DAMAGE);
	// �o���A�j��p�[�e�B�N���̃T�C�Y�ݒ�
	m_pBossBase->SetBarrierBreakSize(EnemyParameters::BOSS_BARRIERBREAK_SIZE);
	// ���S�G�t�F�N�g�̃T�C�Y
	m_pBossBase->SetDeadEffectSize(EnemyParameters::BOSS_DEADEFFECT_SCALE);
}
/*
*	@brief	�X�V����
*	@details �{�X�N���X�̍X�V����
*	@param elapsedTime	�o�ߎ���
*	@return	�Ȃ�
*/
void Boss::ChangeState()
{
	// ���f���̏�Ԃ��X�V
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());
}
/*
*	@brief	�`�揈��
*	@details �{�X�N���X�̕`�揈��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void Boss::Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���[���h�s���ݒ�i�X�P�[�� �� ��] �� �ʒu�j
	Matrix enemyWorld = Matrix::CreateScale(m_pBossBase->GetScale())
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	// �V�[���h�̃��[���h�s���ݒ�i�X�P�[�� �� ��] �� �ʒu�j
	Matrix sheildWorld = Matrix::CreateScale(m_pBossBase->GetScale() * EnemyParameters::BOSS_SHIELD_SIZE)
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	// �V�[���h�̍��W��ݒ�
	m_pBossBase->GetBossSheild()->SetPosition(m_bossBS.Center);
	// �V�[���h�̉�]��ݒ�
	m_pBossBase->GetBossSheild()->SetRotation(m_pBossBase->GetQuaternion());
	// ���f���`��
	m_pBossModel->Render(context, states, enemyWorld, view, proj);
	// �V�[���h�`��
	m_pBossBase->GetBossSheild()->Render(context, states, sheildWorld, view, proj);
	// HP�o�[�̈ʒu��ݒ�
	Vector3 hpBarPos = m_pBossBase->GetPosition() - EnemyParameters::BOSS_HPBAR_OFFSET;
	// HP�o�[�̃X�P�[����ݒ�
	m_pBossBase->GetHPBar()->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));
	// HP�o�[�`��
	m_pBossBase->GetHPBar()->Render(view, proj, hpBarPos, m_rotate);
}
/*
*	@brief	�e�̈ʒu�ݒ�
*	@details �{�X�N���X�̒e�̔��ˈʒu��ݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void Boss::BulletPositioning()
{
	using namespace DirectX::SimpleMath;
	// �e�̔��ˈʒu��ݒ�
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation()) * Matrix::CreateTranslation(m_pBossBase->GetPosition());
	// �����̍��W�ɉ�]��K�p
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);
	// ���̍��W�ɉ�]��K�p
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);
	// �E�̍��W�ɉ�]��K�p
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);
}
/*
*	@brief	�e�̐���
*	@details �{�X�̒e�𐶐�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void Boss::CreateBullet()
{
	// �e�̃T�C�Y��ݒ�
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE);
	// �e�𔭎˂����I�u�W�F�N�g��ݒ�
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);
	// Enemies�N���X�Őݒ肵���e�̃^�C�v�ɂ���ď����𕪊�
	switch (m_pBossBase->GetBulletType())
	{
	case BossBase::BossBulletType::STAGE_1:// �ʏ�e
		// �����̒e�𔭎�
		CreateCenterBullet(BulletType::NORMAL);
		break;
	case BossBase::BossBulletType::STAGE_2:// ��

		// ���̒e�𔭎�
		CreateLeftBullet(BulletType::NORMAL);
		// �E�̒e�𔭎�
		CreateRightBullet(BulletType::NORMAL);
		break;
	case BossBase::BossBulletType::STAGE_3:// �O��
		// �����̒e�𔭎�
		CreateCenterBullet(BulletType::NORMAL);
		// ���̒e�𔭎�
		CreateLeftBullet(BulletType::NORMAL);
		// �E�̒e�𔭎�
		CreateRightBullet(BulletType::NORMAL);
		break;
	}
}
/*
*	@brief	��������e�𔭎�
*	@details �{�X�̒�������e�𔭎˂���
*	@param type �e�̎��
*	@return	�Ȃ�
*/
void Boss::CreateCenterBullet(BulletType type)
{
	// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);
	// �e�𐶐�
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);
}
/*
*	@brief	���̒e�𔭎�
*	@details �{�X�̍�����e�𔭎˂���
*	@param type �e�̎��
*	@return	�Ȃ�
*/
void Boss::CreateLeftBullet(BulletType type)
{
	// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);
	// �e�𐶐�
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeft, m_bulletDirection);// �e�𐶐�
}
/*
*	@brief �E�̒e�𔭎�
*	@details �{�X�̉E����e�𔭎˂���
*	@param type �e�̎��
*	@return �Ȃ�
*/
void Boss::CreateRightBullet(BulletType type)
{
	// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);
	// �e�𐶐�
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRight, m_bulletDirection);// �e�𐶐�
}