/*
*	@file	SettingScene.h
*	@brief	メニュークラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <vector>
#include <memory>
// DirectX
#include <Mouse.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include "Libraries/Microsoft/DebugDraw.h"
// 自作ヘッダーファイル
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;
class Fade;

class SettingScene final : public IScene
{
public:// public関数
	SettingScene(IScene::SceneID sceneID);	// コンストラクタ
	~SettingScene() override;// デストラクタ
	void Initialize(CommonResources* resources) override;// 初期化
	void Update(float elapsedTime)override;// 更新
	void Render() override;// 描画
	void Finalize() override;// 終了処理
	SceneID GetNextSceneID() const;// 次のシーンIDを取得
private:// private関数
	void SetVolume();// 音量設定
	void UpdateSettingBars(const UpdateContext& ctx);// 設定バーの更新
	void UpdateFadeAndMouse(const UpdateContext& ctx);// フェードとマウスの更新
	void UpdateBackgroundAndFade(float elapsedTime);// 背景とフェードの更新
	void UpdateMousePointers(const UpdateContext& ctx);// マウスポインターの更新
	void HandleMenuSelection(const UpdateContext& ctx);// メニュー選択の処理
private:// private定数
	static const float VOLUME;// 音量の基準
private:// private変数
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
	// 設定画面のUI
	std::vector < std::unique_ptr<IMenuUI> >m_pUI;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// BGMボリューム
	float m_BGMvolume;
	// SEボリューム
	float m_SEvolume;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// フェードフラグ
	bool m_isFade;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;


};
