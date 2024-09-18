/*
	@file	PlayScene.h
	@brief	プレイシーンクラス
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
	std::unique_ptr<SkyBox> m_skybox;
	// 敵全体
	std::unique_ptr<Enemies> m_pEnemies;
	// 敵
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// 地面（ステージ１
	std::unique_ptr<Stage> m_pStage;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// エフェクト
	std::vector<std::unique_ptr<Effect>> m_pEffect;
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
	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	float m_volume;		// ボリューム
	int m_counter;		// フェードカウンタ
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 弾生成フラグ
	bool m_isBullet = false;
	// 弾生成済み
	bool m_isBulletBorned = false;
	// プレイヤーと敵の当たり判定
	bool m_isHitPlayerToEnemy = false;
	// 回転角(度)
	float m_angle;
	// 音量の基準
	const float VOLUME = 1.0f;
	float m_BGMvolume;	// ボリューム
	float m_SEvolume;	// ボリューム
	float m_mouseSensitivity;
	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
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
	// FMODのシステムの初期化と音声データのロード
	void InitializeFMOD();

};
