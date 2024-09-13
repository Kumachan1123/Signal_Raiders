#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullets/PlayerBullets.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/PlayerPointer/PlayerPointer.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/FPS_Camera/FPS_Camera.h"
// �O���錾
class CommonResources;
class PlayerController;
class PlayerUI;
class PlayerBullets;
class FPS_Camera;
class Enemies;
class Player
{
private:
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �G
	Enemies* m_pEnemies;
	// �J����
	std::unique_ptr<FPS_Camera> m_pCamera;
	// �v���C���[��HP
	float m_playerHP = 100.0f;
	// �v���C���[��HP��UI
	std::unique_ptr <PlayerHP> m_pPlayerHP;
	// �v���C���[�̒e
	std::unique_ptr<PlayerBullets> m_pPlayerBullets;
	// �Ə�
	std::unique_ptr<PlayerPointer> m_pPlayerPointer;
	// �v���C���[�R���g���[���[
	std::unique_ptr<PlayerController> m_pPlayerController;
	// ���E��
	DirectX::BoundingSphere m_inPlayerArea;// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	DirectX::BoundingSphere m_playerSphere;// �v���C���[�̋��E��
public:
	Player(CommonResources* commonResources);
	~Player();

	// ������
	void Initialize(Enemies* pEnemies);
	// �X�V
	void Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb, float elapsedTime);
	// �`��
	void Render();

	// Getter
public:
	float GetPlayerHP() { return m_playerHP; }	// �v���C���[��HP
	FPS_Camera* GetCamera() { return m_pCamera.get(); }// �J����
	PlayerController* GetPlayerController() { return m_pPlayerController.get(); }// �v���C���[�R���g���[���[
	DirectX::BoundingSphere GetPlayerSphere() const { return m_playerSphere; }// �v���C���[�̋��E��
	DirectX::BoundingSphere GetInPlayerArea() const { return m_inPlayerArea; }// �v���C���[�ƓG�Ƃ̈��͈͓��ł̋��E��
	// Setter
public:
	void SetPlayerHP(float playerHP) { m_playerHP = playerHP; }	// �v���C���[��HP
};