/*
*	@file	EnemySpin.cpp
*	@brief	�G�X�s���N���X
*/
#include "pch.h"
#include "EnemySpin.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G�X�s���N���X�̃R���X�g���N�^
*	@param	EnemyAI* pEnemyAI �GAI
*	@return	�Ȃ�
*/
EnemySpin::EnemySpin(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)
	, m_scale(DirectX::SimpleMath::Vector3::One)
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_time(0.0f)
	, m_angle(0.0f)
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemySpin::~EnemySpin()
{
	m_pEnemyAI = nullptr; // �GAI�̃|�C���^�[��nullptr�ɐݒ�
}
/*
*	@brief	������
*	@details �G�X�s���N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemySpin::Initialize()
{
	m_rotation = m_pEnemyAI->GetRotation();// �G�̉�]���擾
	m_velocity = m_pEnemyAI->GetVelocity(); // �G�̈ړ����x���擾
	m_scale = m_pEnemyAI->GetScale(); // �G�̃X�P�[�����擾
	m_initialPosition = m_pEnemyAI->GetPosition(); // �G�̏����ʒu���擾
}
/*
*	@brief	�X�V
*	@details �G�X�s���N���X�̍X�V
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemySpin::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// ���Ԃ̉��Z
	m_angle = CalculateAngle(m_pEnemyAI->GetPosition(), m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());// �v���C���[�̕������擾���A���ʂ���������
	m_angle = Lerp(m_angle, CalculateAngle(m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pEnemyAI->GetPosition()), m_time);// �v���C���[�̕������擾���A���]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// �G����]������
	m_pEnemyAI->SetRotation(m_rotation);// �G�̉�]��ݒ�
	m_pEnemyAI->SetVelocity(m_velocity);// �G�̈ړ����x��ݒ�
}


