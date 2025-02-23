/*
	@file	BulletManager.h
	@brief	�e�Ǘ��N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <SimpleMath.h>
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

// ����w�b�_�[�t�@�C��
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"


// �N���X�̑O���錾
class CommonResources;
class Player;
class IEnemy;
class EnemyManager;
class BulletParameters;
class BulletManager
{

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	BulletManager(CommonResources* commonResources);// �R���X�g���N�^
	~BulletManager();// �f�X�g���N�^

	// public�����o�֐�
	void Initialize(Player* pPlayer, EnemyManager* pEnemies);// ������
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
	void CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);// �v���C���[�̒e����
	void CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);// �G�̒e����
	void RemoveBulletsByShooter(IEnemy* shooter);// �e�̍폜�i�G�����񂾂Ƃ��ɌĂ΂��j
	void ReLoadPlayerBullet();// �e�̕�[
	void ConsumePlayerBullet();// �e�̏���

	// �Q�b�^�[�E�Z�b�^�[
	void SetEnemyBulletType(EnemyBullet::BulletType type) { m_enemyBulletType = type; }// �G�̒e�̎�ސݒ�
	void SetEnemyBulletSize(float size) { m_enemyBulletSize = size; }// �G�̒e�̑傫���ݒ�
	bool GetIsPlayerShoot() const { return m_isPlayerShoot; }// �v���C���[�̒e�����t���O�擾
	void SetIsPlayerShoot(bool isPlayerShoot) { m_isPlayerShoot = isPlayerShoot; }// �v���C���[�̒e�����t���O�ݒ�
	void SetShooter(IEnemy* pShooter) { m_pShooter = pShooter; }// �e���ˏo�����G�̃|�C���^�[�ݒ�
	void SetAdditionalDamage(int additionalDamage);// �ǉ��_���[�W�ݒ�
	int GetPlayerBulletCount() const { return m_playerBulletCount; }// �v���C���[�̒e�̐��擾
	void SetPlayerBulletCount(int count) { m_playerBulletCount = count; }// �v���C���[�̒e�̐��ݒ�
	int GetSpecialAttackCount() const { return m_specialAttackCount; }// ����U���̐��擾
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }// ����U���̐��ݒ�
	bool GetIsReloading() const { return m_isReloading; }// �����[�h���t���O�擾
	void SetIsReloading(bool isReloading) { m_isReloading = isReloading; }// �����[�h���t���O�ݒ�
private:
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// �v���C���[
	Player* m_pPlayer;
	// �G�S��
	EnemyManager* m_pEnemyManager;
	// �e���ˏo�����G�̃|�C���^�[
	IEnemy* m_pShooter;
	// �G�̒e�̎��
	EnemyBullet::BulletType m_enemyBulletType;
	// �G�̒e�̑傫��
	float m_enemyBulletSize;
	// �v���C���[�̒e�����t���O
	bool m_isPlayerShoot;
	// �o�ߎ���
	float m_elapsedTime;
	// �����[�h�^�C�}�[
	float m_reloadTimer;
	// �����[�h���t���O
	bool m_isReloading;
	// �v���C���[�̒e
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	// �G�̒e
	std::vector<std::unique_ptr<EnemyBullet>> m_enemyBullets;
	// �v���C���[�̒e�̍ő吔
	int m_playerBulletCount;
	// �{�X���o������U���̐�
	int m_specialAttackCount;

	// private�����o�֐�
	void UpdatePlayerBullets(float elapsedTime);// �v���C���[�̒e�X�V
	void UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy);// �G�̒e�X�V
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);// �G�ƃv���C���[�̒e�̓����蔻��
	bool CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy);// �v���C���[�ƓG�̒e�̓����蔻��
	void CheckCollisionWithBullets();	// �G�̒e�ƃv���C���[�̒e�̓����蔻��
	void SetSound();// ���ʉ��ݒ�
};