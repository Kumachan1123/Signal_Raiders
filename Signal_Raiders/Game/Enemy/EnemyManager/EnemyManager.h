/*
*	@file	EnemyManager.h
*	@brief	�G�}�l�[�W���[�N���X
*/
#pragma once
#ifndef ENEMY_MANAGER_DEFINED
#define ENEMY_MANAGER_DEFINED
// �W�����C�u����
#include <cassert>
#include <memory>
#include <vector>
#include <thread>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
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

// �G�}�l�[�W���[�N���X
class EnemyManager
{
private:
	// �\����
	// �X�e�[�W���Ƃ̐ݒ�
	struct StageSettings
	{
		int enemyMax;						// �G�̐������
		int bossHP;							// �{�X�̗̑�
		int specialAttackCount;				// ��x�ɏo�����U���̐�
		BossBase::BossBulletType bulletType;// �{�X�̒e�̎��
	};
	// �X�e�[�W���Ƃ̐ݒ�̃}�b�v
	const std::unordered_map<int, StageSettings> stageData =
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
	// �G���X�g�擾
	std::vector<std::unique_ptr<IEnemy>>& GetEnemies() { return m_pEnemies; }
	// �U�����̓G���X�g�擾
	std::vector<std::unique_ptr<IEnemy>>& GetAttackingEnemies() { return m_pAttackingEnemies; }
	// �������ꂽ�G�̍ő吔�擾
	int GetEnemyIndex() const { return m_enemyIndex; }
	// �G�̐��擾
	int GetEnemySize() const { return static_cast<int>(m_pEnemies.size()); }
	// Wi-Fi�|�C���^�[�擾
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }
	// �G�t�F�N�g�擾
	std::vector<std::unique_ptr<Effect>>& GetEffects() { return m_pEffects; }
	// �G�����ς݃t���O
	bool GetisBorned() const { return m_isBorned; }
	// �{�X�����t���O
	bool GetIsBossAlive() const { return m_isBossAlive; }
	// �{�X�������o�Đ��t���O
	bool GetIsBossAppear() const { return m_isBossAppear; }
	// SE�̉��ʎ擾
	float GetSEVolume() const { return m_SEVolume; }
	// SE�̉��ʐݒ�
	void SetSEVolume(float volume) { m_SEVolume = volume; }
	// �X�e�[�W�ԍ��ݒ�
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }
	// �ǂ̃|�C���^�[�ݒ�
	void SetWall(Wall* pWall) { m_pWall = pWall; };
	// �e�}�l�[�W���[�擾
	BulletManager* GetBulletManager() { return m_pBulletManager; }
	// �e�}�l�[�W���[�ݒ�
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }
	// ����U���̐��擾
	int GetSpecialAttackCount() const { return m_specialAttackCount; }
	// ����U���̐��ݒ�
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }
public:
	// public�֐�
	// �R���X�g���N�^
	EnemyManager(CommonResources* commonResources);
	// �f�X�g���N�^
	~EnemyManager();
	// ������
	void Initialize(Player* pPlayer);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
private:
	// private�֐�
	// �G�̐�������ݒ�
	void SetEnemyMax();
	// �X�V�����𕪊����郁�\�b�h
	// �G�����J�n���Ԃ̍X�V
	void UpdateStartTime(float elapsedTime);
	// �G�t�F�N�g�̍X�V
	void UpdateEffects(float elapsedTime);
	// �G�̐���
	void HandleEnemySpawning(float elapsedTime);
	// �^�C�v���w�肵�ēG�𐶐�
	void SpawnEnemy(EnemyType type);
	// �G�����I������
	void FinalizeEnemySpawn();
	// �{�X����
	void SpawnBoss();
	// �G�̓����蔻��
	void HandleEnemyCollisions();
	// �v���C���[�ƓG�̓����蔻��
	void UpdateEnemies(float elapsedTime);
	// �G�̒e�ƃv���C���[�̓����蔻��
	void HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& pEnemy);
	// �G�ƃv���C���[�̓����蔻��
	void HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& pEnemy);
	// �ǂƂ̓����蔻��
	void HandleWallCollision();
	// ���S�����G���폜
	void RemoveDeadEnemies();
	// �G�̎��S����
	void HandleEnemyDeath(std::unique_ptr<IEnemy>& pEnemy);
private:
	// private�ϐ�
	// �R�������\�[�X
	CommonResources* m_pCommonResources;
	// �G
	std::vector<std::unique_ptr<IEnemy>> m_pEnemies;
	// �U�����̓G�z��
	std::vector<std::unique_ptr<IEnemy>> m_pAttackingEnemies;
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
	// Wi-Fi���擾���邽�߂̃X���b�h
	std::thread m_wifiThread;
	// �v���C���[(�R���g���[���[)
	Player* m_pPlayer;
	// �e�}�l�[�W���[�i�v���C�V�[������󂯎��j
	BulletManager* m_pBulletManager;
	// ��
	Wall* m_pWall;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_pEffects;
	// SE�̉���
	float m_SEVolume;
	// �{�X�̒e�̎��
	BossBase::BossBulletType m_bossBulletType;
};
#endif // ENEMY_MANAGER_DEFINED
