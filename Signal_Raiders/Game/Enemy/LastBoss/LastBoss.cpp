/*
	@file	LastBoss.cpp
	@brief	���X�{�X�N���X
*/
#include <pch.h>
#include "LastBoss.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief	�R���X�g���N�^
*	@param[in] pBoss		�{�X�x�[�X�̃|�C���^
*	@param[in] resources	���ʃ��\�[�X
*	@return	�Ȃ�
*/
LastBoss::LastBoss(BossBase* pBoss, CommonResources* commonResources)
	: m_commonResources(commonResources)// ���ʃ��\�[�X
	, m_pBossBase(pBoss)// �{�X�̃|�C���^
	, m_bossBS{}// ���E���̏�����
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// �U���̃N�[���_�E���^�C��
	, m_specialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)// ����U���̃N�[���_�E���^�C��
	, m_initSpecialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)	// ����U���̃N�[���_�E���^�C��(�������p)
	, m_bulletCooldown(EnemyParameters::ATTACK_INTERVAL)// �e�̃N�[���_�E���^�C��
	, m_time(0.0f)// ����
	, m_position{}// �ʒu
	, m_velocity{}// ���x
	, m_rotate{}// ��]
	, m_enemyBSToPlayerArea{}// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	, m_enemyBulletBS{}// �G�̒e�̋��E��
	, m_playerBS{}// �v���C���[�̋��E��
	, m_matrix{}// �}�g���N�X
	, m_isDead(false)// ���S�t���O
	, m_isHitToPlayer(false)// �v���C���[�ɓ���������
	, m_isHitToOtherEnemy(false)// ���̓G�ɓ���������
	, m_isEnemyHitByPlayerBullet(false)// �G���v���C���[�̒e�ɓ���������
	, m_isPlayerHitByEnemyBullet(false)// �v���C���[���G�̒e�ɓ���������
	, m_canAttack(true)// �U���\��
	, m_isAttack(false)// �U������
	, m_bulletType(EnemyBullet::BulletType::NORMAL)// �e�̎��
	, m_audioManager{ }// �I�[�f�B�I�}�l�[�W���[
{
}
/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
LastBoss::~LastBoss() {  }// �e���폜
/*
*	@brief	����������
*	@return	�Ȃ�
*/
void LastBoss::Initialize()
{
	//m_pBossBase->SetBossSheildType(BossSheild::BossShieldType::LASTBOSS);// �{�X�̃V�[���h�̎�ނ�ݒ�
	m_pBossModel = std::make_unique<LastBossModel>();	// �{�X���f������ ����̓^�C�v�ɂ���ĕ���\��
	m_pBossModel->Initialize(m_commonResources);// �{�X���f�������� ����̓^�C�v�ɂ���ĕ���\��
	//m_pBossSheild = std::make_unique<BossSheild>(BossSheild::BossType::LASTBOSS, m_maxHP, this);// �V�[���h���� ����̓^�C�v�ɂ���ĕ���\��
	//m_pBossSheild->Initialize(m_commonResources);// �V�[���h������ ����̓^�C�v�ɂ���ĕ���\��

}
/*
*	@brief	�X�V����
*	@param[in] elapsedTime �o�ߎ���
*	@return	�Ȃ�
*/
void LastBoss::Update(float elapsedTime)
{
	//m_cameraEye = m_pCamera->GetEyePosition();// �J�����̈ʒu���擾
	//m_cameraTarget = m_pCamera->GetTargetPosition();// �J�����̒����_���擾
	//m_cameraUp = m_pCamera->GetUpVector();// �J�����̏�������擾
	m_pBossModel->SetState(m_pBossBase->GetBossAI()->GetState());// ���f���̃A�j���[�V�����X�V
	//m_pBossAI->Update(elapsedTime);// AI�̍X�V
	//m_position = m_pBossAI->GetPosition();// �G�̍��W���X�V
	//m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	//m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// �U���̃N�[���_�E���^�C�����擾
	//m_specialAttackCooldown -= elapsedTime;// ����U���̃N�[���_�E���^�C�������炷
	//this->ShootBullet();// �e����
	//m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// ���E���̒��S���W���X�V
	//this->BulletPositioning();// �e�̈ʒu�ݒ�
	//m_pHPBar->SetCurrentHP(m_currentHP);// HP�o�[��HP���X�V
	//m_pHPBar->Update(elapsedTime);// HP�o�[�̍X�V
	//if (m_currentHP <= m_maxHP / 2)m_pBossSheild->SetSheild(true);// HP�������ȉ��ɂȂ�����V�[���h���o��
	//m_pBossSheild->Update(elapsedTime);// �V�[���h�̍X�V
	//m_isDead = m_pHPBar->GetIsDead();// ���񂾂��ǂ������󂯎��
}
/*
*	@brief	�`�揈��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void LastBoss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
*	@brief	�����蔻��`�揈��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void LastBoss::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// ���g�p�̌x�����o���Ȃ�
	UNREFERENCED_PARAMETER(proj);// ���g�p�̌x�����o���Ȃ�
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);// �`��J�n
	DirectX::XMVECTOR color = Colors::Black;// �F
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// ��������
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// �������Ă��Ȃ�
	DrawCollision::DrawBoundingSphere(m_bossBS, color);// ���E���̕`��
	DrawCollision::DrawEnd();// �`��I��
#endif
}
