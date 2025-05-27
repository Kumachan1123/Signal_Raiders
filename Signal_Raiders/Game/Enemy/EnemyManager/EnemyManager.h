/*
	@file	EnemyManager.h
	@brief	�G�}�l�[�W���[�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <Effects.h>
#include <SimpleMath.h>
#include <memory>
#include <Model.h>
#include <vector>
// �O�����C�u����
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ����w�b�_�[�t�@�C��
#include "Game/BulletManager/BulletManager.h"
#include "Game/Effect/Effect.h"
#include "Game/Enemy/EnemyFactory/EnemyFactory.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IEnemy.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Wifi/Wifi.h"
// �O���錾
class CommonResources;
class IEnemy;
class BossBase;
class Player;
class Effect;
class Wifi;
class Wall;
class EnemyManager
{
private:
	// �\����
	struct StageSettings// �X�e�[�W���Ƃ̐ݒ�
	{
		int enemyMax;// �G�̐������
		int bossHP;// �{�X�̗̑�
		int specialAttackCount;// ��x�ɏo�����U���̐�
		BossBase::BossBulletType bulletType;// �{�X�̒e�̎��
	};
	const std::unordered_map<int, StageSettings> stageData =// �X�e�[�W���Ƃ̐ݒ�̃}�b�v
	{
		//{�X�e�[�W�ԍ�, {�G�̐������, �{�X�̗̑�, ��x�ɏo�����U���̐�, �{�X�̒e�̎��}}
		{0, {5, 100, 3,BossBase::BossBulletType::STAGE_1}},// �X�e�[�W1
		{1, {10, 200,4, BossBase::BossBulletType::STAGE_2}},// �X�e�[�W2
		{2, {20, 300, 5,BossBase::BossBulletType::STAGE_3}},// �X�e�[�W3
		{3, {30, 500, 7,BossBase::BossBulletType::STAGE_4}},// �X�e�[�W4
		{4, {40, 1000,10,BossBase::BossBulletType::STAGE_5}}// �X�e�[�W5

	};

public:
	// �A�N�Z�T
	std::vector<std::unique_ptr<IEnemy>>& GetEnemies() { return m_enemies; }  // �G���X�g�擾
	std::vector<std::unique_ptr<IEnemy>>& GetAttackingEnemies() { return m_attackingEnemies; }  // �U�����̓G���X�g�擾
	int GetEnemyIndex() const { return m_enemyIndex; }// �������ꂽ�G�̍ő吔�擾
	int GetEnemySize() const { return static_cast<int>(m_enemies.size()); }// �G�̐��擾
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }// Wi-Fi�|�C���^�[�擾
	std::vector<std::unique_ptr<Effect>>& GetEffect() { return m_effect; }// �G�t�F�N�g�擾
	bool GetisBorned() const { return m_isBorned; }// �G�����ς݃t���O
	bool GetIsBossAlive() const { return m_isBossAlive; }// �{�X�����t���O
	bool GetIsBossAppear() const { return m_isBossAppear; }// �{�X�������o�Đ��t���O
	float GetSEVolume() const { return m_SEVolume; }// SE�̉��ʎ擾
	void SetSEVolume(float volume) { m_SEVolume = volume; }// SE�̉��ʐݒ�
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// �X�e�[�W�ԍ��ݒ�
	void SetWall(Wall* pWall) { m_pWall = pWall; };// �ǂ̃|�C���^�[�ݒ�
	BulletManager* GetBulletManager() { return m_pBulletManager; }// �e�}�l�[�W���[�擾
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }// �e�}�l�[�W���[�ݒ�
	int GetSpecialAttackCount() const { return m_specialAttackCount; }// ����U���̐��擾
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }// ����U���̐��ݒ�
public:
	// public�֐�
	EnemyManager(CommonResources* commonResources);// �R���X�g���N�^
	~EnemyManager();// �f�X�g���N�^
	void Initialize(Player* pPlayer);	// ������
	void Update(float elapsedTime);	// �X�V
	void Render();	// �`��
private:
	// private�֐�
	void SetEnemyMax();	// �G�̐�������ݒ�
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
private:
	// private�ϐ�
	// �R�������\�[�X
	CommonResources* m_pCommonResources;
	// �G
	std::vector<std::unique_ptr<IEnemy>> m_enemies;
	// �U�����̓G�z��
	std::vector<std::unique_ptr<IEnemy>> m_attackingEnemies;
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
	// �{�X�̐����ҋ@����
	float m_bossBornWaitTime;
	// �{�X�������o�Đ��t���O
	bool m_isBossAppear;
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
	// SE�̉���
	float m_SEVolume;
	// �{�X�̒e�̎��
	BossBase::BossBulletType m_bossBulletType;


};
