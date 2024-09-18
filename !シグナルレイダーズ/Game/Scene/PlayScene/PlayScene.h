/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#include "Game/Scene/IScene.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/SkyBox/SkyBox.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
#include "Mouse.h"
#include "Game/Stages/Stage.h"
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
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullet;
	// �X�J�C�{�b�N�X�p�̃����o�ϐ�
	std::unique_ptr<SkyBox> m_skybox;
	// �G�S��
	std::unique_ptr<Enemies> m_pEnemies;
	// �G
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// �n�ʁi�X�e�[�W�P
	std::unique_ptr<Stage> m_pStage;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_pEffect;
	// �t�F�[�h
	std::unique_ptr<Fade> m_fade;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
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
	// ��]�p(�x)
	float m_angle;
	// ���ʂ̊
	const float VOLUME = 1.0f;
	float m_BGMvolume;	// �{�����[��
	float m_SEvolume;	// �{�����[��
	float m_mouseSensitivity;
	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	IScene::SceneID m_nowSceneID;
public:
	PlayScene(IScene::SceneID sceneID);
	~PlayScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
private:
	// FMOD�̃V�X�e���̏������Ɖ����f�[�^�̃��[�h
	void InitializeFMOD();

};
