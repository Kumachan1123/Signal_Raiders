/*
	@file	TitleScene.h
	@brief	タイトルシーンクラス
*/
#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED
// 標準ライブラリ
#include <cassert>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include "Libraries/Microsoft/DebugDraw.h"
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/Scene/IScene.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/TitleLogo/TitleLogo.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/TitleScene/TitleMenu/TitleMenu.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"

// 前方宣言
class CommonResources;
class Fade;

// タイトルシーンクラス
class TitleScene final :public IScene
{
public:
	// public関数
	// コンストラクタ
	TitleScene(IScene::SceneID sceneID);
	// デストラクタ
	~TitleScene() override;
	// 初期化
	void Initialize(CommonResources* resources) override;
	// 更新
	void Update(float elapsedTime)override;
	// 描画
	void Render() override;
	// 終了
	void Finalize() override;
	// 次のシーンIDを取得
	SceneID GetNextSceneID() const override;
private:
	// 定数
	// 音量の基準
	static	const float VOLUME;
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
	// タイトルロゴ
	std::unique_ptr<TitleLogo> m_pTitleLogo;
	// タイトルメニュー
	std::unique_ptr<TitleMenu> m_pTitleMenu;
	// タイトル画面のUI(メニュー、マウスカーソル）
	std::vector<std::unique_ptr<IMenuUI>> m_pUI;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// BGM音量
	float m_BGMvolume;
	// SE音量
	float m_SEvolume;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
};
#endif // TITLE_SCENE_DEFINED
