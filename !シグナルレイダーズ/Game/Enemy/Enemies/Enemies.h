/*
	@file	Enemies.h
	@brief	�G�����N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Wifi/Wifi.h"
// �O���錾
class CommonResources;
class Enemy;
class PlayerController;
class Effect;
class Wifi;

class Enemies
{
private:
	// �R�������\�[�X
	CommonResources* m_commonResources;

	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// �G�����t���O
	bool m_isEnemyBorn = false;
	// �G�����ς�
	bool m_isBorned = false;
	// �v���C���[�ƓG�̓����蔻��
	bool m_isHitPlayerToEnemy = false;
	// �G�̐�
	int m_enemyIndex = 0;
	// �G�̐����Ԋu
	float m_enemyBornInterval = 0.0f;
	// �G�̐����^�C�~���O���Ǘ�
	float m_enemyBornTimer = 0.0f;
	// �G�̐����J�n����
	float m_startTime = 0.0f;
	// Wifi
	Wifi* m_wifi;
	// �v���C���[�R���g���[���[
	PlayerController* m_playerController;
	// �G�t�F�N�g
	Effect* m_pEffect;


public:
	Enemies(CommonResources* commonResources);
	~Enemies();
	// �X�V
	void Update(Effect* pEffect, Wifi* pWifi, PlayerController* pPlayerController, float elapsedTime);
	// �`��
	void Draw();

};
