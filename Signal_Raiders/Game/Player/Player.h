#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullets/PlayerBullets.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"

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
	// �v���C���[�̒e
	std::unique_ptr<PlayerBullets> m_pPlayerBullets;

	// �_���[�W�G�t�F�N�g���Ǘ�����N���X
	std::unique_ptr<DamageEffects> m_pDamageEffects;
	// �v���C���[�R���g���[���[
	std::unique_ptr<PlayerController> m_pPlayerController;


	// ���E��
	DirectX::BoundingSphere m_inPlayerArea;// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	DirectX::BoundingSphere m_playerSphere;// �v���C���[�̋��E��
	// �U�����Ă����G�̌���
	DirectX::SimpleMath::Vector3 m_enemyDir;
	// �v���C���[���_���[�W��H�������
	bool m_isDamage = false;
	bool m_isPlayEffect = false;// �G�t�F�N�g�Đ��t���O
	// �v���C���[���_���[�W��H��������̎���
	float m_damageTime = 0.0f;
	// SE�̉���
	float m_SEVolume = 0.0f;
	// �}�E�X���x
	float m_mouseSensitive;
	// �`�[�g�R�}���h�F�G����|����t���O
	bool m_isKillAll;
	// �`�[�g�R�}���h���L����
	bool m_isCheat = false;
	// �o�ߎ���
	float m_timer = 0.0f;
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
	FPS_Camera* GetCamera() { return m_pCamera.get(); }// �J����
	EnemyManager* GetEnemies() { return m_pEnemyManager; }// �G
	PlayerBullets* GetPlayerBullets() { return m_pPlayerBullets.get(); }// �v���C���[�̒e
	DirectX::SimpleMath::Vector3 GetPlayerPos() const { return m_playerPos; }// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 GetPlayerDir() const { return m_playerDir; }// �v���C���[�̌���
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }// �v���C���[�R���g���[���[
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }// �v���C���[�̋��E��
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	float GetVolume() const { return m_SEVolume; }// ���ʎ擾
	void SetVolume(float volume) { m_SEVolume = volume; }// ���ʎ擾
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
public:
	void CreateBullet();// �e�𐶐�����
private:
	void PlayerDamage(float elapsedTime);// �v���C���[���_���[�W���󂯂����̏���

};