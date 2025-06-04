/*
*	@file WarningEffects.h
*	@brief �_���[�W�G�t�F�N�g��x���G�t�F�N�g�̊Ǘ��N���X�̃w�b�_�[�t�@�C��
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <unordered_map>
// �O�����C�u����
#include "Game/CommonResources.h"
// ����w�b�_�[�t�@�C��
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/WarningEffect/DamageEffect/DamageEffect.h"
// �O���錾
class Player;
class EnemyManager;
class CommonResources;
class DamageEffect;
class IEnemy;
class WarningEffects
{
public:	// �A�N�Z�T
	// �U�����悤�Ƃ��Ă���G�̐��擾
	int GetWarningEffectCount()const { return m_warningEffectCount; }
	// �U�����悤�Ƃ��Ă���G�̐��ݒ�
	void SetWarningEffectCount(int count) { m_warningEffectCount = count; }
public:	// public�����o�֐�
	// �R���X�g���N�^
	WarningEffects(CommonResources* resources);
	// �f�X�g���N�^
	~WarningEffects();
	// �_���[�W���󂯂����̉��o�𐶐�
	void CreateDamageEffects();
	// �G���U�����Ă������̉��o�𐶐�
	void CreateInComingEnemy();
	// ������
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
private:// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �v���C���[
	Player* m_pPlayer;
	// �G
	EnemyManager* m_pEnemyManager;
	// �_���[�W�G�t�F�N�g
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;
	// �U�����Ă����G���X�g
	std::vector<IEnemy*> m_pAttackingEnemy;
	// �U�����悤�Ƃ��Ă���G�̐�
	int m_warningEffectCount;
	// �G�ƃG�t�F�N�g�̃}�b�v
	std::unordered_map<IEnemy*, DamageEffect*> m_enemyEffectMap;
};