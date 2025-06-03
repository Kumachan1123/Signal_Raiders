/*
*	@file	LastBoss.cpp
*	@brief	���X�{�X�N���X
*/
#include <pch.h>
#include "LastBoss.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/BossAI/BossAI.h"
/*
*	@brief	�R���X�g���N�^
*	@details ���X�{�X�N���X�̃R���X�g���N�^
*	@param pBoss		�{�X�x�[�X�̃|�C���^
*	@param resources	���ʃ��\�[�X
*	@return	�Ȃ�
*/
LastBoss::LastBoss(BossBase* pBoss, CommonResources* commonResources)
	: m_pCommonResources(commonResources)// ���ʃ��\�[�X
	, m_pBossBase(pBoss)// �{�X�̃|�C���^
	, m_bossBS{}// ���E���̏�����
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
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
LastBoss::~LastBoss()
{
	// ���ʃ��\�[�X�̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
	// �{�X�̃|�C���^��nullptr�ɐݒ�
	m_pBossBase = nullptr;
	// ���X�{�X���f���̃|�C���^��nullptr�ɐݒ�
	m_pBossModel = nullptr;
}
/*
*	@brief	����������
*	@details ���X�{�X�̃��f���𐶐����A���������s��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void LastBoss::Initialize()
{
	// ���X�{�X���f������
	m_pBossModel = std::make_unique<LastBossModel>();
	// ���X�{�X���f�������� 
	m_pBossModel->Initialize(m_pCommonResources);
	// ���X�{�X�̏����ʒu��ݒ�
	m_position = EnemyParameters::INITIAL_LASTBOSS_POSITION;
	// �x�[�X�N���X�ɏ����ʒu��ݒ�
	m_pBossBase->SetPosition(m_position);
	// �ʏ펞�̃��X�{�X�̓����蔻���ݒ�
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_LASTBOSS_RADIUS);
	// �V�[���h�W�J���̃��X�{�X�̓����蔻���ݒ�
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::LASTBOSS_SHIELD_RADIUS);
	// �e�̃T�C�Y��ݒ�
	m_pBossBase->SetBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);
	// ������BossBulletType���g���ăX�e�[�W���Ƃ̃��X�{�X�̍U���͂𕪊�
	switch (m_pBossBase->GetBulletType())
	{
	case BossBase::BossBulletType::STAGE_4:// �X�e�[�W4�̃��X�{�X
		// �{�X���v���C���[�ɗ^����_���[�W��ݒ�
		m_pBossBase->SetToPlayerDamage(EnemyParameters::BOSS_DAMAGE);
		break;
	case BossBase::BossBulletType::STAGE_5:// �X�e�[�W5�̃��X�{�X
		// �{�X���v���C���[�ɗ^����_���[�W��ݒ�
		m_pBossBase->SetToPlayerDamage(EnemyParameters::LASTBOSS_DAMAGE);
		break;
	}
	// �o���A�j��p�[�e�B�N���̃T�C�Y�ݒ�
	m_pBossBase->SetBarrierBreakSize(EnemyParameters::LASTBOSS_BARRIERBREAK_SIZE);
	// ���S�G�t�F�N�g�̃T�C�Y
	m_pBossBase->SetDeadEffectSize(EnemyParameters::LASTBOSS_DEADEFFECT_SCALE);
}
/*
*	@brief	�X�V����
*	@details ���X�{�X�̏�Ԃ��X�V����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void LastBoss::ChangeState()
{
	// ���f���̏�Ԃ��X�V
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());
}
/*
*	@brief	�`�揈��
*	@details ���X�{�X�̃��f���ƃV�[���h�AHP�o�[��`�悷��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void LastBoss::Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
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
	Matrix sheildWorld = Matrix::CreateScale(m_pBossBase->GetScale() * EnemyParameters::LASTBOSS_SHIELD_SIZE)
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
	// HP�o�[��`��
	m_pBossBase->GetHPBar()->Render(view, proj, hpBarPos, m_rotate);
}
/*
*	@brief	�e�̈ʒu�ݒ�
*	@details ���X�{�X�̒e�̔��ˈʒu�����肷��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void LastBoss::BulletPositioning()
{
	using namespace DirectX::SimpleMath;
	// �e�̔��ˈʒu��ݒ�
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation()) * Matrix::CreateTranslation(m_pBossBase->GetPosition());
	// �����̍��W�ɉ�]��K�p
	m_bulletPosLeftDown = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTDOWN_GUN_OFFSET, transform);
	// ����̍��W�ɉ�]��K�p
	m_bulletPosLeftUp = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTUP_GUN_OFFSET, transform);
	// �E���̍��W�ɉ�]��K�p
	m_bulletPosRightDown = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTDOWN_GUN_OFFSET, transform);
	// �E��̍��W�ɉ�]��K�p
	m_bulletPosRightUp = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTUP_GUN_OFFSET, transform);

}
/*
*	@brief	�e�𐶐�����
*	@details ���X�{�X�̒e�𐶐�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void LastBoss::CreateBullet()
{
	// �e�̃T�C�Y��ݒ�
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);
	// �e�𔭎˂����I�u�W�F�N�g��ݒ�
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);
	// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(BulletType::NORMAL);
	// �e�𐶐��i����j
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftUp, m_bulletDirection);
	// �e�𐶐��i�����j
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftDown, m_bulletDirection);
	// �e�𐶐��i�E��j
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightUp, m_bulletDirection);
	// �e�𐶐��i�E���j
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightDown, m_bulletDirection);

}

