/*
	@file	Enemies.h
	@brief	�G�����N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Wifi/Wifi.h"
// �O���錾
class CommonResources;
class Enemy;
class Player;
class Effect;
class Wifi;
class Enemies
{
private:
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �G
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
	// wi-fi
	std::unique_ptr<Wifi> m_pWifi;
	// �v���C���[�R���g���[���[
	Player* m_pPlayer;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_effect;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// SE�̉���
	float m_SEVolume = 0.0f;
public:
	Enemies(CommonResources* commonResources);
	~Enemies();
	// ������
	void Initialize(Player* pPlayer);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// ���ʉ��̏�����
	void InitializeFMOD();

public:
	// Getter
	std::vector<std::unique_ptr<Enemy>>& GetEnemy() { return m_enemy; }// �G
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }// Wi-Fi
	std::vector<std::unique_ptr<Effect>>& GetEffect() { return m_effect; }// �G�t�F�N�g
	bool GetisBorned() const { return m_isBorned; }// �G�����ς�
	// Setter
	void SetVolume(float volume) { m_SEVolume = volume; }// ���ʎ擾
};
