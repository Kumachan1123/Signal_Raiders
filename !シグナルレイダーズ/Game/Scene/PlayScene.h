/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#include "IScene.h"
#include "Game/SkyBox/SkyBox.h"
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IBullet.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
#include "Mouse.h"
// �O���錾
class CommonResources;

namespace mylib
{
	class InputManager;
	class DebugCamera;
	class GridFloor;
}

class FPS_Camera;

class PlayScene final :
    public IScene
{
public:
	// Wi-Fi�̋����̉ϒ��z���n��
	//std::vector<int> GetWifiLevels()const { return m_wifilevels; }
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// �f�o�b�O�J����
	//std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// FPS�J����
	std::unique_ptr<FPS_Camera> m_camera;	
	std::unique_ptr<DirectX::Mouse> m_mouse;
	// �v���C���[�R���g���[���[
	std::unique_ptr<PlayerController>m_playerController;
	// �v���C���[�̒e
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	// �v���C���[��HP
	int m_playerHP = 100;
	// �X�J�C�{�b�N�X�p�̃����o�ϐ�
	std::unique_ptr<SkyBox> m_skybox;
	// �G
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// �i�q��
	std::unique_ptr<mylib::GridFloor> m_gridFloor;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �G�����t���O
	bool m_isEnemyBorn = false;
	// �G�����ς�
	bool m_isBorned = false;
	// �e�����t���O
	bool m_isBullet = false;
	// �e�����ς�
	bool m_isBulletBorned = false;
	// �v���C���[�ƓG�̓����蔻��
	bool m_isHitPlayerToEnemy = false;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;
	// ��]�p(�x)
	float m_angle;
	// �e�̈ړ�
	DirectX::SimpleMath::Vector3 m_ballPos;
	DirectX::SimpleMath::Vector3 m_ballDirection;							
	DirectX::SimpleMath::Vector3 m_ballVel;
	// wi-fi�֘A�̕ϐ�
		// �N���X���낢��
	std::unique_ptr<Wifi> m_wifi;
	// �u�e�v���E��
	DirectX::BoundingSphere m_inPlayerArea;
	DirectX::BoundingSphere m_PlayerSphere;
	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	int m_count = 0;
public:
	PlayScene();
	~PlayScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;
	void UpdateBullets(float elapsedTime);
	void UpdateEnemies(float elapsedTime);
	SceneID GetNextSceneID() const;
	
};
