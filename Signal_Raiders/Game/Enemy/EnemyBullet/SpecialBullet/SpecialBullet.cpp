/*
*	@file SpecialBullet.cpp
*	@brief �G�e�N���X(����e)
*/
#include <pch.h>
#include "SpecialBullet.h"
/*
*	@brief �R���X�g���N�^
*	@details �G�e�N���X(����e)�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SpecialBullet::SpecialBullet()
	: m_pEnemyBullet(nullptr)// �G�e�|�C���^�[
	, m_pCommonResources(nullptr)// ���ʃ��\�[�X
	, m_rotationSpeed(0.0f)// �e�̉�]���x
	, m_distance(5.0f)// �e�̓G�Ƃ̋���
	, m_height(0.0f)// �e�̍���
	, m_spiralAngle(0.0f)// ��]�e�̊p�x
	, m_elapsedTime(0.0f)// �o�ߎ���
	, m_time(0.0f)// ����
	, m_basePos(DirectX::SimpleMath::Vector3::Zero)// �e�̊�ʒu
	, m_positionOffSet(DirectX::SimpleMath::Vector3::Zero)// �e�̈ʒu�I�t�Z�b�g
	, m_currentTarget(DirectX::SimpleMath::Vector3::Zero)// ���݂̃^�[�Q�b�g�̈ʒu
	, m_seVolume(0.0f)// SE����
	, m_isPlayChargeSE(false)// �`���[�WSE�Đ��t���O
	, m_isPlayShotSE(false)// ����SE�Đ��t���O
{
}
/*
*	@brief �f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
SpecialBullet::~SpecialBullet()
{
	// ���ʃ��\�[�X�̃|�C���^�[��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
	// �G�e�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyBullet = nullptr;
}
/*
*	@brief �������֐�
*	@details �G�e�N���X(����e)�̏������֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpecialBullet::Initialize()
{
	// �e�̍��W��ݒ�
	m_position = m_pEnemyBullet->GetPosition();
	// �e�̑��x��ݒ�
	m_velocity = m_pEnemyBullet->GetVelocity();
	// �e�̕�����ݒ�
	m_direction = m_pEnemyBullet->GetDirection();
	// ��]�e�̊p�x��ݒ�
	m_spiralAngle = m_pEnemyBullet->GetAngle();
	// �e�̋��E����ݒ�
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();
}
/*
*	@brief �X�V�֐�
*	@details �G�e�N���X(����e)�̍X�V�֐�
*	@param float elapsedTime �X�V����
*	@return �Ȃ�
*/
void SpecialBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_pCommonResources->GetAudioManager()->Update();
	// ���Ԃ̉��Z
	m_time += elapsedTime;
	// �o�ߎ��Ԃ�ۑ�
	m_elapsedTime = elapsedTime;
	// ���v���ɉ�]���邽�߂̊p�x
	m_spiralAngle += m_rotationSpeed * elapsedTime;
	// XY���ʏ�ŉ~�^�� (���v���)
	// X���W�̃I�t�Z�b�g
	float xOffset = cosf(m_spiralAngle) * m_distance;
	// Z���W�̃I�t�Z�b�g
	float zOffset = sinf(m_spiralAngle) * m_distance;
	// ���Ƃ��Ƃ�Y���W�̓����͕ύX���Ȃ�
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);
	// �q�I�u�W�F�N�g��W�J
	Expand();
	// �q�I�u�W�F�N�g�𔭎�
	Shot();
	// �q�I�u�W�F�N�g�����[
	StopExpand();
	// �q�I�u�W�F�N�g��߂�
	ComeBack();
	// �v���C���[�Ɍ������X�p�C������`���Ĉړ�
	m_position = m_basePos + m_positionOffSet;
	// ���E���̒��S���W��e�̍��W�ɍX�V
	m_boundingSphere.Center = m_position;
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)m_pEnemyBullet->SetIsShot(true);// �ҋ@���Ԃ��߂����甭��
	// �e�̕������X�V
	m_pEnemyBullet->SetDirection(m_direction);
	// �e�̑��x���X�V
	m_pEnemyBullet->SetVelocity(m_velocity);
	// �e�̍��W���X�V
	m_pEnemyBullet->SetPosition(m_position);
	// �e�̋��E�����X�V
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);
}
/*
*	@brief ��]�e��W�J����
*	@details ��]�e��W�J����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpecialBullet::Expand()
{
	// �W�J���Ă��Ȃ��ꍇ�͉������Ȃ�
	if (!m_pEnemyBullet->GetIsExpand())return;
	// ��]���x��ݒ�
	m_rotationSpeed = 1.0f;
	// �e�̋�����⊮
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);
	// �e�̍�����⊮
	m_height = 2.0f;
	// SE���Đ�����Ă��Ȃ��ꍇ
	if (!m_isPlayChargeSE)
	{
		// �W�J�����Đ�
		m_pCommonResources->GetAudioManager()->PlaySound("ChargeSpecial", m_seVolume);
		// SE�Đ��t���O�𗧂Ă�
		m_isPlayChargeSE = true;
	}
}
/*
*	@brief ��]�e�𔭎˂���
*	@details ��]�e�𔭎˂���֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpecialBullet::Shot()
{
	// ���˂��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (!m_pEnemyBullet->GetIsShot()) return;
	// ��]���x��ݒ�
	m_rotationSpeed = 3.0f;
	// �e�̋�����⊮
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);
	// ��_��ړI�n�Ɍ������Đ��`�⊮
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetCurrentTarget(), m_elapsedTime * 2);
	// SE���Đ�����Ă��Ȃ��ꍇ
	if (!m_isPlayShotSE)
	{
		// ���ˉ����Đ�
		m_pCommonResources->GetAudioManager()->PlaySound("SpecialAttack", m_seVolume);
		// SE�Đ��t���O�𗧂Ă�
		m_isPlayShotSE = true;
	}
}
/*
*	@brief ��]�e�̓W�J����߂�
*	@details ��]�e�̓W�J����߂�֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpecialBullet::StopExpand()
{
	// �W�J���Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_pEnemyBullet->GetIsExpand())return;
	// ��]���~�߂�
	m_rotationSpeed = 0.0f;
	// �e�̋�����⊮
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);
	// �e�̍�����␳
	m_height = 1.50f;
}
/*
*	@brief ��]�e�������̎���ɖ߂�
*	@details ��]�e�������̎���ɖ߂��֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpecialBullet::ComeBack()
{
	// ���˂��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_pEnemyBullet->GetIsShot()) return;
	// ��_��e�������Ă�������ɓ�����
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);
	// ��_��ړI�n�Ɍ������Đ��`�⊮
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetEnemyPosition(), m_elapsedTime * 50);
}
