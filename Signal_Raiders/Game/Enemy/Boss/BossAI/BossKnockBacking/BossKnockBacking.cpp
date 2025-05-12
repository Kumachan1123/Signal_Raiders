/*
	@file	BossKnockBacking.cpp
	@brief	�{�X�̃m�b�N�o�b�N���̏����N���X
*/
#include "pch.h"
#include "BossKnockBacking.h"
using namespace DirectX::SimpleMath;
/*
*	@brief	�R���X�g���N�^
*	@param[in]	pBoss	�{�XAI�N���X�̃|�C���^
*	@return	�Ȃ�
*/
BossKnockBacking::BossKnockBacking(BossAI* pBoss)
	: m_pBoss(pBoss)// �{�XAI�N���X�̃|�C���^
	, m_rotation(Quaternion::Identity)// ��]
	, m_velocity(Vector3::Zero)// ���x
	, m_scale(Vector3::One)// �X�P�[��
	, m_initialPosition(Vector3::Zero)// ���W�����l
	, m_time(0.0f)// ���Ԃ̏�����
	, m_rotationSpeed(0.0f)// ��]���x
	, m_angle(0.0f)// �p�x
{
}
/*
*	@brief �f�X�g���N�^
*	@return �Ȃ�
*/
BossKnockBacking::~BossKnockBacking() {/*do nothing*/ }
/*
*	@brief ������
*	@return �Ȃ�
*/
void BossKnockBacking::Initialize()
{
	m_rotation = m_pBoss->GetRotation();// ��]
	m_velocity = m_pBoss->GetVelocity();// ���x
	m_scale = m_pBoss->GetScale();// �X�P�[��
	m_initialPosition = m_pBoss->GetPosition();// ���W�����l
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // ��]���x
}

/*
*	@brief �X�V
*	@param[in] elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossKnockBacking::Update(float elapsedTime)
{
	m_time += elapsedTime;// ���Ԃ̍X�V
	m_pBoss->SetVelocity(m_velocity);// ���x���Z�b�g
}

