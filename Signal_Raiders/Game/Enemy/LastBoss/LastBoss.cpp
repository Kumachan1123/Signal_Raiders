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
	m_pCommonResources = nullptr;	// ���ʃ��\�[�X�̃|�C���^��nullptr�ɐݒ�
	m_pBossBase = nullptr;	// �{�X�̃|�C���^��nullptr�ɐݒ�
	m_pBossModel = nullptr;	// ���X�{�X���f���̃|�C���^��nullptr�ɐݒ�
}
/*
*	@brief	����������
*	@details ���X�{�X�̃��f���𐶐����A���������s��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void LastBoss::Initialize()
{
	m_pBossModel = std::make_unique<LastBossModel>();	// ���X�{�X���f������
	m_pBossModel->Initialize(m_pCommonResources);// ���X�{�X���f�������� 
	m_position = EnemyParameters::INITIAL_LASTBOSS_POSITION;// ���X�{�X�̏����ʒu��ݒ�
	m_pBossBase->SetPosition(m_position);// �x�[�X�N���X�ɏ����ʒu��ݒ�
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_LASTBOSS_RADIUS);// �ʏ펞�̃��X�{�X�̓����蔻���ݒ�
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::LASTBOSS_SHIELD_RADIUS);// �V�[���h�W�J���̃��X�{�X�̓����蔻���ݒ�
	m_pBossBase->SetBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);// �e�̃T�C�Y��ݒ�
	switch (m_pBossBase->GetBulletType())	// BulletType()�Ƃ��邪�A�����ł̓X�e�[�W���Ƃ̃��X�{�X�̍U���͂𕪊�
	{
	case BossBase::BossBulletType::STAGE_4:
		m_pBossBase->SetToPlayerDamage(EnemyParameters::BOSS_DAMAGE);// �{�X���v���C���[�ɗ^����_���[�W��ݒ�
		break;
	case BossBase::BossBulletType::STAGE_5:
		m_pBossBase->SetToPlayerDamage(EnemyParameters::LASTBOSS_DAMAGE);// �{�X���v���C���[�ɗ^����_���[�W��ݒ�
		break;
	}
	m_pBossBase->SetBarrierBreakSize(EnemyParameters::LASTBOSS_BARRIERBREAK_SIZE);// �o���A�j��p�[�e�B�N���̃T�C�Y�ݒ�
	m_pBossBase->SetDeadEffectSize(EnemyParameters::LASTBOSS_DEADEFFECT_SCALE);// ���S�G�t�F�N�g�̃T�C�Y


}
/*
*	@brief	�X�V����
*	@details ���X�{�X�̏�Ԃ��X�V����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void LastBoss::ChangeState()
{
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());// ���f���̃A�j���[�V�����X�V
}
/*
*	@brief	�`�揈��
*	@details ���X�{�X�̃��f���ƃV�[���h�AHP�o�[��`�悷��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void LastBoss::Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g
	auto states = m_pCommonResources->GetCommonStates();// �X�e�[�g
	// ���[���h�s���ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_pBossBase->GetScale())// �X�P�[��
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())// ��]
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());// �ʒu
	// �V�[���h�̃��[���h�s���ݒ�
	Matrix sheildWorld = Matrix::CreateScale(m_pBossBase->GetScale() * 10)// �X�P�[��(�V�[���h�̑傫��)
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())// ��]
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());// �ʒu
	m_pBossBase->GetBossSheild()->SetPosition(m_bossBS.Center);// �V�[���h�̍��W��ݒ�
	m_pBossBase->GetBossSheild()->SetRotation(m_pBossBase->GetQuaternion());// �V�[���h�̉�]��ݒ�
	m_pBossModel->Render(context, states, enemyWorld, view, proj);// ���f���`��
	m_pBossBase->GetBossSheild()->Render(context, states, sheildWorld, view, proj);// �V�[���h�`��
	Vector3 hpBarPos = m_pBossBase->GetPosition() - EnemyParameters::BOSS_HPBAR_OFFSET;// HP�o�[�̈ʒu��ݒ�
	m_pBossBase->GetHPBar()->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));// HP�o�[�̃X�P�[����ݒ�
	m_pBossBase->GetHPBar()->Render(view, proj, hpBarPos, m_rotate);// HP�o�[�`��

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
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation())// �e�̔��ˈʒu��ݒ�
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	m_bulletPosLeftDown = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTDOWN_GUN_OFFSET, transform);// �����̍��W�ɉ�]��K�p
	m_bulletPosLeftUp = Vector3::Transform(EnemyParameters::LASTBOSS_LEFTUP_GUN_OFFSET, transform);// ����̍��W�ɉ�]��K�p
	m_bulletPosRightDown = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTDOWN_GUN_OFFSET, transform);// �E���̍��W�ɉ�]��K�p
	m_bulletPosRightUp = Vector3::Transform(EnemyParameters::LASTBOSS_RIGHTUP_GUN_OFFSET, transform);// �E��̍��W�ɉ�]��K�p

}
/*
*	@brief	�e�𐶐�����
*	@details ���X�{�X�̒e�𐶐�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void LastBoss::CreateBullet()
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::LASTBOSS_BULLET_SIZE);// �e�̃T�C�Y��ݒ�
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);// �e�𔭎˂����I�u�W�F�N�g��ݒ�
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(BulletType::NORMAL);// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftUp, m_bulletDirection);// �e�𐶐��i����j
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeftDown, m_bulletDirection);// �e�𐶐��i�����j
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightUp, m_bulletDirection);// �e�𐶐��i�E��j
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRightDown, m_bulletDirection);// �e�𐶐��i�E���j

}

