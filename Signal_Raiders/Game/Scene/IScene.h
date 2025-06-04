/*
*	@file	IScene.h
*	@brief	シーンのインターフェイスクラス
*/
#pragma once
// 前方宣言
class CommonResources;
class IScene
{
public:	// 列挙型
	// シーンＩＤ
	enum class SceneID : unsigned int
	{
		NONE,// シーンなし
		TITLE,// タイトル
		SETTING,// 設定画面
		STAGESELECT,// ステージセレクト
		PLAY,// プレイ
		CLEAR,// クリア
		GAMEOVER// ゲームオーバー
	};
public:// 関数
	// デストラクタ
	virtual ~IScene() = default;
	// 初期化
	virtual void Initialize(CommonResources* resources) = 0;
	// 更新
	virtual void Update(float elapsedTime) = 0;
	// 描画
	virtual void Render() = 0;
	// 終了
	virtual void Finalize() = 0;
	// 次のシーンIDを取得
	virtual SceneID GetNextSceneID() const = 0;
};
