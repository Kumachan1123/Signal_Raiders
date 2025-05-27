/*
*	@file	PlayScene.h
*	@brief	プレイシーンクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <Mouse.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include "Libraries/Microsoft/DebugDraw.h" 
#include <Libraries/MyLib/InputManager.h>
#include "Libraries/MyLib/GridFloor.h"
// 自作ヘッダーファイル
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
// 前方宣言
class CommonResources;
class Fade;
class PlayScene final : public IScene
{
public:// アクセサ
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// ステージ番号を設定する
	SceneID GetNextSceneID() const;// 次のシーンIDを取得する
public:// public関数
	PlayScene(IScene::SceneID sceneID);// コンストラクタ
	~PlayScene() override;// デストラクタ
	void Initialize(CommonResources* resources) override;// 初期化
	void Update(float elapsedTime)override;// 更新
	void Render() override;// 描画
	void Finalize() override;// 終了
public:
	// 音量の基準
	static const float VOLUME;
	// ゲームスタート時間
	static const float GAME_START_TIME;
	// 待ち時間
	static const float WAIT_TIME;
	// 危険信号を表示するプレイヤーの体力
	static const float CRISIS_HP;
private:
	// 共通リソース
	CommonResources* m_pCommonResources;
	// プレイヤー
	std::unique_ptr<Player> m_pPlayer;
	// スカイボックス用のメンバ変数
	std::unique_ptr<Sky> m_pSky;
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
	// Wi-Fiローディング
	std::unique_ptr<WifiLoading> m_pWifiLoading;
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
	// プレイヤーのUI(ゲーム開始から5秒後のUI）
	std::vector<std::unique_ptr<IPlayUI>> m_pPlayerUI;
	// ボス登場演出
	std::unique_ptr<BossAppear> m_pBossAppear;
	// フェード
	std::unique_ptr<Fade> m_pFade;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// ブルーム
	Bloom* m_pBloom;
	// フェードの状態
	Fade::FadeState m_fadeState;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// ステージ番号
	int m_stageNumber;
	// BGMボリューム
	float m_BGMvolume;
	// SEボリューム
	float m_SEvolume;
	// マウス感度
	float m_mouseSensitivity;
	// 経過時間
	float m_time;
	// 敵が全滅した時のフェード処理までの待ち時間
	float m_waitTime;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
	// HP再設定フラグ
	bool m_isResetHP;


};
