/*
*	@file	EnemyAI.cpp
*	@brief	�GAI�N���X
*/
#include <pch.h>
#include "EnemyAI.h"

/*
*	@brief	�R���X�g���N�^
*	@details �GAI�N���X�̃R���X�g���N�^
*	@param	IEnemy* pEnemy�@�G
*	@return	�Ȃ�
*/
EnemyAI::EnemyAI(IEnemy* pEnemy)
	: m_pCurrentState(nullptr)// ���݂̏��
	, m_attackCooldown(0.0f)// �U���N�[���_�E��
	, m_time(0.0f)// ����
	, m_isHitPlayerBullet(false)// �v���C���[�̒e�ɓ���������
	, m_knockTime(0.0f)// �m�b�N�o�b�N����
	, m_pEnemyAttack(nullptr)// �U���N���X
	, m_pEnemySpin(nullptr)// �X�s���N���X
	, m_pEnemyIdling(nullptr)// �p�j�N���X
	, m_canAttack(true)// �U���\��
	, m_enemyState(IState::EnemyState::IDLING)// �G�̏��
	, m_pEnemy(pEnemy)// �G
	, m_isAttack(false)// �U������
{
	// �e��|�C���^�[��������
	// �G�̃|�C���^�[��ݒ�
	m_pEnemy = pEnemy;
	// �U���N���X�𐶐�
	m_pEnemyAttack = std::make_unique<EnemyAttack>(this);
	// �p�j�N���X�𐶐�
	m_pEnemyIdling = std::make_unique<EnemyIdling>(this);
	// �X�s���N���X�𐶐�
	m_pEnemySpin = std::make_unique<EnemySpin>(this);
	// �ǐՃN���X�𐶐�
	m_pEnemyChase = std::make_unique<EnemyChase>(this);
	// �V���h�E�X�e�b�v�N���X�𐶐�
	m_pEnemyShadowStep = std::make_unique<EnemyShadowStep>(this);
}

/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyAI::~EnemyAI()
{
	// �U���N���X�̉��
	m_pEnemyAttack.reset();
	// �p�j�N���X�̉��
	m_pEnemyIdling.reset();
	// �X�s���N���X�̉��
	m_pEnemySpin.reset();
	// ���݂̏�Ԃ�nullptr�ɐݒ�
	m_pCurrentState = nullptr;
	// �G�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemy = nullptr;
}

/*
*	@brief	������
*	@details �GAI�N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemyAI::Initialize()
{
	using namespace DirectX::SimpleMath;
	// �����ʒu��������
	m_initialPosition = Vector3::Zero;
	// �����ʒu��Y���W��ݒ�
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX);
	// Y���̉�]��ݒ�
	m_rotation.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX);
	// ���V�̏������x
	m_velocity = EnemyParameters::INITIAL_VELOCITY;
	// �X�P�[��������
	m_scale = Vector3::One;
	// �ʒu��������
	m_position = m_initialPosition;
	//// �p�j�Ԑ��ɂ���
	//m_pCurrentState = m_pEnemyIdling.get();
	// �ǐՑԐ��ɂ���
	m_pCurrentState = m_pEnemyChase.get();
	// ��Ԃ�������
	m_pCurrentState->Initialize();
	// �\�����p�j��Ԃ̂��̂ɂ���
	m_enemyState = IState::EnemyState::IDLING;
}
/*
*	@brief	�X�V
*	@details �GAI�N���X�̍X�V
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemyAI::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// �v���C���[�̒e�ɓ����������ǂ������擾
	if (m_pEnemy->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;


	//// �G���v���C���[�̈��͈͓��ɓ����Ă���ꍇ
	//if ((m_pEnemy->GetHitToPlayer() || m_isHitPlayerBullet))
	//{
	//	//�U���Ԑ��ɂ���
	//	ChangeState(m_pEnemyAttack.get());
	//	// �U���Ԑ��ɂ���
	//	m_enemyState = IState::EnemyState::ATTACK;
	//	// �U�����ɂ���
	//	SetIsAttack(true);
	//}
	//else// �G���v���C���[�̈��͈͊O�ɂ���ꍇ
	//{
	//	//�p�j�Ԑ��ɂ���
	//	ChangeState(m_pEnemyIdling.get());
	//	// �p�j�Ԑ�
	//	m_enemyState = IState::EnemyState::IDLING;
	//	// �U�����łȂ�
	//	SetIsAttack(false);
	//}
	// �v���C���[�̒e�ɓ��������ꍇ
	if (m_pEnemy->GetEnemyHitByPlayerBullet())
	{
		//�X�s������
		ChangeState(m_pEnemySpin.get());
		// �U����H�������Ԃɂ���
		m_enemyState = IState::EnemyState::HIT;
		// �U�����łȂ�
		SetIsAttack(false);
	}
	// ���݂̏�Ԃ��X�V
	m_pCurrentState->Update(elapsedTime);
	// ���Ԃ̉��Z
	m_time += elapsedTime;
	// �G���ӂ�ӂ핂�V������
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	// �G��Y���W���X�V
	m_position.y += m_velocity.y * elapsedTime;
	// �G�̈ʒu���X�V
	m_pEnemy->SetPosition(m_position);
}
/*
*	@brief	��ԕύX
*	@details �GAI�N���X�̏�ԕύX
*	@param	IState* newState�@�V�������
*	@return	�Ȃ�
*/
void EnemyAI::ChangeState(IState* newState)
{
	// �V������Ԃ����݂̏�ԂƈقȂ�ꍇ
	if (m_pCurrentState != newState)
	{
		// �V������ԂɕύX
		m_pCurrentState = newState;
		// �V������Ԃ�������
		m_pCurrentState->Initialize();
	}
}