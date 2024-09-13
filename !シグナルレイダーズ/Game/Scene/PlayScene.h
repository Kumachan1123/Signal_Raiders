/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#include "IScene.h"
#include "Game/SkyBox/SkyBox.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/Interface/IBullet.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
#include "Mouse.h"
#include "Game/Stages/Stage1.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/PlayerPointer/PlayerPointer.h"
#include "Game/Effect/Effect.h"
#include "Game/Fade/Fade.h"
#include "Game/KumachiLib/AudioManager.h"
// �O���錾
class CommonResources;

namespace mylib
{
	class InputManager;
}


class Fade;
class PlayScene final :
	public IScene
{

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �v���C���[
	std::unique_ptr<Player> m_pPlayer;
	// �v���C���[�̒e
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	// �X�J�C�{�b�N�X�p�̃����o�ϐ�
	std::unique_ptr<SkyBox> m_skybox;
	// �G�S��
	std::unique_ptr<Enemies> m_pEnemies;
	// �G
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// �n�ʁi�X�e�[�W�P
	std::unique_ptr<Stage1> m_stage1;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_effect;
	// �t�F�[�h
	std::unique_ptr<Fade> m_fade;
	// �t�F�[�h�̏��
	Fade::FadeState m_fadeState;
	// �t�F�[�h�摜�ԍ�
	int m_fadeTexNum;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
	// �t�F�[�h�Ŏg�p����ϐ�
	bool m_isFade;		// �t�F�[�h�t���O
	float m_volume;		// �{�����[��
	int m_counter;		// �t�F�[�h�J�E���^
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �e�����t���O
	bool m_isBullet = false;
	// �e�����ς�
	bool m_isBulletBorned = false;
	// �v���C���[�ƓG�̓����蔻��
	bool m_isHitPlayerToEnemy = false;
	// ���f��
//	std::unique_ptr<DirectX::Model> m_model;
	// ��]�p(�x)
	float m_angle;


	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

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
private:
	// FMOD�̃V�X�e���̏������Ɖ����f�[�^�̃��[�h
	void InitializeFMOD();

};
