/*
*	@file	EnemyChase.cpp
*	@brief	�G�ǐՃN���X
*/
#include <pch.h>
#include "EnemyChase.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G�ǐՃN���X�̃R���X�g���N�^
*	@param	EnemyAI* pEnemyAI �GAI
*	@return	�Ȃ�
*/
EnemyChase::EnemyChase(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// �GAI�̃|�C���^�[
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��]�̏�����
	, m_velocity(DirectX::SimpleMath::Vector3::Zero) // ���x�̏�����
	, m_scale(DirectX::SimpleMath::Vector3::One) // �X�P�[���̏�����
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero) // �����ʒu�̏�����
	, m_position(DirectX::SimpleMath::Vector3::Zero) // ���݈ʒu�̏�����
	, m_time(0.0f) // ���Ԃ̏�����
	, m_angle(0.0f) // �p�x�̏�����
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyChase::~EnemyChase()
{
	// �GAI�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyAI = nullptr;
}
/*
*	@brief	������
*	@details �G�ǐՃN���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemyChase::Initialize()
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
void EnemyChase::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �v���C���[�̃|�C���^�[���擾
	Player* player = m_pEnemyAI->GetEnemy()->GetPlayer();
	// �v���C���[�̍��W���擾
	Vector3 playerPos = player->GetPlayerPos();
	// ���Ԃ̉��Z
	m_time += elapsedTime;
	// �v���C���[�Ƃ̋������v�Z
	float distance = (playerPos - m_position).Length();
	// �v���C���[�Ƃ̋����ɉ����ď����𕪊�
	if (distance > STOP_DISTANCE)// ��~������蒷���ꍇ
	{
		// �v���C���[�ɋ߂Â�
		m_velocity = Seek(m_position, playerPos, elapsedTime * 10);
	}
	else// ��~�����ȉ��̏ꍇ
	{
		// �v���C���[�Ƃ̋������߂��̂Œ�~
		m_velocity = Vector3::Zero;
	}
	// �i�s�����ɉ�]
	if (m_velocity.LengthSquared() > 0.0f) // ���x���[���łȂ��ꍇ
	{
		// �i�s����������
		m_rotation = Quaternion::CreateFromYawPitchRoll(atan2f(m_velocity.x, m_velocity.z), 0.0f, 0.0f);
	}
	// �v���C���[�̈ʒu�x�N�g�����v�Z
	Vector3 toPlayer = playerPos - m_position;
	// �x�N�g���𐳋K��
	toPlayer.Normalize();
	// �v���C���[�����Ă���������擾
	Vector3 playerLook = player->GetPlayerDir();
	// �v���C���[�̎����ƓG�̈ʒu�x�N�g���̃h�b�g�ς��v�Z
	float dot = playerLook.Dot(toPlayer);
	// �h�b�g�ς���p�x���v�Z
	m_angle = acosf(dot);
	// ���E�ɓ��������荞�݂ɑJ��
	if (m_angle < DirectX::XM_PIDIV2)
	{
		// ��荞�ݏ�ԂɑJ��
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyChase());
		return; // �X�V���I��
	}


	// �G�̈ʒu���X�V
	m_pEnemyAI->SetPosition(m_position);
	// �G�̉�]���X�V
	m_pEnemyAI->SetRotation(m_rotation);
	// �G�̑��x���X�V
	m_pEnemyAI->SetVelocity(m_velocity);
}
