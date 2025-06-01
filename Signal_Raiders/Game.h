/*
*	@file Game.h
*	@brief ゲームのメインクラス
*/
#pragma once
// 標準ライブラリ
#include <thread>
// DirectX
#include <DeviceResources.h>
#include "StepTimer.h"
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/nlohmann/json.hpp>
// ゲーム関連
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Scene/SceneManager.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/KumachiLib/ModelManager/ModelManager.h"
#include "Game/KumachiLib/TextureManager/TextureManager.h"
#include "Game/Screen.h"

class Game final : public DX::IDeviceNotify
{
public:// public関数
	Game() noexcept(false);// コンストラクタ
	~Game() = default;// デストラクタ

	Game(Game&&) = default;// ムーブコンストラクタ
	Game& operator= (Game&&) = default;// ムーブ代入演算子

	Game(Game const&) = delete;// コピーコンストラクタ
	Game& operator= (Game const&) = delete;// コピー代入演算子

	void Initialize(HWND window, int width, int height);// 初期化
	void Tick();// ゲームループを実行
	void OnDeviceLost() override;// デバイスが失われたときの処理
	void OnDeviceRestored() override;// デバイスが復元されたときの処理
	// メッセージ
	void OnActivated();// アクティブ化されたときの処理
	void OnDeactivated();// 非アクティブ化されたときの処理
	void OnSuspending();// 一時停止されたときの処理
	void OnResuming();// 再開されたときの処理
	void OnWindowMoved();// ウィンドウが移動されたときの処理
	void OnDisplayChange();// ディスプレイの変更があったときの処理
	void OnWindowSizeChanged(int width, int height);// ウィンドウサイズが変更されたときの処理
	void GetDefaultSize(int& width, int& height) const noexcept; // デフォルトのウィンドウサイズを取得
	void SetFullscreenState(BOOL value);// フルスクリーン状態を設定
private:// private関数
	void Update(DX::StepTimer const& timer);// 更新処理
	void Render();// 描画処理
	void Clear();// 画面をクリア
	void CreateDeviceDependentResources();// デバイス依存リソースの作成
	void CreateWindowSizeDependentResources(); // ウィンドウサイズ依存リソースの作成
private:// private変数
	// デバイスリソース
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;
	// タイマー
	DX::StepTimer                           m_timer;
	// フルスクリーン状態
	BOOL m_fullscreen;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>  m_commonStates;
	// コモンリソース
	std::unique_ptr<CommonResources>        m_pCommonResources;
	// デバッグ文字列
	std::unique_ptr<mylib::DebugString>     m_debugString;
	// 入力マネージャ
	std::unique_ptr<mylib::InputManager>    m_inputManager;
	// シーンマネージャ
	std::unique_ptr<SceneManager>           m_sceneManager;
	// オーディオマネージャ
	std::unique_ptr<AudioManager>           m_audioManager;
	// モデルマネージャ
	std::unique_ptr<ModelManager>           m_modelManager;
	// テクスチャマネージャ
	std::unique_ptr<TextureManager>         m_textureManager;
};
