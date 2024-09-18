/*
	@file	SettingScene.h
	@brief	メニュークラス
*/
#pragma once
#include "Game/Scene/IScene.h"
#include <DeviceResources.h>
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Scene/PressKey/PressKey.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Scene/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
// 前方宣言
class CommonResources;
class Fade;

class FPS_Camera;

class SettingScene final :
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
	// 指示
	std::unique_ptr<PressKey> m_pPressKey;
	// メニュー
	std::unique_ptr<SettingMenu> m_pSettingMenu;
	// 設定バー
	std::unique_ptr<SettingBar> m_pSettingBar;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// フェード画像番号
	int m_fadeTexNum;
	// 音量の基準
	const float VOLUME = 0.5f;
	float m_BGMvolume;	// ボリューム
	float m_SEvolume;	// ボリューム
	// シーンチェンジフラグ
	bool m_isChangeScene;

	// オーディオマネージャー
	AudioManager* m_audioManager;

	// FPSカメラ
	std::unique_ptr<FPS_Camera> m_camera;
	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ


public:
	SettingScene();
	~SettingScene() override;


	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();



private:

	void SetVolume();

};
