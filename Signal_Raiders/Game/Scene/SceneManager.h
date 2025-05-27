/*
	@file	SceneManager.h
	@brief	シーンマネージャクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "IScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game/Scene/SettingScene/SettingScene.h"
#include "Game/Scene/PlayScene/PlayScene.h"
#include "Game/Scene/ResultScene/ResultScene.h"
#include "Game/Scene/StageSelectScene/StageSelectScene.h"
// 前方宣言
class CommonResources;
class SceneManager
{
public:// publicメンバ関数
	SceneManager();// コンストラクタ
	~SceneManager();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void Update(float elapsedTime);// 更新
	void Render();// 描画
	void Finalize();// 終了
private:// privateメンバ関数
	void ChangeScene(IScene::SceneID sceneID);// シーン変更
	void CreateScene(IScene::SceneID sceneID);// シーン作成
	void DeleteScene();// シーン削除
	IScene::SceneID GetSceneID() const { return m_nowSceneID; }// シーンID取得
	void SetSceneID(IScene::SceneID sceneID) { m_nowSceneID = sceneID; }// シーンID設定
private:// private定数
	static const int STAGE_MAX;// ステージ数
private:// privateメンバ変数
	std::unique_ptr<IScene> m_currentScene;// 現在のシーン
	CommonResources* m_pCommonResources;// 共通リソース
	IScene::SceneID m_nowSceneID;// 現在のシーンID
	int m_stageNumber;// プレイシーンにステージ番号を渡すための変数

};
