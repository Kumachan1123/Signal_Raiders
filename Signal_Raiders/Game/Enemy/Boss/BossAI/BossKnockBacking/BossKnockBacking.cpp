/*
	@file	BossKnockBacking.cpp
	@brief	�{�X�̃m�b�N�o�b�N���̏����N���X
*/
#include "pch.h"
#include "BossKnockBacking.h"

/*
*	@brief	�R���X�g���N�^
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̃R���X�g���N�^
*	@param	pBoss	�{�XAI�N���X�̃|�C���^
*	@return	�Ȃ�
*/
BossKnockBacking::BossKnockBacking(BossAI* pBoss)
	: m_pBoss(pBoss)// �{�XAI�N���X�̃|�C���^
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// ��]
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// ���x
	, m_scale(DirectX::SimpleMath::Vector3::One)// �X�P�[��
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// ���W�����l
	, m_time(0.0f)// ���Ԃ̏�����
	, m_rotationSpeed(0.0f)// ��]���x
	, m_angle(0.0f)// �p�x
{
}
/*
*	@brief �f�X�g���N�^
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@return �Ȃ�
*/
BossKnockBacking::~BossKnockBacking() {/*do nothing*/ }
/*
*	@brief ������
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̏�����
*	@param �Ȃ�
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
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossKnockBacking::Update(float elapsedTime)
{
	m_time += elapsedTime;// ���Ԃ̍X�V
	m_pBoss->SetVelocity(m_velocity);// ���x���Z�b�g
}

