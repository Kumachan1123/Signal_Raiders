/*
*	@file	IScene.h
*	@brief	シーンのインターフェイスクラス
*/
#pragma once
// 前方宣言
class CommonResources;
class IScene
{
public:
	// 列挙型
	enum class SceneID : unsigned int// シーンＩＤ
	{
		NONE,// シーンなし
		TITLE,// タイトル
		SETTING,// 設定画面
		STAGESELECT,// ステージセレクト
		PLAY,// プレイ
		CLEAR,// クリア
		GAMEOVER// ゲームオーバー
	};
public:
	virtual ~IScene() = default;// デストラクタ
	virtual void Initialize(CommonResources* resources) = 0;// 初期化
	virtual void Update(float elapsedTime) = 0;// 更新
	virtual void Render() = 0;// 描画
	virtual void Finalize() = 0;// 終了
	virtual SceneID GetNextSceneID() const = 0;// 次のシーンIDを取得
};
