/*
*	@file	EnemyShadowStep.cpp
*	@brief	�G�V���h�E�X�e�b�v�N���X
*/
#include <pch.h>
#include "EnemyShadowStep.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G�V���h�E�X�e�b�v�N���X�̃R���X�g���N�^
*	@param	EnemyAI* pEnemyAI �GAI
*	@return	�Ȃ�
*/
EnemyShadowStep::EnemyShadowStep(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// �GAI�̃|�C���^�[
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��]�̏�����
	, m_velocity(DirectX::SimpleMath::Vector3::Zero) // ���x�̏�����
	, m_scale(DirectX::SimpleMath::Vector3::One) // �X�P�[���̏�����
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero) // �����ʒu�̏�����
	, m_position(DirectX::SimpleMath::Vector3::Zero) // ���݈ʒu�̏�����
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero) // �m�b�N�o�b�N�J�n�ʒu�̏�����
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero) // �m�b�N�o�b�N�I���ʒu�̏�����
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero) // �m�b�N�o�b�N�̏������x�̏�����
	, m_time(0.0f) // ���Ԃ̏�����
	, m_angle(0.0f) // �p�x�̏�����
	, m_initialized(false) // �������t���O�̏�����
	, m_randomRadius(0.0f) // �����_���Ȕ��a�̏�����
	, m_elapsedAngle(0.0f) // �ݐϊp�x�̏�����
	, m_rotateDirection(1) // ��]�����̏�����
	, m_evadeRadius(0.0f) // ��荞�݂̔��a�̏�����
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyShadowStep::~EnemyShadowStep()
{
	// �GAI�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyAI = nullptr;
}
/*
*	@brief	������
*	@details �G�V���h�E�X�e�b�v�N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemyShadowStep::Initialize()
{
	// AI����ʒu���擾���Đݒ肷��
	m_position = m_pEnemyAI->GetPosition();
	// AI�����]���擾���Đݒ肷��
	m_rotation = m_pEnemyAI->GetRotation();
	// AI����ړ����x���擾���Đݒ肷��
	m_velocity = m_pEnemyAI->GetVelocity();
	// AI����X�P�[�����擾���Đݒ肷��
	m_scale = m_pEnemyAI->GetScale();
	// AI���珉���ʒu���擾���Đݒ肷��
	m_initialPosition = m_pEnemyAI->GetPosition();
}
/*
*	@brief	�X�V
*	@details �G�X�s���N���X�̍X�V
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemyShadowStep::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���Ԃ̉��Z
	m_time += elapsedTime;
	// �v���C���[�̃|�C���^�[���擾
	Player* player = m_pEnemyAI->GetEnemy()->GetPlayer();
	// �v���C���[�̍��W���擾
	Vector3 playerPos = player->GetPlayerPos();
	// AI����ʒu���擾���Đݒ肷��
	m_position = m_pEnemyAI->GetPosition();
	// Chase����ShadowStep�ɂȂ����Ƃ�
	if (!m_initialized)
	{
		// 10����30�͈̔�
		m_randomRadius = MinRandomRadius + rand() % (int)RadiusRange;
		// ��荞�݂̔��a��ݒ�
		m_evadeRadius = m_randomRadius;
		// ��荞�݂̒��S�̓v���C���[�̏����ʒu
		m_centerPos = player->GetPlayerPos();
		// �G�̈ʒu����v���C���[�̏����ʒu�܂ł̃x�N�g�����v�Z
		Vector3 toPlayer = m_position - m_centerPos;
		// ���߂��x�N�g������p�x���v�Z
		m_angle = atan2f(toPlayer.z, toPlayer.x);
		// ��]�̓r���o�߂�������
		m_elapsedAngle = 0.0f;
		// ��荞�݂̂��߂̊p�x�������_���ɐݒ�
		m_rotateDirection = (rand() % 2 == 0) ? 1 : -1;
		// �������t���O��true�ɐݒ�
		m_initialized = true;
	}
	// ��]�p�x�X�V
	float deltaAngle = XM_PI * elapsedTime;
	// ��荞�݂̊p�x���X�V
	m_angle += deltaAngle * m_rotateDirection;
	// ��荞�݂̈ʒu���v�Z
	m_elapsedAngle += deltaAngle;
	// �v���C���[�̏����ʒu�𒆐S�Ƃ����~��̖ڕW���W
	float x = cosf(m_angle) * m_evadeRadius;// X���W
	float z = sinf(m_angle) * m_evadeRadius;// Z���W
	// �ڕW���W���v�Z
	m_targetPos = m_centerPos + Vector3(x, 0.0f, z);
	// ���݈ʒu���炻�̖ڕW�n�_�ւ̃x�N�g��
	Vector3 direction = m_targetPos - m_position;
	// �����x�N�g���𐳋K��
	direction.Normalize();
	// �����x�N�g���ɑ��x���|���Ĉړ��ʂ��v�Z
	m_velocity = direction * 50.0f * elapsedTime;
	// �G�̉�]���X�V
	if (m_velocity.LengthSquared() > 0.0f)
	{
		// �i�s����������
		m_rotation = Quaternion::CreateFromYawPitchRoll
		(atan2f(m_velocity.x, m_velocity.z), 0.0f, 0.0f);
		// �GAI�ɉ�]��ݒ�
		m_pEnemyAI->SetRotation(m_rotation);
	}
	// �ʒu���X�V
	m_position += m_velocity;
	// �V�����ʒu��ݒ�
	m_pEnemyAI->SetPosition(m_position);
	// ����������I��
	if (m_elapsedAngle >= XM_PI * 3)
	{
		// �������t���O��false�ɐݒ�
		m_initialized = false;
		// �v���C���[�̈ʒu���擾
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyChase());
	}
}
