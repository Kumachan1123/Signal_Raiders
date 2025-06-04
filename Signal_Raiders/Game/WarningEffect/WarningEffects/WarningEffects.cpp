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
	: m_pCommonResources(resources)// ���ʃ��\�[�X
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
	// �v���C���[�̃|�C���^�[���󂯎��
	m_pPlayer = pPlayer;
	// �G�̃|�C���^�[���󂯎��
	m_pEnemyManager = pEnemyManager;
}
/*
*	@brief �_���[�W���󂯂����̉��o�𐶐�
*	@details �_���[�W�G�t�F�N�g�𐶐�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void WarningEffects::CreateDamageEffects()
{
	// �_���[�W�G�t�F�N�g�̐���
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_pCommonResources);
	// �v���C���[�̃|�C���^��ݒ�
	damageEffect->SetPlayer(m_pPlayer);
	// �G�t�F�N�g�^�C�v��ݒ�
	damageEffect->SetEffectType(DamageEffect::EffectType::DAMAGE);
	// ������
	damageEffect->Initialize();
	// �_���[�W�G�t�F�N�g�����X�g�ɒǉ�
	m_pDamageEffect.push_back(std::move(damageEffect));
}
/*
*	@brief �G���U�����Ă������̉��o�𐶐�
*	@details �G���U�����Ă������̉��o�𐶐�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void WarningEffects::CreateInComingEnemy()
{
	// �U�����Ă��悤�Ƃ��Ă���G�̐�����
	for (auto& attackingEnemy : m_pEnemyManager->GetEnemies())
	{
		// �U���t���O�������Ă��Ȃ������玟�̃��[�v��
		if (!attackingEnemy->GetIsAttack())continue;
		// ���łɐ�������Ă����玟�̃��[�v��
		if (m_enemyEffectMap.find(attackingEnemy.get()) != m_enemyEffectMap.end())continue;
		// �_���[�W�G�t�F�N�g�̐���
		std::unique_ptr<DamageEffect> warningEffect = std::make_unique<DamageEffect>(m_pCommonResources);
		// �v���C���[�̃|�C���^��ݒ�
		warningEffect->SetPlayer(m_pPlayer);
		// �G�t�F�N�g�^�C�v��ݒ�
		warningEffect->SetEffectType(DamageEffect::EffectType::INCOMINGENEMY);
		// �U�����Ă����G�̃|�C���^��ݒ�
		warningEffect->SetEnemy(attackingEnemy.get());
		// ������
		warningEffect->Initialize();
		// �U���t���O��false�ɂ���
		attackingEnemy->SetIsAttack(false);
		// �G�̃|�C���^���擾
		IEnemy* enemy = attackingEnemy.get();
		// �U�����Ă����G�����X�g�ɒǉ�
		m_pAttackingEnemy.push_back(enemy);
		// m_pDamageEffect�ɓ��ꂽ���m_enemyEffectMap�ɓo�^
		m_enemyEffectMap[enemy] = warningEffect.get();
		// �܂��G�t�F�N�g�����X�g�ɒǉ�
		m_pDamageEffect.push_back(std::move(warningEffect));
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
	// �V�����_���[�W�G�t�F�N�g
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;
	// �G�t�F�N�g�̐��X�V
	for (auto& damageEffect : m_pDamageEffect)
	{
		// �_���[�W�G�t�F�N�g���X�V
		damageEffect->Update(elapsedTime);
		// �_���[�W�G�t�F�N�g�̔j���t���O��true��������
		if (damageEffect->Destroy())
		{
			// �_���[�W�G�t�F�N�g��j��
			damageEffect.reset();
			// ���̃��[�v��
			continue;
		}
		// �V�������X�g�Ƀ_���[�W�G�t�F�N�g��ǉ�
		newDamageEffect.push_back(std::move(damageEffect));
	}
	// �_���[�W�G�t�F�N�g��V�������X�g�ɒu��������
	m_pDamageEffect = std::move(newDamageEffect);
	// �Đ�����I������x���G�t�F�N�g�ƓG�̃|�C���^�[���폜
	for (auto& attackingEnemy : m_pAttackingEnemy)
	{
		// ��������Ă��Ȃ������玟�̃��[�v��
		if (m_enemyEffectMap.find(attackingEnemy) == m_enemyEffectMap.end())continue;
		// �Đ��t���O��false��������
		if (!m_enemyEffectMap[attackingEnemy]->GetPlayEffect())
		{
			// �}�b�v����폜
			m_enemyEffectMap.erase(attackingEnemy);
			// ���X�g����폜
			m_pAttackingEnemy.erase(std::remove(m_pAttackingEnemy.begin(), m_pAttackingEnemy.end(), attackingEnemy), m_pAttackingEnemy.end());
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
	// �_���[�W�G�t�F�N�g���X�V����
	for (auto& damageEffect : m_pDamageEffect)if (damageEffect->GetPlayEffect())damageEffect->Render();
}