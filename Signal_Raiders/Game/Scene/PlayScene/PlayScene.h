/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#include "Game/Scene/IScene.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Sky/Sky.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/Enemy/EnemyCounter/EnemyCounter.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
#include "Mouse.h"
#include "Game/Stage/Stage.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/PlayerPointer/PlayerPointer.h"
#include "Game/Effect/Effect.h"
#include "Game/Fade/Fade.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Radar/Radar.h"
#include "Game/DamageEffect/DamageEffect.h"
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
	std::unique_ptr<Sky> m_skybox;
	// �G�S��
	std::unique_ptr<Enemies> m_pEnemies;
	// �G
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// ���[�_�[
	std::unique_ptr<Radar> m_pRadar;
	// �n�ʁi�X�e�[�W)
	std::unique_ptr<Stage> m_pStage;
	// ��
	std::unique_ptr<Wall> m_pWall;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �G�t�F�N�g
	std::vector<std::unique_ptr<Effect>> m_pEffect;

	// �G�J�E���^�[
	std::unique_ptr<EnemyCounter> m_pEnemyCounter;

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
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �X�e�[�W�ԍ�
	int m_stageNumber;
	// ���ʂ̊
	const float SE_VOLUME = 1.0f;// SE�̉���
	const float BGM_VOLUME = 0.5f;// BGM�̉���
	float m_BGMvolume;	// �{�����[��
	float m_SEvolume;	// �{�����[��
	float m_mouseSensitivity;	// �}�E�X���x
	// �o�ߎ���
	float m_timer;
	// �G���S�ł������̃t�F�[�h�����܂ł̑҂�����
	float m_waitTime;
	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	IScene::SceneID m_nowSceneID;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

public:
	PlayScene(IScene::SceneID sceneID);
	~PlayScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }
	SceneID GetNextSceneID() const;
private:
	// FMOD�̃V�X�e���̏������Ɖ����f�[�^�̃��[�h
	void InitializeFMOD();

};
