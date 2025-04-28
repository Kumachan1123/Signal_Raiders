/*
	@file	Boss.cpp
	@brief	�{�X�N���X
*/
#include "pch.h"
#include "Boss.h"
//// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/BossAI/BossAI.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief	�R���X�g���N�^
*	@param[in] pBoss		�{�X�x�[�X�̃|�C���^
*	@param[in] resources	���ʃ��\�[�X
*	@return	�Ȃ�
*/
Boss::Boss(BossBase* pBoss, CommonResources* commonResources)
	: m_commonResources(commonResources)// ���ʃ��\�[�X
	, m_pBossBase(pBoss)// �{�X�̃|�C���^
	, m_bossBS{}// ���E���̏�����
	, m_pBossModel{}// �{�X���f��
	, m_pHPBar{}// HP�o�[
	, m_time(0.0f)// ����
	, m_position{}// �ʒu
	, m_velocity{}// ���x
	, m_rotate{}// ��]
	, m_matrix{}// �}�g���N�X
	, m_bulletType(EnemyBullet::BulletType::NORMAL)// �e�̎��
{
}
/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
Boss::~Boss() {  }
/*
*	@brief	����������
*	@return	�Ȃ�
*/
void Boss::Initialize()
{
	m_pBossModel = std::make_unique<BossModel>();	// �{�X���f������
	m_pBossModel->Initialize(m_commonResources);// �{�X���f��������
	m_pBossBase->SetDefaultHitRadius(EnemyParameters::NORMAL_BOSS_RADIUS);// �ʏ펞�{�X�̓����蔻���ݒ�
	m_pBossBase->SetDefensiveHitRadius(EnemyParameters::BOSS_SHIELD_RADIUS);// �V�[���h�W�J���̃{�X�̓����蔻���ݒ�
	m_pBossBase->SetBulletSize(EnemyParameters::BOSS_BULLET_SIZE);// �e�̃T�C�Y��ݒ�
	m_pBossBase->SetToPlayerDamage(EnemyParameters::BOSS_DAMAGE);// �{�X���v���C���[�ɗ^����_���[�W��ݒ�
}
/*
*	@brief	�X�V����
*	@return	�Ȃ�
*/
void Boss::ChangeState()
{
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());// ���f���̃A�j���[�V�����X�V
}
/*
*	@brief	�`�揈��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void Boss::Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g
	auto states = m_commonResources->GetCommonStates();// �X�e�[�g
	// ���[���h�s���ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_pBossBase->GetScale())// �X�P�[��
		* Matrix::CreateFromQuaternion(m_pBossBase->GetQuaternion())// ��]
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());// �ʒu
	// �V�[���h�̃��[���h�s���ݒ�
	Matrix sheildWorld = Matrix::CreateScale(m_pBossBase->GetScale() * 3)// �X�P�[��(�V�[���h�̑傫��)
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
*	@return	�Ȃ�
*/
void Boss::BulletPositioning()
{
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossBase->GetBossAI()->GetRotation())// �e�̔��ˈʒu��ݒ�
		* Matrix::CreateTranslation(m_pBossBase->GetPosition());
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);// �����̍��W�ɉ�]��K�p
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);// ���̍��W�ɉ�]��K�p
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);// �E�̍��W�ɉ�]��K�p
}
/*
*	@brief	�e�̐���
*	@return	�Ȃ�
*/
void Boss::CreateBullet()
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE);// �e�̃T�C�Y��ݒ�
	m_pBossBase->GetBulletManager()->SetShooter(m_pBossBase);// �e�𔭎˂����I�u�W�F�N�g��ݒ�
	switch (m_pBossBase->GetBulletType())	// Enemies�N���X�Őݒ肵���e�̃^�C�v�ɂ���ď����𕪊�
	{
	case BossBase::BossBulletType::STAGE_1:// �ʏ�e
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// �����̒e�𔭎�
		break;
	case BossBase::BossBulletType::STAGE_2:// ��

		CreateLeftBullet(EnemyBullet::BulletType::NORMAL);// ���̒e�𔭎�
		CreateRightBullet(EnemyBullet::BulletType::NORMAL);// �E�̒e�𔭎�
		break;
	case BossBase::BossBulletType::STAGE_3:// �O��
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// �����̒e�𔭎�
		CreateLeftBullet(EnemyBullet::BulletType::NORMAL);// ���̒e�𔭎�
		CreateRightBullet(EnemyBullet::BulletType::NORMAL);// �E�̒e�𔭎�
		break;

	}
}
/*
*	@brief	��������e�𔭎�
*	@param[in] type �e�̎��
*/
void Boss::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// �e�𐶐�
}
/*
*	@brief	���̒e�𔭎�
*	@param[in] type �e�̎��
*/
void Boss::CreateLeftBullet(EnemyBullet::BulletType type)
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosLeft, m_bulletDirection);// �e�𐶐�
}
/*
*	@brief �E�̒e�𔭎�
*	@param[in] type �e�̎��
*/
void Boss::CreateRightBullet(EnemyBullet::BulletType type)
{
	m_pBossBase->GetBulletManager()->SetEnemyBulletType(type);// �e�̎�ނ�ݒ�
	m_pBossBase->GetBulletManager()->CreateEnemyBullet(m_bulletPosRight, m_bulletDirection);// �e�𐶐�
}