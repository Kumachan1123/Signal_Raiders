/*
*	@file	BulletManager.h
*	@details �G�ƃv���C���[�̒e���Ǘ�����}�l�[�W���[�N���X
*	@brief	�e�Ǘ��N���X
*/
#pragma once
#ifndef BULLET_MANAGER_DEFINED
#define BULLET_MANAGER_DEFINED
// �W�����C�u����
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include "Game/CommonResources.h"
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

// �O���錾
class CommonResources;
class Player;
class IEnemy;
class EnemyManager;
class BulletParameters;

// �e�}�l�[�W���[�N���X
class BulletManager
{
public:
	// �A�N�Z�T
	// �G�̒e�̎�ސݒ�
	void SetEnemyBulletType(BulletType type) { m_enemyBulletType = type; }
	// �G�̒e�̑傫���ݒ�
	void SetEnemyBulletSize(float size) { m_enemyBulletSize = size; }
	// �v���C���[�̒e�����t���O�擾
	bool GetIsPlayerShoot() const { return m_isPlayerShoot; }
	// �v���C���[�̒e�����t���O�ݒ�
	void SetIsPlayerShoot(bool isPlayerShoot) { m_isPlayerShoot = isPlayerShoot; }
	// �e���ˏo�����G�̃|�C���^�[�ݒ�
	void SetShooter(IEnemy* pShooter) { m_pShooter = pShooter; }
	// �ǉ��_���[�W�ݒ�
	void SetAdditionalDamage(int additionalDamage);
	// �v���C���[�̒e�̐��擾
	int GetPlayerBulletCount() const { return m_playerBulletCount; }
	// �v���C���[�̒e�̐��ݒ�
	void SetPlayerBulletCount(int count) { m_playerBulletCount = count; }
	// ����U���̐��擾
	int GetSpecialAttackCount() const { return m_specialAttackCount; }
	// ����U���̐��ݒ�
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }
	// �����[�h���t���O�擾
	bool GetIsReloading() const { return m_isReloading; }
	// �����[�h���t���O�ݒ�
	void SetIsReloading(bool isReloading) { m_isReloading = isReloading; }
public:
	// public�����o�֐�
	// �R���X�g���N�^
	BulletManager(CommonResources* commonResources);
	// �f�X�g���N�^
	~BulletManager();
	// ������
	void Initialize(Player* pPlayer, EnemyManager* pEnemies);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// �v���C���[�̒e����
	void CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction);
	// �G�̒e����
	void CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction);
	// �e�̍폜�i�G�����񂾂Ƃ��ɌĂ΂��j
	void RemoveBulletsByShooter(IEnemy* shooter);
	// �e�̕�[
	void ReLoadPlayerBullet();
	// �e�̏���
	void ConsumePlayerBullet();
private:
	// private�����o�֐�
	// �v���C���[�̒e�X�V
	void UpdatePlayerBullets(float elapsedTime);
	// �G�̒e�X�V
	void UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy);
	// �G�ƃv���C���[�̒e�̓����蔻��
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);
	// �v���C���[�ƓG�̒e�̓����蔻��
	bool CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy);
	// �G�̒e�ƃv���C���[�̒e�̓����蔻��
	void CheckCollisionWithBullets();
private:
	// private�����o�ϐ�
	// �R�������\�[�X
	CommonResources* m_pCommonResources;
	// �v���C���[
	Player* m_pPlayer;
	// �G�S��
	EnemyManager* m_pEnemyManager;
	// �e���ˏo�����G�̃|�C���^�[
	IEnemy* m_pShooter;
	// �G�̒e�̎��
	BulletType m_enemyBulletType;
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
	std::vector<std::unique_ptr<PlayerBullet>> m_pPlayerBullets;
	// �G�̒e
	std::vector<std::unique_ptr<EnemyBullet>> m_pEnemyBullets;
	// �v���C���[�̒e�̍ő吔
	int m_playerBulletCount;
	// �{�X���o������U���̐�
	int m_specialAttackCount;
};
#endif // BULLET_MANAGER_DEFINED