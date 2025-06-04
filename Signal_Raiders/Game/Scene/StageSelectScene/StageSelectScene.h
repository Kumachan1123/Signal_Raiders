/*
*	@file	StageSelectScene.h
*	@brief	ステージセレクトシーンクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
// DirectX
#include <DeviceResources.h>
#include <Mouse.h>
 // 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/StageSelectScene/Menu/StageSelectMenu.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/StageSelectScene/Stage/StageSelect.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Interface/IMenuUI.h"
// 前方宣言
class CommonResources;
class Fade;
class BackGround;
class StageSelectScene final : public IScene
{
public:// アクセサ
	// 次のシーンIDを取得
	SceneID GetNextSceneID()const override;
	// ステージ番号を取得
	int GetStageNumber() const { return m_stageNumber; }
	// ステージ番号を設定
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }
public:// public関数
	// コンストラクタ
	StageSelectScene(IScene::SceneID sceneID);
	// デストラクタ
	~StageSelectScene() override;
	// 初期化
	void Initialize(CommonResources* resources) override;
	// 更新
	void Update(float elapsedTime)override;
	// 描画
	void Render() override;
	// 終了
	void Finalize() override;
private:// 定数
	// 音量の基準
	static const float VOLUME;
	// タイトルに戻る値
	static const int TITLE;
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// ステージ選択メニュー
	std::unique_ptr<StageSelectMenu> m_pStageSelectMenu;
	// マウスポインター
	std::unique_ptr<MousePointer> m_pMousePointer;
	// ステージ選択画面のUI(メニュー、マウスカーソル）
	std::vector<std::unique_ptr<IMenuUI>> m_pUI;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 結果クラス
	std::unique_ptr<StageSelect> m_pStageSelect;
	// 結果クラスに渡すテクスチャパス
	const wchar_t* m_pTexturePath;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// BGM音量
	float m_BGMvolume;
	// SE音量
	float m_SEvolume;
	// 画面遷移フェード
	std::unique_ptr<Fade> m_pFade;
	// 背景
	std::unique_ptr<BackGround> m_pBackGround;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
	// ステージ番号
	int m_stageNumber;
};
