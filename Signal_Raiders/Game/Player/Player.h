/*
*	@file		Player.h
*	@brief		�v���C���[�N���X
*/
#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED
// DirectXTK
#include <DeviceResources.h>
#include <Mouse.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include "Game/CommonResources.h"
// ����w�b�_�[�t�@�C��
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/WarningEffect/WarningEffects/WarningEffects.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/FPS_Camera/FPS_Camera.h"

// �O���錾
class CommonResources;
class PlayerController;
class PlayerUI;
class Goal;
class Reticle;
class PlayerHP;
class Crisis;
class WarningEffects;
class FPS_Camera;

// �v���C���[�N���X
class Player
{
public:
	// �A�N�Z�T
	// �v���C���[��HP
	float GetPlayerHP() const { return m_playerHP; }
	// �v���C���[��HP
	void SetPlayerHP(float playerHP) { m_playerHP = playerHP; }
	// �v���C���[�̍ő�HP
	float GetMaxPlayerHP() const { return m_maxPlayerHP; }
	// �v���C���[�̍ő�HP
	void SetMaxPlayerHP(float maxPlayerHP) { m_maxPlayerHP = maxPlayerHP; }
	// �v���C���[������鎞��
	float GetDashTime() const { return m_dashTime; }
	// �v���C���[������鎞��
	void SetDashTime(float dashTime) { m_dashTime = dashTime; }
	// �J����
	FPS_Camera* GetCamera() { return m_pCamera.get(); }
	// �G
	EnemyManager* GetEnemies() { return m_pEnemyManager; }
	// �e�}�l�[�W���[
	BulletManager* GetBulletManager() { return m_pBulletManager; }
	// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 GetPlayerPos() const { return m_playerPos; }
	// �v���C���[�̌���
	DirectX::SimpleMath::Vector3 GetPlayerDir() const { return m_playerDir; }
	// �v���C���[�R���g���[���[
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }
	// �v���C���[�̋��E��
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }
	// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }
	// ���ʎ擾
	float GetVolume() const { return m_SEVolume; }
	// ���ʎ擾
	void SetVolume(float volume) { m_SEVolume = volume; }
	// ���ʂ�␳����Ƃ��̒l
	float GetVolumeCorrection() const { return m_VolumeCorrection; }
	// ���ʂ�␳����Ƃ��̒l
	void SetVolumeCorrection(float volumeCorrection) { m_VolumeCorrection = volumeCorrection; }
	// �}�E�X���x
	float GetMouseSensitive() const { return m_mouseSensitive; }
	// �}�E�X���x
	void SetMouseSensitive(float sensitive) { m_mouseSensitive = sensitive; }
	// �U�����Ă����G�̌���
	DirectX::SimpleMath::Vector3 GetEnemyBulletDirection() const { return m_enemyDir; }
	// �U�����Ă����G�̌���
	void SetEnemyBulletDirection(const DirectX::SimpleMath::Vector3& enemyDir) { m_enemyDir = enemyDir; }
	// �v���C���[���_���[�W���󂯂���
	bool GetisPlayerDamage() const { return m_isDamage; }
	// �v���C���[���_���[�W���󂯂���
	void SetisPlayerDamage(bool isDamage) { m_isDamage = isDamage; }
	// �G�t�F�N�g�Đ��t���O
	bool GetisPlayEffect() const { return m_isPlayEffect; }
	// �G�t�F�N�g�Đ��t���O
	void SetisPlayEffect(bool isPlayEffect) { m_isPlayEffect = isPlayEffect; }
	// �`�[�g�R�}���h�F�G����|����t���O
	bool GetisKillAll() const { return m_isKillAll; }
	// �`�[�g�R�}���h���L����
	bool GetisCheat() const { return m_isCheat; }
	// �`�[�g�R�}���h���L����
	void SetisCheat(bool isCheat) { m_isCheat = isCheat; }
	// �e�}�l�[�W���[���Z�b�g
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }
public:
	// public�֐�
	// �R���X�g���N�^
	Player(CommonResources* commonResources);
	// �f�X�g���N�^
	~Player();
	// ������
	void Initialize(EnemyManager* pEnemiyManager);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// �e�𐶐�����
	void CreateBullet();
private:
	// private�֐�
	// �v���C���[���_���[�W���󂯂����̏���
	void PlayerDamage(float elapsedTime);
	// �U�����悤�Ƃ��Ă���G�Ɋւ��鏈��
	void InComingEnemy();
private:
	// �萔
	// �v���C���[��HP�����l
	const float PLAYER_HP = 100.0f;
	// �}�E�X���x�����l
	const float MOUSE_SENSITIVE = 0.1f;
	// �v���C���[�ƓG�Ƃ̈��͈͂̋��E���̔��a
	const float IN_PLAYER_AREA_RADIUS = 20.0f;
	// �v���C���[�̋��E���̔��a
	const float PLAYER_SPHERE_RADIUS = 3.0f;
	// �_���[�W���̃J�����h��Ɋւ���萔
	// �J�����h��̎��g��
	const float DAMAGE_SHAKE_FREQUENCY = 70.0f;
	// �J�����h��̐U��
	const float DAMAGE_SHAKE_AMPLITUDE = 0.15f;
	// �_���[�W�G�t�F�N�g�̌p������
	const float DAMAGE_DURATION = 0.25f;
private:
	// private�ϐ�
	// �R�������\�[�X
	CommonResources* m_pCommonResources;
	// �G
	EnemyManager* m_pEnemyManager;
	// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 m_playerPos;
	// �v���C���[�̌���
	DirectX::SimpleMath::Vector3 m_playerDir;
	// �J����
	std::unique_ptr<FPS_Camera> m_pCamera;
	// �v���C���[��HP
	float m_playerHP;
	// �v���C���[�̍ő�HP
	float m_maxPlayerHP;
	// �v���C���[������鎞��
	float m_dashTime;
	// �_���[�W�G�t�F�N�g���Ǘ�����N���X
	std::unique_ptr<WarningEffects> m_pWarningEffects;
	// �v���C���[�R���g���[���[
	std::unique_ptr<PlayerController> m_pPlayerController;
	// �e�}�l�[�W���[�i�v���C�V�[������󂯎��j
	BulletManager* m_pBulletManager;
	// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	DirectX::BoundingSphere m_inPlayerArea;
	// �v���C���[�̋��E��
	DirectX::BoundingSphere m_playerSphere;
	// �U�����Ă����G�̌���
	DirectX::SimpleMath::Vector3 m_enemyDir;
	// �v���C���[���_���[�W��H�������
	bool m_isDamage;
	// �G�t�F�N�g�Đ��t���O
	bool m_isPlayEffect;
	// �v���C���[���_���[�W��H��������̎���
	float m_damageTime;
	// SE�̉���
	float m_SEVolume;
	// ���ʕ␳
	float m_VolumeCorrection;
	// �}�E�X���x
	float m_mouseSensitive;
	// �`�[�g�R�}���h�F�G����|����t���O
	bool m_isKillAll;
	// �`�[�g�R�}���h���L����
	bool m_isCheat;
	// �o�ߎ���
	float m_timer;
};
#endif // PLAYER_DEFINED