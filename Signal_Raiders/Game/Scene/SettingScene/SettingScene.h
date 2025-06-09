/*
*	@file	SettingScene.h
*	@brief	メニュークラス
*/
#pragma once
#ifndef SETTING_SCENE_DEFINED
#define SETTING_SCENE_DEFINED
// 標準ライブラリ
#include <cassert>
#include <vector>
#include <memory>
// DirectX
#include <Mouse.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Scene/GameEndChecker/GameEndChecker.h"
#include "Game/KumachiLib/MyMouse/MyMouse.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"

// 前方宣言
class CommonResources;
class Fade;

// 設定シーンクラス
class SettingScene final : public IScene
{
public:
	// public関数
	// コンストラクタ
	SettingScene(IScene::SceneID sceneID);
	// デストラクタ
	~SettingScene() override;
	// 初期化
	void Initialize(CommonResources* resources) override;
	// 更新
	void Update(float elapsedTime)override;
	// 描画
	void Render() override;
	// 終了処理
	void Finalize() override;
	// 次のシーンIDを取得
	SceneID GetNextSceneID() const;
private:
	// private関数
	// 音量設定
	void SetVolume();
	// フェードとマウスの更新
	void UpdateFadeAndMouse(const UpdateContext& ctx);
	// 背景とフェードの更新
	void UpdateBackgroundAndFade(float elapsedTime);
	// メニュー選択の処理
	void HandleMenuSelection();
	// ゲーム終了前の確認処理
	void UpdateCheckGameEnd();
private:
	// private定数
	// 音量の基準
	static const float VOLUME;
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	//	変数
	DX::DeviceResources* m_pDR;
	// フェード
	std::unique_ptr<Fade> m_pFade;
	// フェードの状態
	Fade::FadeState m_fadeState;
	// 背景
	std::unique_ptr<BackGround> m_pBackGround;
	// メニュー
	std::unique_ptr<SettingMenu> m_pSettingMenu;
	// 設定バー
	std::unique_ptr<SettingBar> m_pSettingBar;
	// マウスポインター
	std::unique_ptr<MousePointer> m_pMousePointer;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// ゲーム終了前確認画面
	std::unique_ptr<GameEndChecker> m_pGameEndChecker;
	// BGMボリューム
	float m_BGMvolume;
	// SEボリューム
	float m_SEvolume;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 経過時間
	float m_elapsedTime;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
};
#endif // SETTING_SCENE_DEFINED
