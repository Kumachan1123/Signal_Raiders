/*
	@file	ResultScene.h
	@brief	リザルトシーンクラス
*/
#pragma once
#include "Game/Scene/IScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/BackGround/BackGround.h"
#include "Game/Scene/ResultScene/ResultMenu/ResultMenu.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Scene/ResultScene/Result/Result.h"
#include "Game/MousePointer/MousePointer.h"
// 前方宣言
class CommonResources;
class Fade;
class BackGround;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class ResultScene final :
	public IScene
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// マウスポインター
	std::unique_ptr<MousePointer> m_pMousePointer;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// リザルトメニュー
	std::unique_ptr<ResultMenu> m_pResultMenu;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 結果クラス
	std::unique_ptr<Result> m_pResult;
	// 結果クラスに渡すテクスチャパス
	const wchar_t* m_pTexturePath;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// ステージ番号
	int m_stageNumber;
	// 空の行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// フェードで使用する変数
	bool m_isFade;		// フェードフラグ
	float m_volume;		// ボリューム
	int m_counter;		// フェードカウンタ
	// 音量の基準
	const float VOLUME = 0.5f;
	float m_BGMvolume;	// ボリューム
	float m_SEvolume;	// ボリューム
	float m_time = 0.0f;// 拡縮に使う時間
	float m_size = 0.0f;// 画像サイズ
	float m_pressKeySize = 0.0f;
	// 画面遷移フェード
	std::unique_ptr<Fade> m_pFade;
	// 背景
	std::unique_ptr<BackGround> m_pBackGround;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
public:
	ResultScene(IScene::SceneID sceneID);
	~ResultScene() override;

	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }
	SceneID GetNextSceneID() const;
	void InitializeFMOD();
	int GetStageNumber() const { return m_stageNumber; }

};
