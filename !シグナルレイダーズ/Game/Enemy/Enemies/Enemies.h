/*
	@file	Enemies.h
	@brief	�G�����N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Wifi/Wifi.h"
// �O���錾
class CommonResources;
class IEnemy;
class Boss;
class Player;
class Effect;
class Wifi;
class Enemies
{
public:

private:
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �G
	std::vector<std::unique_ptr<IEnemy>> m_enemies;
	// �{�X
	std::unique_ptr<Boss> m_boss;
	// �X�e�[�W�ԍ��i0����4���v���C�V�[������Q�Ɓj
	int m_stageNumber = 0;
	// �G�����t���O
	bool m_isEnemyBorn = false;
	// �G�����ς�
	bool m_isBorned = false;
	// �v���C���[�ƓG�̓����蔻��
	bool m_isHitPlayerToEnemy = false;
	// �G�̐�
	int m_enemyIndex = 0;
	// �G�̐������
	int m_enemyMax = 0;
	// �G�̐����Ԋu
	float m_enemyBornInterval = 0.0f;
	// �G�̐����^�C�~���O���Ǘ�
	float m_enemyBornTimer = 0.0f;
	// �G�̐����J�n����
	float m_startTime = 0.0f;
	// �{�X�����t���O
	bool m_isBossBorn = false;
	// �{�X�����ς�
	bool m_isBossBorned = false;
	// �{�X�����t���O
	bool m_isBossAlive = true;
	// �{�X�̗̑�
	int m_bossHP;
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
	Boss::BossBulletType m_bossBulletType;
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
	std::vector<std::unique_ptr<IEnemy>>& GetEnemies() { return m_enemies; }  // �G���X�g
	int GetEnemyIndex() const { return m_enemyIndex; }// �������ꂽ�G�̍ő吔
	int GetEnemySize() const { return static_cast<int>(m_enemies.size()); }
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }// Wi-Fi
	std::vector<std::unique_ptr<Effect>>& GetEffect() { return m_effect; }// �G�t�F�N�g
	bool GetisBorned() const { return m_isBorned; }// �G�����ς�
	bool GetIsBossAlive() const { return m_isBossAlive; }// �{�X�����t���O
	// Setter
	void SetVolume(float volume) { m_SEVolume = volume; }// ���ʎ擾
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// �X�e�[�W�ԍ�
private:
	// �G�̐�������ݒ�
	void SetEnemyMax();
};
