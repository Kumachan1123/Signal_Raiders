/*
	@file	EnemySpin.cpp
	@brief	�G�X�s���N���X
*/
#include "pch.h"
#include "EnemySpin.h"
/*
*	@brief	�R���X�g���N�^
*	@param	EnemyAI* enemyAI �GAI
*	@return	�Ȃ�
*/
EnemySpin::EnemySpin(EnemyAI* enemyAI)
	: m_enemyAI(enemyAI)
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
*	@return	�Ȃ�
*/
EnemySpin::~EnemySpin() {/*do nothing*/ }
/*
*	@brief	������
*	@return	�Ȃ�
*/
void EnemySpin::Initialize()
{
	m_rotation = m_enemyAI->GetRotation();// �G�̉�]���擾
	m_velocity = m_enemyAI->GetVelocity(); // �G�̈ړ����x���擾
	m_scale = m_enemyAI->GetScale(); // �G�̃X�P�[�����擾
	m_initialPosition = m_enemyAI->GetPosition(); // �G�̏����ʒu���擾
}
/*
*	@brief	�X�V
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemySpin::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// ���Ԃ̉��Z
	m_angle = CalculateAngle(m_enemyAI->GetPosition(), m_enemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());// �v���C���[�̕������擾���A���ʂ���������
	m_angle = Lerp(m_angle, CalculateAngle(m_enemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_enemyAI->GetPosition()), m_time);// �v���C���[�̕������擾���A���]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// �G����]������
	m_enemyAI->SetRotation(m_rotation);// �G�̉�]��ݒ�
	m_enemyAI->SetVelocity(m_velocity);// �G�̈ړ����x��ݒ�
}


