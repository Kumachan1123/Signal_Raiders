/*
	@file	EnemyFactory.cpp
	@brief	�G�t�@�N�g���[�N���X
*/
#include "pch.h"
#include "EnemyFactory.h"
/*
*	@brief	�G����
*	@param[in]	EnemyType type	�G�̎��
*	@param[in]	Player* player	�v���C���[�̃|�C���^
*	@param[in]	CommonResources* resources	���\�[�X�̃|�C���^
*	@param[in]	int HP	�G��HP
*	@return	�G�̃|�C���^
*/
std::unique_ptr<IEnemy> EnemyFactory::CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP)
{
	switch (type)// �G�̎�ނɂ���Đ�������G��ς���
	{
	case EnemyType::NORMAL:// �ʏ�G
		return std::make_unique<Enemy>(player, resources, HP);
	case EnemyType::VERTICAL_ATTACKER:// �����e�����G
		return std::make_unique<VerticalAttacker>(player, resources, HP);
	case EnemyType::BOSS:// �{�X
		return std::make_unique<BossBase>(player, resources, HP);
	default:// ����ȊO�̓G�͐������Ȃ�
		return nullptr;
	}
}

/*
*	@brief	�{�X����
*	@param[in]	BossType type	�{�X�̎��
*	@param[in]	BossBase* pBoss	�{�X�̃|�C���^
*	@param[in]	CommonResources* resources	���\�[�X�̃|�C���^
*	@return	�{�X�̃|�C���^
*/
std::unique_ptr<IBossLogic> EnemyFactory::CreateBoss(BossType type, BossBase* pBoss, CommonResources* resources)
{
	switch (type)// �{�X�̎�ނɂ���Đ�������{�X��ς���
	{
	case BossType::NORMAL_BOSS:// �ʏ�{�X
		return std::make_unique<Boss>(pBoss, resources);
	case BossType::LAST_BOSS:// �ŏI�{�X
		return std::make_unique<LastBoss>(pBoss, resources);
	default:// ����ȊO�̃{�X�͐������Ȃ�
		return nullptr;
	}
}
