/*
*	@file	EnemyIdling.cpp
*	@brief	�G�p�j�N���X
*/
#include <pch.h>
#include "EnemyIdling.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G�p�j�N���X�̃R���X�g���N�^
*	@param	EnemyAI* pEnemyAI
*	@return	�Ȃ�
*/
EnemyIdling::EnemyIdling(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// �GAI
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// ��]
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// ���x
	, m_scale(DirectX::SimpleMath::Vector3::One)// �X�P�[��
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// �����ʒu
	, m_time(0.0f)// ����
	, m_rotationSpeed(0.0f)// ��]���x
{}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyIdling::~EnemyIdling()
{
	m_pEnemyAI = nullptr; // �GAI�̃|�C���^�[��nullptr�ɐݒ�
}
/*
*	@brief	������
*	@details �G�p�j�N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemyIdling::Initialize()
{
	m_rotation = m_pEnemyAI->GetRotation();// ��]�擾
	m_velocity = m_pEnemyAI->GetVelocity();// ���x�擾
	m_scale = m_pEnemyAI->GetScale();// �X�P�[���擾
	m_initialPosition = m_pEnemyAI->GetPosition(); // �����ʒu�擾
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // ��]���x
}
/*
*	@brief	�X�V
*	@details �G�p�j�N���X�̍X�V
*	@param	float elapsedTime
*	@return	�Ȃ�
*/
void EnemyIdling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// sin�g���g������]�̍X�V
	float randomMultiplier = GenerateRandomMultiplier(EnemyParameters::RANDOM_ROTATION_MIN, EnemyParameters::RANDOM_ROTATION_MAX);  // �����_���Ȕ{���𐶐�
	float sinRotationSpeed = m_rotationSpeed + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);  // ��]���x��sin�g�ŕω�������
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);  // ����������]���x�Ɋ�Â��A������iY���j�𒆐S�ɉ�]�𐶐�
	m_rotation *= deltaRotation;  // �����̉�]�ɐV������]��K�p
	m_rotation.Normalize();  // ��]�𐳋K�����A���肵���N�H�[�^�j�I�����ێ�
	Vector3 forward = Vector3::Transform(Vector3::Backward * EnemyParameters::MOVE_SPEED, m_rotation);  // �O���x�N�g�����擾
	m_pEnemyAI->SetPosition(m_pEnemyAI->GetPosition() + forward * (m_velocity.Length()) * elapsedTime);  // �G�̈ʒu���X�V
	m_pEnemyAI->SetRotation(m_rotation);  // �G�̉�]���X�V
	m_pEnemyAI->SetVelocity(m_velocity);  // �G�̑��x���X�V
}

