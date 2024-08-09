/*
	@file	PlayScene.h
	@brief	プレイシーンクラス
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
#include "Game/Stages/Stage1.h"
#include "Game/Player/PlayerUI/PlayerHP/PlayerHP.h"
#include "Game/Player/PlayerUI/PlayerPointer/PlayerPointer.h"
#include "Game/Particle/Particle.h"
// 前方宣言
class CommonResources;

namespace mylib
{
	class InputManager;
	class DebugCamera;
	class GridFloor;
}

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}
class FPS_Camera;

class PlayScene final :
	public IScene
{
public:
	// Wi-Fiの強さの可変長配列を渡す
	//std::vector<int> GetWifiLevels()const { return m_wifilevels; }
private:
	// 共通リソース
	CommonResources* m_commonResources;

	// デバッグカメラ
	//std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// FPSカメラ
	std::unique_ptr<FPS_Camera> m_camera;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	// プレイヤーコントローラー
	std::unique_ptr<PlayerController>m_playerController;
	// プレイヤーの弾
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	// プレイヤーのHP
	float m_playerHP = 100.0f;
	std::unique_ptr <PlayerHP> m_pPlayerHP;
	// 照準
	std::unique_ptr<PlayerPointer> m_pPlayerPointer;
	// スカイボックス用のメンバ変数
	std::unique_ptr<SkyBox> m_skybox;
	// 敵
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// 格子床
	std::unique_ptr<mylib::GridFloor> m_gridFloor;
	// 地面（ステージ１
	std::unique_ptr<Stage1> m_stage1;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// Particle
	std::vector<std::unique_ptr<Particle>> m_particles;

	// 追加の変数==========================================


	// FMODで使用する変数（ポインタ）
	FMOD::System* m_system;	// FMODのシステム
	FMOD::Sound* m_soundSE;	// SE用の音声データ
	FMOD::Sound* m_soundBGM;	// BGM用の音声データ
	FMOD::Channel* m_channelSE;	// SEを再生するチャンネル
	FMOD::Channel* m_channelBGM;// BGMを再生するチャンネル

	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	float m_volume;		// ボリューム
	int m_counter;		// フェードカウンタ

	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 敵生成フラグ
	bool m_isEnemyBorn = false;
	// 敵生成済み
	bool m_isBorned = false;
	// 敵の数
	int m_enemyIndex = 0;
	// 敵の生成間隔
	float m_enemyBornInterval = 0.0f;
	// 敵の生成タイミングを管理
	float m_enemyBornTimer = 0.0f;
	// 弾生成フラグ
	bool m_isBullet = false;
	// 弾生成済み
	bool m_isBulletBorned = false;
	// プレイヤーと敵の当たり判定
	bool m_isHitPlayerToEnemy = false;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;
	// 回転角(度)
	float m_angle;
	// 弾の移動
	DirectX::SimpleMath::Vector3 m_ballPos;
	DirectX::SimpleMath::Vector3 m_ballDirection;
	DirectX::SimpleMath::Vector3 m_ballVel;
	// wi-fi関連の変数
		// クラスいろいろ
	std::unique_ptr<Wifi> m_wifi;
	// 「弾」境界球
	DirectX::BoundingSphere m_inPlayerArea;
	DirectX::BoundingSphere m_PlayerSphere;
	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
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
private:
	// FMODのシステムの初期化と音声データのロード
	void InitializeFMOD();
};
