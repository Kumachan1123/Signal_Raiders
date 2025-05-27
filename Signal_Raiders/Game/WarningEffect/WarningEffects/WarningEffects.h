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
public:
	// �A�N�Z�T
	int GetWarningEffectCount()const { return m_warningEffectCount; }// �U�����悤�Ƃ��Ă���G�̐��擾
	void SetWarningEffectCount(int count) { m_warningEffectCount = count; }// �U�����悤�Ƃ��Ă���G�̐��ݒ�
public:
	// public�����o�֐�
	WarningEffects(CommonResources* resources);// �R���X�g���N�^
	~WarningEffects();// �f�X�g���N�^
	void CreateDamageEffects();	// �_���[�W���󂯂����̉��o�𐶐�
	void CreateInComingEnemy();	// �G���U�����Ă������̉��o�𐶐�
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);// ������
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
private:
	CommonResources* m_pCommonResources;// ���ʃ��\�[�X
	Player* m_pPlayer;// �v���C���[
	EnemyManager* m_pEnemyManager;// �G
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;	// �_���[�W�G�t�F�N�g
	std::vector<IEnemy*> m_pAttackingEnemy;// �U�����Ă����G���X�g
	int m_warningEffectCount;// �U�����悤�Ƃ��Ă���G�̐�
	std::unordered_map<IEnemy*, DamageEffect*> m_enemyEffectMap;// �G�ƃG�t�F�N�g�̃}�b�v
};