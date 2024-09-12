/*
	@file	TitleScene.h
	@brief	タイトルシーンクラス
*/
#pragma once
#include "Game/Scene/IScene.h"
#include <DeviceResources.h>
#include "Game/Fade/Fade.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/TitleLogo/TitleLogo.h"
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Scene/PressKey/PressKey.h"
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
	std::unique_ptr<Fade> m_fade;
	// フェードの状態
	Fade::FadeState m_fadeState;
	// 背景
	std::unique_ptr<BackGround> m_backGround;
	// タイトルロゴ
	std::unique_ptr<TitleLogo> m_titleLogo;
	// 指示
	std::unique_ptr<PressKey> m_pressKey;
	// フェード画像番号
	int m_fadeTexNum;

	// シーンチェンジフラグ
	bool m_isChangeScene;

	// オーディオマネージャー
	AudioManager* m_audioManager;

	// FPSカメラ
	std::unique_ptr<FPS_Camera> m_camera;
	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	float m_volume;		// ボリューム
	int m_counter;		// フェードカウンタ


public:
	TitleScene();
	~TitleScene() override;


	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
	void InitializeFMOD();
private:


	void DrawSpace();// スペースキー押してってやつ描画

};
