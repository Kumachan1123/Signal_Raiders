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
	// プレイヤーの弾
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullet;
	// スカイボックス用のメンバ変数
	std::unique_ptr<Sky> m_skybox;
	// 敵全体
	std::unique_ptr<Enemies> m_pEnemies;
	// 敵
	std::vector<std::unique_ptr<Enemy>> m_enemy;
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

	// フェード
	std::unique_ptr<Fade> m_fade;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
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
	const float SE_VOLUME = 1.0f;// SEの音量
	const float BGM_VOLUME = 0.5f;// BGMの音量
	float m_BGMvolume;	// ボリューム
	float m_SEvolume;	// ボリューム
	float m_mouseSensitivity;	// マウス感度
	// 経過時間
	float m_timer;
	// 敵が全滅した時のフェード処理までの待ち時間
	float m_waitTime;
	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
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
	// FMODのシステムの初期化と音声データのロード
	void InitializeFMOD();

};
