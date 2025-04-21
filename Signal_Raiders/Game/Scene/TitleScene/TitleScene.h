/*
	@file	TitleScene.h
	@brief	タイトルシーンクラス
*/
#pragma once
#include "Game/Scene/IScene.h"
#include <DeviceResources.h>
#include "Game/Fade/Fade.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/TitleLogo/TitleLogo.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/TitleScene/TitleMenu/TitleMenu.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/MousePointer/MousePointer.h"
// 前方宣言
class CommonResources;
class Fade;

class FPS_Camera;

class TitleScene final :
	public IScene
{

private:
	// 共通リソース
	CommonResources* m_commonResources;
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
	// マウスポインター
	std::unique_ptr<MousePointer> m_pMousePointer;
	// メニュー
	std::unique_ptr<TitleMenu> m_pMenu;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// フェード画像番号
	int m_fadeTexNum;

	// シーンチェンジフラグ
	bool m_isChangeScene;

	// FPSカメラ
	std::unique_ptr<FPS_Camera> m_camera;
	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	// 音量の基準
	const float VOLUME = 0.5f;
	float m_BGMvolume;	// ボリューム
	float m_SEvolume;	// ボリューム
	int m_counter;		// フェードカウンタ
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;

public:
	TitleScene(IScene::SceneID sceneID);
	~TitleScene() override;
	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;
	SceneID GetNextSceneID() const;
};
