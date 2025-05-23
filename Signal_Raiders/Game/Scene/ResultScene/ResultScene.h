/*
	@file	ResultScene.h
	@brief	リザルトシーンクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <unordered_map>
// DirectX
#include "DeviceResources.h"
#include <Mouse.h>
// 外部ライブラリ
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/MyLib/InputManager.h"
// 自作ヘッダーファイル
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/ResultScene/ResultMenu/ResultMenu.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/ResultScene/Result/Result.h"
#include "Game/MousePointer/MousePointer.h"
#include "Game/Interface/IMenuUI.h"
// 前方宣言
class CommonResources;
class Fade;
class BackGround;

class ResultScene final : public IScene
{
public:// アクセサ
	int GetStageNumber() const { return m_stageNumber; }// ステージ番号を取得
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// ステージ番号を設定
	SceneID GetNextSceneID() const;// 次のシーンIDを取得
public:// public関数
	ResultScene(IScene::SceneID sceneID);// コンストラクタ
	~ResultScene() override;// デストラクタ
	void Initialize(CommonResources* resources) override;// 初期化
	void Update(float elapsedTime)override;// 更新
	void Render() override;// 描画
	void Finalize() override;// 終了
public:
	// 音量の基準
	static const float VOLUME;
	// ステージ選択に移動する値
	static const int STAGE_SELECT;
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// マウスポインター
	std::unique_ptr<MousePointer> m_pMousePointer;
	// リザルトメニュー
	std::unique_ptr<ResultMenu> m_pResultMenu;
	// リザルトシーンのUI(メニュー、マウスカーソル）
	std::vector<std::unique_ptr<IMenuUI>> m_pUI;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 結果クラス
	std::unique_ptr<Result> m_pResult;
	// 結果に応じて変わるテクスチャパスマップ
	std::unordered_map<IScene::SceneID, const wchar_t*> m_pResultTexturePathMap;
	// ステージ番号
	int m_stageNumber;
	// BGMボリューム
	float m_BGMvolume;
	// SEボリューム
	float m_SEvolume;
	// 画面遷移フェード
	std::unique_ptr<Fade> m_pFade;
	// 背景
	std::unique_ptr<BackGround> m_pBackGround;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
};
