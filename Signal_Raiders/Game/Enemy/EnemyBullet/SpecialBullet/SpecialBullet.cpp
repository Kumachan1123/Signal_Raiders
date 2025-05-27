/*
*	@file SpecialBullet.cpp
*	@brief �G�e�N���X(����e)
*/
#include <pch.h>
#include "SpecialBullet.h"
/*
*	@brief �R���X�g���N�^
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
*	@return �Ȃ�
*/
SpecialBullet::~SpecialBullet() {/*do nothing*/ }
/*
*	@brief �������֐�
*	@return �Ȃ�
*/
void SpecialBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// �e�̍��W
	m_velocity = m_pEnemyBullet->GetVelocity();// �e�̑��x
	m_direction = m_pEnemyBullet->GetDirection();// �e�̕���
	m_spiralAngle = m_pEnemyBullet->GetAngle();// ��]�e�̊p�x
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// �u�e�v���E��
}
/*
*	@brief �X�V�֐�
*	@param[in] float elapsedTime �X�V����
*	@return �Ȃ�
*/
void SpecialBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources->GetAudioManager()->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_time += elapsedTime;// ���Ԃ̉��Z
	m_elapsedTime = elapsedTime;// �o�ߎ��Ԃ�ۑ�
	m_spiralAngle += m_rotationSpeed * elapsedTime;	// ���v���ɉ�]���邽�߂̊p�x
	// XY���ʏ�ŉ~�^�� (���v���)
	float xOffset = cosf(m_spiralAngle) * m_distance;// X���W�̃I�t�Z�b�g
	float zOffset = sinf(m_spiralAngle) * m_distance;// Z���W�̃I�t�Z�b�g
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);	// ���Ƃ��Ƃ�Y���W�̓����͕ύX���Ȃ�
	Expand();// �q�I�u�W�F�N�g��W�J
	Shot();// �q�I�u�W�F�N�g�𔭎�
	StopExpand();// �q�I�u�W�F�N�g�����[
	ComeBack();// �q�I�u�W�F�N�g��߂�
	m_position = m_basePos + m_positionOffSet;	// �v���C���[�Ɍ������X�p�C������`���Ĉړ�
	m_boundingSphere.Center = m_position;	// ���E���̒��S���W��e�̍��W�ɍX�V
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)	// �e�̎����ɉ����ăt���O��؂�ւ���
		m_pEnemyBullet->SetIsShot(true);// �e�𔭎˂���
	// �e�̊e������X�V
	m_pEnemyBullet->SetDirection(m_direction);// �e�̕���
	m_pEnemyBullet->SetVelocity(m_velocity);// �e�̑��x
	m_pEnemyBullet->SetPosition(m_position);// �e�̍��W
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// �u�e�v���E��
}
/*
*	@brief ��]�e��W�J����
*	@return �Ȃ�
*/
void SpecialBullet::Expand()
{
	if (!m_pEnemyBullet->GetIsExpand())return;// �W�J���Ă��Ȃ��ꍇ�͉������Ȃ�
	m_rotationSpeed = 1.0f; // ���x�����p�i�l��傫������Ƒ�����]����j
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);// �e�̋�����⊮
	m_height = 2.0f; // �e�̍�����⊮
	if (!m_isPlayChargeSE)// SE���Đ�����Ă��Ȃ��ꍇ
	{
		m_pCommonResources->GetAudioManager()->PlaySound("ChargeSpecial", m_seVolume);// SE���Đ�
		m_isPlayChargeSE = true;// SE�Đ��t���O�𗧂Ă�
	}

}
/*
*	@brief ��]�e�𔭎˂���
*	@return �Ȃ�
*/
void SpecialBullet::Shot()
{
	if (!m_pEnemyBullet->GetIsShot()) return;// ���˂��Ă��Ȃ��ꍇ�͉������Ȃ�
	m_rotationSpeed = 3.0f; // ���x�����p�i�l��傫������Ƒ�����]����j
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);// �e�̋�����⊮
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetCurrentTarget(), m_elapsedTime * 2);// ��_��ړI�n�Ɍ������Đ��`�⊮
	if (!m_isPlayShotSE)// SE���Đ�����Ă��Ȃ��ꍇ
	{
		m_pCommonResources->GetAudioManager()->PlaySound("SpecialAttack", m_seVolume);// SE���Đ�
		m_isPlayShotSE = true;// SE�Đ��t���O�𗧂Ă�
	}

}
/*
*	@brief ��]�e�̓W�J����߂�
*	@return �Ȃ�
*/
void SpecialBullet::StopExpand()
{
	if (m_pEnemyBullet->GetIsExpand())return;// �W�J���Ă��Ȃ��ꍇ�͉������Ȃ�
	m_rotationSpeed = 0.0f; // ���x�����p�i�l��傫������Ƒ�����]����j
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);// �e�̋�����⊮
	m_height = 1.50f; // �e�̍�����␳
}
/*
*	@brief ��]�e�������̎���ɖ߂�
*	@return �Ȃ�
*/
void SpecialBullet::ComeBack()
{
	if (m_pEnemyBullet->GetIsShot()) return;// ���˂��Ă��Ȃ��ꍇ�͉������Ȃ�
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);	// ��_��e�������Ă�������ɓ�����
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetEnemyPosition(), m_elapsedTime * 50);// ��_��ړI�n�Ɍ������Đ��`�⊮
}
