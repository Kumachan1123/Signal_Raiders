#include <pch.h>
#include "SpecialBullet.h"
using namespace DirectX::SimpleMath;
SpecialBullet::SpecialBullet()
	: m_pEnemyBullet(nullptr)
	, m_rotationSpeed(0.0f)
	, m_distance(5.0f)
	, m_height(0.0f)
	, m_spiralAngle(0.0f)
	, m_elapsedTime(0.0f)
	, m_time(0.0f)
	, m_basePos(Vector3::Zero)
	, m_positionOffSet(Vector3::Zero)
	, m_currentTarget(Vector3::Zero)
{
}

SpecialBullet::~SpecialBullet()
{
}

void SpecialBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// �e�̍��W
	m_velocity = m_pEnemyBullet->GetVelocity();// �e�̑��x
	m_direction = m_pEnemyBullet->GetDirection();// �e�̕���
	m_spiralAngle = m_pEnemyBullet->GetAngle();// ��]�e�̊p�x
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// �u�e�v���E��

}

void SpecialBullet::Update(float elapsedTime)
{
	m_time += elapsedTime;
	m_elapsedTime = elapsedTime;
	// ���v���ɉ�]���邽�߂̊p�x
	m_spiralAngle += m_rotationSpeed * elapsedTime;
	// XY���ʏ�ŉ~�^�� (���v���)
	float xOffset = cosf(m_spiralAngle) * m_distance;
	float zOffset = sinf(m_spiralAngle) * m_distance;
	// ���Ƃ��Ƃ�Y���W�̓����͕ύX���Ȃ�
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);
	Expand();// �q�I�u�W�F�N�g��W�J
	Shot();// �q�I�u�W�F�N�g�𔭎�
	StopExpand();// �q�I�u�W�F�N�g�����[
	ComeBack();// �q�I�u�W�F�N�g��߂�
	// �v���C���[�Ɍ������X�p�C������`���Ĉړ�
	m_position = m_basePos + m_positionOffSet;
	m_boundingSphere.Center = m_position;
	// �e�̎����ɉ����ăt���O��؂�ւ���
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)
	{
		m_pEnemyBullet->SetIsShot(true);
	}
	// �e�̊e������X�V
	m_pEnemyBullet->SetDirection(m_direction);// �e�̕���
	m_pEnemyBullet->SetVelocity(m_velocity);// �e�̑��x
	m_pEnemyBullet->SetPosition(m_position);// �e�̍��W
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// �u�e�v���E��
}

void SpecialBullet::Expand()
{
	if (!m_pEnemyBullet->GetIsExpand())return;
	m_rotationSpeed = 1.0f; // ���x�����p�i�l��傫������Ƒ�����]����j
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);
	m_height = 2.0f;
}

void SpecialBullet::Shot()
{
	if (!m_pEnemyBullet->GetIsShot()) return;
	m_rotationSpeed = 3.0f;
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetCurrentTarget(), m_elapsedTime * 2);

}

void SpecialBullet::StopExpand()
{
	if (m_pEnemyBullet->GetIsExpand())return;
	m_rotationSpeed = 0.0f;
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);
	m_height = 1.50f;
}

void SpecialBullet::ComeBack()
{
	if (m_pEnemyBullet->GetIsShot()) return;
	//m_look.Normalize();// �v���C���[�������Ă�������𐳋K��
	//// ��_��e�������Ă�������ɓ�����
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);
	// ��_��ړI�n�Ɍ������Đ��`�⊮
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetEnemyPosition(), m_elapsedTime * 50);
}
