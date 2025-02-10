#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/DamageEffect/DamageEffects/DamageEffects.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/FPS_Camera/FPS_Camera.h"
// �O���錾
class CommonResources;
class PlayerController;
class PlayerUI;
class PlayerBullets;
class Goal;
class Reticle;
class PlayerHP;
class Crisis;
class DamageEffects;
class FPS_Camera;
class Enemies;
class Player
{
private:
	// �R�������\�[�X
	CommonResources* m_commonResources;
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
	std::unique_ptr<DamageEffects> m_pDamageEffects;
	// �v���C���[�R���g���[���[
	std::unique_ptr<PlayerController> m_pPlayerController;
	// �e�}�l�[�W���[�i�v���C�V�[������󂯎��j
	BulletManager* m_pBulletManager;
	// ���E��
	DirectX::BoundingSphere m_inPlayerArea;// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	DirectX::BoundingSphere m_playerSphere;// �v���C���[�̋��E��
	// �U�����Ă����G�̌���
	DirectX::SimpleMath::Vector3 m_enemyDir;
	// �v���C���[���_���[�W��H�������
	bool m_isDamage;
	bool m_isPlayEffect;// �G�t�F�N�g�Đ��t���O
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
private:// �萔
	// �v���C���[��HP�����l
	const float PLAYER_HP = 100.0f;
	// �}�E�X���x�����l
	const float MOUSE_SENSITIVE = 0.1f;
	// �v���C���[�ƓG�Ƃ̈��͈͂̋��E���̔��a
	const float IN_PLAYER_AREA_RADIUS = 20.0f;
	// �v���C���[�̋��E���̔��a
	const float PLAYER_SPHERE_RADIUS = 3.0f;
	// �_���[�W���̃J�����h��Ɋւ���萔
	const float DAMAGE_SHAKE_FREQUENCY = 70.0f;   // �J�����h��̎��g��
	const float DAMAGE_SHAKE_AMPLITUDE = 0.15f;  // �J�����h��̐U��
	const float DAMAGE_DURATION = 0.25f;         // �_���[�W�G�t�F�N�g�̌p������
public:
	Player(CommonResources* commonResources);
	~Player();

	// ������
	void Initialize(EnemyManager* pEnemies);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// Getter
public:
	float GetPlayerHP() const { return m_playerHP; }	// �v���C���[��HP
	void SetPlayerHP(float playerHP) { m_playerHP = playerHP; }	// �v���C���[��HP
	float GetMaxPlayerHP() const { return m_maxPlayerHP; }	// �v���C���[�̍ő�HP
	void SetMaxPlayerHP(float maxPlayerHP) { m_maxPlayerHP = maxPlayerHP; }	// �v���C���[�̍ő�HP
	float GetDashTime() const { return m_dashTime; }	// �v���C���[������鎞��
	void SetDashTime(float dashTime) { m_dashTime = dashTime; }	// �v���C���[������鎞��
	FPS_Camera* GetCamera() { return m_pCamera.get(); }// �J����
	EnemyManager* GetEnemies() { return m_pEnemyManager; }// �G
	BulletManager* GetBulletManager() { return m_pBulletManager; }// �e�}�l�[�W���[
	DirectX::SimpleMath::Vector3 GetPlayerPos() const { return m_playerPos; }// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 GetPlayerDir() const { return m_playerDir; }// �v���C���[�̌���
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }// �v���C���[�R���g���[���[
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }// �v���C���[�̋��E��
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	float GetVolume() const { return m_SEVolume; }// ���ʎ擾
	void SetVolume(float volume) { m_SEVolume = volume; }// ���ʎ擾
	float GetVolumeCorrection() const { return m_VolumeCorrection; }// ���ʂ�␳����Ƃ��̒l
	void SetVolumeCorrection(float volumeCorrection) { m_VolumeCorrection = volumeCorrection; }// ���ʂ�␳����Ƃ��̒l
	float GetMouseSensitive() const { return m_mouseSensitive; }// �}�E�X���x
	void SetMouseSensitive(float sensitive) { m_mouseSensitive = sensitive; }// �}�E�X���x
	DirectX::SimpleMath::Vector3 GetEnemyBulletDirection() const { return m_enemyDir; }// �U�����Ă����G�̌���
	void SetEnemyBulletDirection(DirectX::SimpleMath::Vector3 enemyDir) { m_enemyDir = enemyDir; }// �U�����Ă����G�̌���
	bool GetisPlayerDamage() const { return m_isDamage; }	// �v���C���[���_���[�W���󂯂���
	void SetisPlayerDamage(bool isDamage) { m_isDamage = isDamage; }// �v���C���[���_���[�W���󂯂���
	bool GetisPlayEffect() const { return m_isPlayEffect; }// �G�t�F�N�g�Đ��t���O
	void SetisPlayEffect(bool isPlayEffect) { m_isPlayEffect = isPlayEffect; }// �G�t�F�N�g�Đ��t���O
	bool GetisKillAll() const { return m_isKillAll; }// �`�[�g�R�}���h�F�G����|����t���O
	bool GetisCheat() const { return m_isCheat; }// �`�[�g�R�}���h���L����
	void SetisCheat(bool isCheat) { m_isCheat = isCheat; }// �`�[�g�R�}���h���L����
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }// �e�}�l�[�W���[���Z�b�g
public:
	void CreateBullet();// �e�𐶐�����
private:
	void PlayerDamage(float elapsedTime);// �v���C���[���_���[�W���󂯂����̏���

};