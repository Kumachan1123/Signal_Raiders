/*
	@file	PlayerBullets.h
	@brief	�v���C���[�̒e�S�̂��Ǘ�����N���X
*/
#pragma once
#include "Game/Player/Player.h"
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>

class CommonResources;
class Player;
class Enemies;
class PlayerBullets
{
public:
	// �R�������\�[�X
	CommonResources* m_commonResources;
	// �v���C���[�̒e
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullet;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// �v���C���[
	Player* m_pPlayer;
	// �G�S��
	Enemies* m_pEnemies;
	// �e�����t���O
	bool m_isBullet = false;
	// �e�����ς�
	bool m_isBulletBorned = false;
	// �e���������W
	const float DELETE_BULLET_POSITION = -0.25f;
	// �q�b�g�����Ƃ��̉��̉���
	const float HIT_VOLUME = 0.8f;
public:
	PlayerBullets(CommonResources* commonResources);
	~PlayerBullets();
	void Initialize(Player* pPlayer, Enemies* pEnemies);
	void Update(float elapsedTime);
	void Render();
	void CreateBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);
	// Getter
public:
	bool GetIsBullet() const { return m_isBullet; }
	bool GetIsBulletBorned() const { return m_isBulletBorned; }
	std::vector<std::unique_ptr<PlayerBullet>>& GetPlayerBullet() { return m_playerBullet; }
	// Setter
public:
	void SetIsBullet(bool isBullet) { m_isBullet = isBullet; }
	void SetIsBulletBorned(bool isBulletBorned) { m_isBulletBorned = isBulletBorned; }
private:
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);
};