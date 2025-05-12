/*
	@file	BossIdling.cpp
	@brief	�{�X�p�j�N���X
*/
#include "pch.h"
#include "BossIdling.h"

using namespace DirectX::SimpleMath;

/*
*	@brief	�R���X�g���N�^
*	@param[in]	pBoss	�{�XAI�N���X�̃|�C���^
*	@return	�Ȃ�
*/
BossIdling::BossIdling(BossAI* pBoss)
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
BossIdling::~BossIdling() {}
/*
*	@brief ������
*	@return �Ȃ�
*/
void BossIdling::Initialize()
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
void BossIdling::Update(float elapsedTime)
{
	m_time += elapsedTime;// ���Ԃ̍X�V
	//m_angle = CalculateAngle(m_pBoss->GetPosition(), m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());// �v���C���[�̕������擾���A���ʂ���������
	//m_angle = Lerp(m_angle, CalculateAngle(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pBoss->GetPosition()), m_time);// �v���C���[�̕������擾���A���]������
	//m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// ��]
	//m_pBoss->SetRotation(m_rotation);// ��]���Z�b�g
	m_pBoss->SetVelocity(m_velocity);// ���x���Z�b�g
}

