/*
	@file	EnemyManager.h
	@brief	�G�����N���X
*/
#pragma once
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Enemy/EnemyFactory/EnemyFactory.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
// �O���錾
class CommonResources;
class IEnemy;
class Boss;
class Player;
class Effect;
class Wifi;
class Wall;
class EnemyManager
{
private:
	// �X�e�[�W���Ƃ̐ݒ�
	struct StageSettings
	{
		int enemyMax;// �G�̐������
		int bossHP;// �{�X�̗̑�
		int specialAttackCount;// ��x�ɏo�����U���̐�
		Boss::BossBulletType bulletType;// �{�X�̒e�̎��
	};
	const std::unordered_map<int, StageSettings> stageData =
	{
		//{�X�e�[�W�ԍ�, {�G�̐������, �{�X�̗̑�, ��x�ɏo�����U���̐�, �{�X�̒e�̎��}}
		{0, {5, 100, 3,Boss::BossBulletType::NORMAL}},
		{1, {10, 200,4, Boss::BossBulletType::NORMAL}},
		{2, {20, 300,5, Boss::BossBulletType::TWIN}},
		{3, {30, 500, 7,Boss::BossBulletType::THREE}},
		{4, {40, 1000,10, Boss::BossBulletType::THREE}}
	};

private:
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �G
	std::vector<std::unique_ptr<IEnemy>> m_enemies;
	// �U�����̓G�z��
	std::vector<std::unique_ptr<IEnemy>> m_attackingEnemies;
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
	// �{�X����x�ɏo������U���̐�
	int m_specialAttackCount;
	// wi-fi
	std::unique_ptr<Wifi> m_pWifi;
	// �v���C���[(�R���g���[���[)
	Player* m_pPlayer;
	// �e�}�l�[�W���[�i�v���C�V�[������󂯎��j
	BulletManager* m_pBulletManager;
	// ��
	Wall* m_pWall;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_effect;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// SE�̉���
	float m_SEVolume;
	// �{�X�̒e�̎��
	Boss::BossBulletType m_bossBulletType;

public:
	EnemyManager(CommonResources* commonResources);
	~EnemyManager();
	// ������
	void Initialize(Player* pPlayer);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// ���ʉ��̏�����
	void InitializeFMOD();

public:
	std::vector<std::unique_ptr<IEnemy>>& GetEnemies() { return m_enemies; }  // �G���X�g
	std::vector<std::unique_ptr<IEnemy>>& GetAttackingEnemies() { return m_attackingEnemies; }  // �U�����̓G���X�g
	int GetEnemyIndex() const { return m_enemyIndex; }// �������ꂽ�G�̍ő吔
	int GetEnemySize() const { return static_cast<int>(m_enemies.size()); }
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }// Wi-Fi
	std::vector<std::unique_ptr<Effect>>& GetEffect() { return m_effect; }// �G�t�F�N�g
	bool GetisBorned() const { return m_isBorned; }// �G�����ς�
	bool GetIsBossAlive() const { return m_isBossAlive; }// �{�X�����t���O
	void SetVolume(float volume) { m_SEVolume = volume; }// ���ʎ擾
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// �X�e�[�W�ԍ�
	void SetWall(Wall* pWall) { m_pWall = pWall; };// �ǂ̃|�C���^�[
	BulletManager* GetBulletManager() { return m_pBulletManager; }// �e�}�l�[�W���[
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }// �e�}�l�[�W���[
	int GetSpecialAttackCount() const { return m_specialAttackCount; }// ����U���̐��擾
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }// ����U���̐��ݒ�
private:
	// �G�̐�������ݒ�
	void SetEnemyMax();

	// �X�V�����𕪊����郁�\�b�h
	void UpdateStartTime(float elapsedTime);// �G�����J�n����
	void UpdateEffects(float elapsedTime);// �G�t�F�N�g
	void HandleEnemySpawning(float elapsedTime);// �G����
	void SpawnEnemy(EnemyType type);// �G����(�w��^�C�v
	void FinalizeEnemySpawn();// �G�����I��
	void SpawnBoss();// �{�X����
	void HandleEnemyCollisions();// �G�̓����蔻��
	void UpdateEnemies(float elapsedTime);// �v���C���[�ƓG�̓����蔻��
	void HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& enemy);// �G�̒e�ƃv���C���[�̓����蔻��
	void HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy);// �G�ƃv���C���[�̓����蔻��
	void HandleWallCollision();// �ǂƂ̓����蔻��
	void RemoveDeadEnemies();// ���S�����G���폜
	void HandleEnemyDeath(std::unique_ptr<IEnemy>& enemy);// �G�̎��S����
};
