/*
*	@file DamageEffects.cpp
*	@brief �_���[�W�G�t�F�N�g�̊Ǘ��N���X�̃\�[�X�t�@�C��
*/
#include <pch.h>
#include "DamageEffects.h"

/*
*	@brief �R���X�g���N�^
*	@param[in] resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
DamageEffects::DamageEffects(CommonResources* resources)
	: m_commonResources(resources)
	, m_pPlayer{}
{}

/*
*	@brief �f�X�g���N�^
*	@return �Ȃ�
*/
DamageEffects::~DamageEffects() {}

/*
*	@brief ������
*	@param[in] pPlayer �v���C���[�̃|�C���^
*	@param[in] pEnemyManager �G�̃|�C���^
*	@return �Ȃ�
*/
void DamageEffects::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[���󂯎��
	m_pEnemyManager = pEnemyManager;// �G�̃|�C���^�[���󂯎��
}

/*
*	@brief �_���[�W���󂯂����̉��o�𐶐�
*	@return �Ȃ�
*/
void DamageEffects::Create()
{
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_commonResources);// �_���[�W�G�t�F�N�g�̐���
	damageEffect->Initialize(m_pPlayer);// ������
	m_pDamageEffect.push_back(std::move(damageEffect));// �_���[�W�G�t�F�N�g�����X�g�ɒǉ�
}

/*
*	@brief �X�V
*	@param[in] elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void DamageEffects::Update(float elapsedTime)
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
void DamageEffects::Render()
{
	for (auto& damageEffect : m_pDamageEffect)if (damageEffect->GetPlayEffect())damageEffect->Render();	// �_���[�W�G�t�F�N�g���X�V����
}
