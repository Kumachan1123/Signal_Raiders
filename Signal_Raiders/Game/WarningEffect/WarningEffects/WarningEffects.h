/*
*	@file DamageEffects.h
*	@brief �_���[�W�G�t�F�N�g�̊Ǘ��N���X�̃w�b�_�[�t�@�C��
*/
#pragma once
// �W�����C�u����
#include <vector>
// �O�����C�u����
#include "Game/CommonResources.h"
// ����w�b�_�[�t�@�C��
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/WarningEffect/DamageEffect/DamageEffect.h"

// �N���X�̑O���錾
class Player;
class EnemyManager;
class CommonResources;
class DamageEffect;


class WarningEffects
{
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
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	Player* m_pPlayer;// �v���C���[
	EnemyManager* m_pEnemyManager;// �G
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;	// �_���[�W�G�t�F�N�g
};