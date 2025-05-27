/*
	@file	EnemyIdling.cpp
	@brief	�GIdling�N���X
*/
#include "pch.h"
#include "EnemyIdling.h"
/*
*	@brief	�R���X�g���N�^
*	@param	EnemyAI* enemy
*	@return	�Ȃ�
*/
EnemyIdling::EnemyIdling(EnemyAI* enemyAI)
	: m_enemyAI(enemyAI)// �GAI
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// ��]
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// ���x
	, m_scale(DirectX::SimpleMath::Vector3::One)// �X�P�[��
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// �����ʒu
	, m_time(0.0f)// ����
	, m_rotationSpeed(0.0f)// ��]���x
{}
/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
EnemyIdling::~EnemyIdling() {/*do nothing*/ }
/*
*	@brief	������
*	@return	�Ȃ�
*/
void EnemyIdling::Initialize()
{
	m_rotation = m_enemyAI->GetRotation();// ��]�擾
	m_velocity = m_enemyAI->GetVelocity();// ���x�擾
	m_scale = m_enemyAI->GetScale();// �X�P�[���擾
	m_initialPosition = m_enemyAI->GetPosition(); // �����ʒu�擾
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // ��]���x
}
/*
*	@brief	�X�V
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
	m_enemyAI->SetPosition(m_enemyAI->GetPosition() + forward * (m_velocity.Length()) * elapsedTime);  // �G�̈ʒu���X�V
	m_enemyAI->SetRotation(m_rotation);  // �G�̉�]���X�V
	m_enemyAI->SetVelocity(m_velocity);  // �G�̑��x���X�V
}

