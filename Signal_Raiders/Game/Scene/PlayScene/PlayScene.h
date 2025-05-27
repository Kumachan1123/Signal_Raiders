/*
*	@file	PlayScene.h
*	@brief	�v���C�V�[���N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <Mouse.h>
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include "Libraries/Microsoft/DebugDraw.h" 
#include <Libraries/MyLib/InputManager.h>
#include "Libraries/MyLib/GridFloor.h"
// ����w�b�_�[�t�@�C��
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Sky/Sky.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/Enemy/EnemyCounter/EnemyCounter.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Stage/Stage.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/DashGauge/DashGauge.h"
#include "Game/Player/PlayerUI/Reticle/Reticle.h"
#include "Game/Player/PlayerUI/Crisis/Crisis.h"
#include "Game/Player/PlayerUI/PlayGuide/PlayGuide.h"
#include "Game/Player/PlayerUI/Goal/Goal.h"
#include "Game/Player/PlayerUI/WifiLoading/WifiLoading.h"
#include "Game/Player/PlayerUI/BulletGauge/BulletGauge.h"
#include "Game/BossAppear/BossAppear.h"
#include "Game/Effect/Effect.h"
#include "Game/Fade/Fade.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Radar/Radar.h"
#include "Game/KumachiLib/Bloom/Bloom.h"
#include "Game/Interface/IPlayUI.h"
// �O���錾
class CommonResources;
class Fade;
class PlayScene final : public IScene
{
public:// �A�N�Z�T
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// �X�e�[�W�ԍ���ݒ肷��
	SceneID GetNextSceneID() const;// ���̃V�[��ID���擾����
public:// public�֐�
	PlayScene(IScene::SceneID sceneID);// �R���X�g���N�^
	~PlayScene() override;// �f�X�g���N�^
	void Initialize(CommonResources* resources) override;// ������
	void Update(float elapsedTime)override;// �X�V
	void Render() override;// �`��
	void Finalize() override;// �I��
public:
	// ���ʂ̊
	static const float VOLUME;
	// �Q�[���X�^�[�g����
	static const float GAME_START_TIME;
	// �҂�����
	static const float WAIT_TIME;
	// �댯�M����\������v���C���[�̗̑�
	static const float CRISIS_HP;
private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �v���C���[
	std::unique_ptr<Player> m_pPlayer;
	// �X�J�C�{�b�N�X�p�̃����o�ϐ�
	std::unique_ptr<Sky> m_pSky;
	// �G�S��
	std::unique_ptr<EnemyManager> m_pEnemyManager;
	// �e�}�l�[�W���[
	std::unique_ptr<BulletManager> m_pBulletManager;
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
	// ����
	std::unique_ptr<Goal> m_pGoal;
	// Wi-Fi���[�f�B���O
	std::unique_ptr<WifiLoading> m_pWifiLoading;
	// �v���C���[��HP��UI
	std::unique_ptr <PlayerHP> m_pPlayerHP;
	// �_�b�V���Q�[�W
	std::unique_ptr<DashGauge> m_pDashGauge;
	// �e�Q�[�W
	std::unique_ptr<BulletGauge> m_pBulletGauge;
	// ��@���
	std::unique_ptr<Crisis> m_pCrisis;
	// �Ə�
	std::unique_ptr<Reticle> m_pReticle;
	// �������
	std::unique_ptr<PlayGuide> m_pPlayGuide;
	// �v���C���[��UI(�Q�[���J�n����5�b���UI�j
	std::vector<std::unique_ptr<IPlayUI>> m_pPlayerUI;
	// �{�X�o�ꉉ�o
	std::unique_ptr<BossAppear> m_pBossAppear;
	// �t�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// �u���[��
	Bloom* m_pBloom;
	// �t�F�[�h�̏��
	Fade::FadeState m_fadeState;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �X�e�[�W�ԍ�
	int m_stageNumber;
	// BGM�{�����[��
	float m_BGMvolume;
	// SE�{�����[��
	float m_SEvolume;
	// �}�E�X���x
	float m_mouseSensitivity;
	// �o�ߎ���
	float m_time;
	// �G���S�ł������̃t�F�[�h�����܂ł̑҂�����
	float m_waitTime;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
	// HP�Đݒ�t���O
	bool m_isResetHP;


};
