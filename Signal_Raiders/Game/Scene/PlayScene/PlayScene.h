/*
	@file	PlayScene.h
	@brief	プレイシーンクラス
*/
#pragma once
#include "Game/Scene/IScene.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Sky/Sky.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/Enemy/EnemyCounter/EnemyCounter.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/Wifi/Wifi.h"
#include "Mouse.h"
#include "Game/Stage/Stage.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/DashGauge/DashGauge.h"
#include "Game/Player/PlayerUI/Reticle/Reticle.h"
#include "Game/Player/PlayerUI/Crisis/Crisis.h"
#include "Game/Player/PlayerUI/PlayGuide/PlayGuide.h"
#include "Game/Player/PlayerUI/Goal/Goal.h"
#include "Game/Player/PlayerUI/BulletGauge/BulletGauge.h"
#include "Game/Effect/Effect.h"
#include "Game/Fade/Fade.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Radar/Radar.h"
#include "Game/KumachiLib/Bloom/Bloom.h"
// 前方宣言
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
	// 共通リソース
	CommonResources* m_commonResources;
	// プレイヤー
	std::unique_ptr<Player> m_pPlayer;
	// スカイボックス用のメンバ変数
	std::unique_ptr<Sky> m_skybox;
	// 敵全体
	std::unique_ptr<EnemyManager> m_pEnemyManager;
	// 弾マネージャー
	std::unique_ptr<BulletManager> m_pBulletManager;
	// レーダー
	std::unique_ptr<Radar> m_pRadar;
	// 地面（ステージ)
	std::unique_ptr<Stage> m_pStage;
	// 壁
	std::unique_ptr<Wall> m_pWall;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// エフェクト
	std::vector<std::unique_ptr<Effect>> m_pEffect;
	// 敵カウンター
	std::unique_ptr<EnemyCounter> m_pEnemyCounter;
	// 準備
	std::unique_ptr<Goal> m_pGoal;
	// プレイヤーのHPのUI
	std::unique_ptr <PlayerHP> m_pPlayerHP;
	// ダッシュゲージ
	std::unique_ptr<DashGauge> m_pDashGauge;
	// 弾ゲージ
	std::unique_ptr<BulletGauge> m_pBulletGauge;
	// 危機状態
	std::unique_ptr<Crisis> m_pCrisis;
	// 照準
	std::unique_ptr<Reticle> m_pReticle;
	// 操作説明
	std::unique_ptr<PlayGuide> m_pPlayGuide;
	// フェード
	std::unique_ptr<Fade> m_fade;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// ブルーム
	Bloom* m_pBloom;
	// フェードの状態
	Fade::FadeState m_fadeState;
	// フェード画像番号
	int m_fadeTexNum;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// ステージ番号
	int m_stageNumber;
	// 音量の基準
	const float SE_VOLUME = 0.5f;// SEの音量
	const float BGM_VOLUME = 0.5f;// BGMの音量
	float m_BGMvolume;	// ボリューム
	float m_SEvolume;	// ボリューム
	float m_mouseSensitivity;	// マウス感度
	// 経過時間
	float m_timer;
	// 敵が全滅した時のフェード処理までの待ち時間
	float m_waitTime;
	IScene::SceneID m_nowSceneID;


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
	// FMODのシステムの初期化と音声データのロード
	void InitializeFMOD();

};
