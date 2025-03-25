/*
*	@file WarningEffects.cpp
*	@brief �_���[�W�G�t�F�N�g�̊Ǘ��N���X�̃\�[�X�t�@�C��
*/
#include <pch.h>
#include "WarningEffects.h"

/*
*	@brief �R���X�g���N�^
*	@param[in] resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
WarningEffects::WarningEffects(CommonResources* resources)
	: m_commonResources(resources)
	, m_pPlayer{}
	, m_pEnemyManager{}
{}

/*
*	@brief �f�X�g���N�^
*	@return �Ȃ�
*/
WarningEffects::~WarningEffects() {}

/*
*	@brief ������
*	@param[in] pPlayer �v���C���[�̃|�C���^
*	@param[in] pEnemyManager �G�̃|�C���^
*	@return �Ȃ�
*/
void WarningEffects::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���󂯎��
	m_pEnemyManager = pEnemyManager;// �G�̃|�C���^�[���󂯎��
}

/*
*	@brief �_���[�W���󂯂����̉��o�𐶐�
*	@return �Ȃ�
*/
void WarningEffects::CreateDamageEffects()
{
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_commonResources);// �_���[�W�G�t�F�N�g�̐���
	damageEffect->SetPlayer(m_pPlayer);// �v���C���[�̃|�C���^��ݒ�
	damageEffect->SetEffectType(DamageEffect::EffectType::DAMAGE);// �G�t�F�N�g�^�C�v��ݒ�
	damageEffect->Initialize();// ������
	m_pDamageEffect.push_back(std::move(damageEffect));// �_���[�W�G�t�F�N�g�����X�g�ɒǉ�
}

/*
*	@brief �G���U�����Ă������̉��o�𐶐�
*	@return �Ȃ�
*/
void WarningEffects::CreateInComingEnemy()
{
	for (auto& attackingEnemy : m_pEnemyManager->GetEnemies())
	{
		if (!attackingEnemy->GetIsAttack())continue;// �U���t���O�������Ă��Ȃ������玟�̃��[�v��
		std::unique_ptr<DamageEffect> warningEffect = std::make_unique<DamageEffect>(m_commonResources);// �_���[�W�G�t�F�N�g�̐���
		warningEffect->SetPlayer(m_pPlayer);// �v���C���[�̃|�C���^��ݒ�
		warningEffect->SetEffectType(DamageEffect::EffectType::INCOMINGENEMY);// �G�t�F�N�g�^�C�v��ݒ�
		warningEffect->SetEnemy(attackingEnemy.get());// �U�����Ă����G�̃|�C���^��ݒ�
		warningEffect->Initialize();// ������
		m_pDamageEffect.push_back(std::move(warningEffect));// �_���[�W�G�t�F�N�g�����X�g�ɒǉ�

	}

}

/*
*	@brief �X�V
*	@param[in] elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void WarningEffects::Update(float elapsedTime)
{
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;// �V�����_���[�W�G�t�F�N�g
	for (auto& damageEffect : m_pDamageEffect)
	{
		damageEffect->Update(elapsedTime);// �_���[�W�G�t�F�N�g���X�V
		// �Đ����I������_���[�W�G�t�F�N�g�������炻�̃G�t�F�N�g�����S�ɔj��
		if (damageEffect->Destroy())// �_���[�W�G�t�F�N�g�̔j���t���O��true��������
		{
			damageEffect.reset();// �_���[�W�G�t�F�N�g��j��
			continue;// ���̃��[�v��
		}
		newDamageEffect.push_back(std::move(damageEffect));// �V�������X�g�Ƀ_���[�W�G�t�F�N�g��ǉ�
	}
	m_pDamageEffect = std::move(newDamageEffect);// �_���[�W�G�t�F�N�g��V�������X�g�ɒu��������
}
/*
*	@brief �`��
*	@return �Ȃ�
*/
void WarningEffects::Render()
{
	for (auto& damageEffect : m_pDamageEffect)if (damageEffect->GetPlayEffect())damageEffect->Render();	// �_���[�W�G�t�F�N�g���X�V����
}
