/*
*	@file	SceneManager.cpp
*	@brief	シーンマネージャクラス
*/
#include <pch.h>
#include "SceneManager.h"
// ステージ数(条件式で??? < STAGE_MAXのように使い、0～4がステージ、5がタイトルに戻るボタンの番号）
const int SceneManager::STAGE_MAX = 5;
/*
*	@brief コンストラクタ
*	@details シーンマネージャクラスのコンストラクタ
*	@param なし
*	@return なし
*/
SceneManager::SceneManager()
	: m_pCurrentScene{}// 現在のシーン
	, m_pCommonResources{}// 共通リソース
	, m_stageNumber{ 0 }// ステージ番号
	, m_nowSceneID{ IScene::SceneID::NONE }// 現在のシーンID
{
}
/*
*	@brief デストラクタ
*	@details シーンマネージャクラスのデストラクタ
*	@param なし
*	@return	なし
*/
SceneManager::~SceneManager() { Finalize(); }
/*
*	@brief 初期化する
*	@details シーンマネージャクラスの初期化
*	@param resources 共通リソース
*	@return なし
*/
void SceneManager::Initialize(CommonResources* resources)
{
	// リソースがnullptrでないことを確認
	assert(resources);
	// 共通リソースを取得
	m_pCommonResources = resources;
	// タイトルシーンに変更
	ChangeScene(IScene::SceneID::TITLE);
}
/*
*	@brief 更新する
*	@details シーンマネージャクラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void SceneManager::Update(float elapsedTime)
{
	// 現在のシーンを更新
	m_pCurrentScene->Update(elapsedTime);
	// 次のシーンIDがNONEの場合はここで処理を終わる
	if (m_pCurrentScene->GetNextSceneID() == IScene::SceneID::NONE) return;
	// シーンを変更するとき
	ChangeScene(m_pCurrentScene->GetNextSceneID());
}
/*
*	@brief 描画する
*	@details シーンマネージャクラスの描画
*	@param なし
*	@return なし
*/
void SceneManager::Render()
{
	// 現在のシーンを描画する
	m_pCurrentScene->Render();
}
/*
*	@brief 終了する
*	@details シーンを削除する
*	@param なし
*	@return なし
*/
void SceneManager::Finalize()
{
	// 現在のシーンを削除する
	DeleteScene();
}
/*
*	@brief シーンを変更する
*	@details 今のシーンを消して新しいシーンを作成する
*	@param sceneID 新しいシーンのID
*	@return なし
*/
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	// シーンを削除する
	DeleteScene();
	// 新しいシーンを作成する
	CreateScene(sceneID);
}
/*
*	@brief シーンを作成する
*	@details 新しいシーンを作成する
*	@param sceneID 新しいシーンのID
*	@return なし
*/
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	// 現在のシーンがnullptrであることを確認
	assert(m_pCurrentScene == nullptr);
	// シーンIDによって処理を分ける
	switch (sceneID)
	{
	case IScene::SceneID::TITLE:// タイトルシーン
		// タイトルシーンを作成
		m_pCurrentScene = std::make_unique<TitleScene>(sceneID);
		// 処理を終える
		break;
	case IScene::SceneID::SETTING:// 設定シーン
		// 設定シーンを作成
		m_pCurrentScene = std::make_unique<SettingScene>(sceneID);
		// 処理を終える
		break;
	case IScene::SceneID::STAGESELECT:// ステージ選択シーン
		// ステージ選択シーンを作成
		m_pCurrentScene = std::make_unique<StageSelectScene>(sceneID);
		// 処理を終える
		break;
	case IScene::SceneID::PLAY:// プレイシーン
		// プレイシーンを作成
		m_pCurrentScene = std::make_unique<PlayScene>(sceneID);
		// 処理を終える
		break;
	case IScene::SceneID::CLEAR:	// リザルトシーン
	case IScene::SceneID::GAMEOVER:	// CLEARとGAMEOVERは同じ処理
		// リザルトシーンを作成
		m_pCurrentScene = std::make_unique<ResultScene>(sceneID);
		// 処理を終える
		break;
	default:// それ以外のシーン
		// シーン名が存在しない場合はエラー
		assert(!"SceneManager::CreateScene::シーン名が存在しません！");
		// no break
	}
	// シーンが生成されていることを確認
	assert(m_pCurrentScene && "SceneManager::CreateScene::次のシーンが生成されませんでした！");
	// 以下の条件を満たした場合
	if ((GetSceneID() == IScene::SceneID::STAGESELECT ||// ステージ選択か
		GetSceneID() == IScene::SceneID::CLEAR ||		// クリアか
		GetSceneID() == IScene::SceneID::GAMEOVER) &&	// ゲームオーバーのどれか且つ
		m_stageNumber < SceneManager::STAGE_MAX)		// ステージ番号が5未満の場合
	{
		// プレイシーンを取得
		auto playScene = dynamic_cast<PlayScene*>(m_pCurrentScene.get());
		// プレイシーンが取得できていることを確認
		assert(playScene);
		// ステージ番号をセット
		playScene->SetStageNumber(m_stageNumber);
	}
	// プレイシーンの場合
	if (GetSceneID() == IScene::SceneID::PLAY)
	{
		// リザルトシーンを取得
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());
		// リザルトシーンが取得できていることを確認
		assert(resultScene);
		// ステージ番号をセット
		resultScene->SetStageNumber(m_stageNumber);
	}
	// シーンの初期化
	m_pCurrentScene->Initialize(m_pCommonResources);
	// シーンIDをセット
	SetSceneID(sceneID);
}
/*
*	@brief シーンを削除する
*	@details 現在のシーンを削除する
*	@param なし
*	@return なし
*/
void SceneManager::DeleteScene()
{
	// シーンがnullptrの場合は何もしない
	if (!m_pCurrentScene)return;
	// 現在のシーンが「セレクトシーン」の場合、ステージ番号を取得する
	if (GetSceneID() == IScene::SceneID::STAGESELECT)
	{
		//ステージ番号を取得する
		auto stageSelectScene = dynamic_cast<StageSelectScene*>(m_pCurrentScene.get());
		// ステージセレクトシーンが取得できていることを確認
		assert(stageSelectScene);
		// ステージ番号を取得する
		m_stageNumber = stageSelectScene->GetStageNumber();
	}
	// 現在のシーンが「リザルトシーン」の場合、ステージ番号を取得する
	if (GetSceneID() == IScene::SceneID::CLEAR ||// クリアシーンか
		GetSceneID() == IScene::SceneID::GAMEOVER)// ゲームオーバーシーンの場合
	{
		// ステージ番号を取得する
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());
		// リザルトシーンが取得できていることを確認
		assert(resultScene);
		//ステージ番号を取得する
		m_stageNumber = resultScene->GetStageNumber();
	}
	// シーンを削除する
	m_pCurrentScene.reset();
}