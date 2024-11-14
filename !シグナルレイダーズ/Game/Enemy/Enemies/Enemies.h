/*
	@file	Enemies.h
	@brief	�G�����N���X
*/
#pragma once
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/AreaAttacker/AreaAttacker.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Stage/Wall/Wall.h"
// �O���錾
class CommonResources;
class IEnemy;
class Boss;
class Player;
class Effect;
class Wifi;
class Wall;
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
	int m_stageNumber;
	// �G�����t���O
	bool m_isEnemyBorn;
	// �G�����ς�
	bool m_isBorned;
	// �v���C���[�ƓG�̓����蔻��
	bool m_isHitPlayerToEnemy;
	// �G�̐�
	int m_enemyIndex;
	// �G�̐������
	int m_enemyMax;
	// �G�̐����Ԋu
	float m_enemyBornInterval;
	// �G�̐����^�C�~���O���Ǘ�
	float m_enemyBornTimer;
	// �G�̐����J�n����
	float m_startTime;
	// �{�X�����t���O
	bool m_isBossBorn;
	// �{�X�����ς�
	bool m_isBossBorned;
	// �{�X�����t���O
	bool m_isBossAlive;
	// �{�X�̗̑�
	int m_bossHP;
	// wi-fi
	std::unique_ptr<Wifi> m_pWifi;
	// �v���C���[(�R���g���[���[)
	Player* m_pPlayer;
	// ��
	Wall* m_pWall;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_effect;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// SE�̉���
	float m_SEVolume = 0.0f;
	// �{�X�̒e�̎��
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
	void SetWall(Wall* pWall) { m_pWall = pWall; };// �ǂ̃|�C���^�[
private:
	// �G�̐�������ݒ�
	void SetEnemyMax();

	// �X�V�����𕪊����郁�\�b�h
	void UpdateStartTime(float elapsedTime);// �G�����J�n����
	void UpdateEffects(float elapsedTime);// �G�t�F�N�g
	void HandleEnemySpawning(float elapsedTime);// �G����
	void SpawnEnemy();// �G����(�ʏ�^�C�v)
	void SpawnAreaAttacker();// �G����(�͈͍U���^�C�v)
	void FinalizeEnemySpawn();// �G�����I��
	void SpawnBoss();// �{�X����
	void HandleEnemyCollisions();// �G�̓����蔻��
	void HandlePlayerCollisions(float elapsedTime);// �v���C���[�ƓG�̓����蔻��
	void HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& enemy);// �G�̒e�ƃv���C���[�̓����蔻��
	void HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy);// �G�ƃv���C���[�̓����蔻��
	void HandleWallCollision();// �ǂƂ̓����蔻��
	void RemoveDeadEnemies();// ���S�����G���폜
	void HandleEnemyDeath(std::unique_ptr<IEnemy>& enemy);// �G�̎��S����
};
