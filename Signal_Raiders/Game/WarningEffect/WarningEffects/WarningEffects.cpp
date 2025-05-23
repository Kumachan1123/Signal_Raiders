/*
*	@file WarningEffects.cpp
*	@brief �_���[�W�G�t�F�N�g��x���G�t�F�N�g�̊Ǘ��N���X�̃\�[�X�t�@�C��
*/
#include <pch.h>
#include "WarningEffects.h"
/*
*	@brief �R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
WarningEffects::WarningEffects(CommonResources* resources)
	: m_commonResources(resources)// ���ʃ��\�[�X
	, m_warningEffectCount{ 0 }// �U�����悤�Ƃ��Ă���G�̐�
	, m_pPlayer{}// �v���C���[
	, m_pEnemyManager{}// �G�}�l�[�W���[
{}

/*
*	@brief �f�X�g���N�^
*	@details �_���[�W�G�t�F�N�g��x���G�t�F�N�g�̊Ǘ��N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
WarningEffects::~WarningEffects() {/*do nothing*/ }

/*
*	@brief ������
*	@details �_���[�W�G�t�F�N�g��x���G�t�F�N�g�̊Ǘ��N���X�̏�����
*	@param pPlayer �v���C���[�̃|�C���^
*	@param pEnemyManager �G�̃|�C���^
*	@return �Ȃ�
*/
void WarningEffects::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���󂯎��
	m_pEnemyManager = pEnemyManager;// �G�̃|�C���^�[���󂯎��
}

/*
*	@brief �_���[�W���󂯂����̉��o�𐶐�
*	@details �_���[�W�G�t�F�N�g�𐶐�����
*	@param �Ȃ�
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
*	@details �G���U�����Ă������̉��o�𐶐�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void WarningEffects::CreateInComingEnemy()
{
	for (auto& attackingEnemy : m_pEnemyManager->GetEnemies())// �U�����Ă��悤�Ƃ��Ă���G�̐�����
	{
		if (!attackingEnemy->GetIsAttack())continue;// �U���t���O�������Ă��Ȃ������玟�̃��[�v��
		if (m_enemyEffectMap.find(attackingEnemy.get()) != m_enemyEffectMap.end())continue;// ���łɐ�������Ă����玟�̃��[�v��
		std::unique_ptr<DamageEffect> warningEffect = std::make_unique<DamageEffect>(m_commonResources);// �_���[�W�G�t�F�N�g�̐���
		warningEffect->SetPlayer(m_pPlayer);// �v���C���[�̃|�C���^��ݒ�
		warningEffect->SetEffectType(DamageEffect::EffectType::INCOMINGENEMY);// �G�t�F�N�g�^�C�v��ݒ�
		warningEffect->SetEnemy(attackingEnemy.get());// �U�����Ă����G�̃|�C���^��ݒ�
		warningEffect->Initialize();// ������
		attackingEnemy->SetIsAttack(false);// �U���t���O��false�ɂ���
		IEnemy* enemy = attackingEnemy.get();// �G�̃|�C���^���擾
		m_pAttackingEnemy.push_back(enemy); // �U�����Ă����G�����X�g�ɒǉ�
		m_enemyEffectMap[enemy] = warningEffect.get();// m_pDamageEffect�ɓ��ꂽ���m_enemyEffectMap�ɓo�^
		m_pDamageEffect.push_back(std::move(warningEffect));// �܂��G�t�F�N�g�����X�g�ɒǉ�
	}
}

/*
*	@brief �X�V
*	@details �_���[�W�G�t�F�N�g��x���G�t�F�N�g�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void WarningEffects::Update(float elapsedTime)
{
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;// �V�����_���[�W�G�t�F�N�g
	for (auto& damageEffect : m_pDamageEffect)// �G�t�F�N�g�̐��X�V
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
	for (auto& attackingEnemy : m_pAttackingEnemy)	// �Đ�����I������x���G�t�F�N�g�ƓG�̃|�C���^�[���폜
	{
		if (m_enemyEffectMap.find(attackingEnemy) == m_enemyEffectMap.end())continue;// ��������Ă��Ȃ������玟�̃��[�v��
		if (!m_enemyEffectMap[attackingEnemy]->GetPlayEffect())// �Đ��t���O��false��������
		{
			m_enemyEffectMap.erase(attackingEnemy);// �}�b�v����폜
			m_pAttackingEnemy.erase(std::remove(m_pAttackingEnemy.begin(), m_pAttackingEnemy.end(), attackingEnemy), m_pAttackingEnemy.end());// ���X�g����폜
		}
	}
}
/*
*	@brief �`��
*	@details �_���[�W�G�t�F�N�g��x���G�t�F�N�g�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void WarningEffects::Render()
{
	for (auto& damageEffect : m_pDamageEffect)if (damageEffect->GetPlayEffect())damageEffect->Render();	// �_���[�W�G�t�F�N�g���X�V����
}
