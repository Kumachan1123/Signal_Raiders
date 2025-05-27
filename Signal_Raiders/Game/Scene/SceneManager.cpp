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
	assert(resources);// リソースがnullptrでないことを確認
	m_pCommonResources = resources;// 共通リソースを取得
	ChangeScene(IScene::SceneID::TITLE);// タイトルシーンに変更
}
/*
*	@brief 更新する
*	@details シーンマネージャクラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void SceneManager::Update(float elapsedTime)
{
	m_pCurrentScene->Update(elapsedTime); // 現在のシーンを更新
	if (m_pCurrentScene->GetNextSceneID() == IScene::SceneID::NONE) return;// 次のシーンIDがNONEの場合はここで処理を終わる
	ChangeScene(m_pCurrentScene->GetNextSceneID());// シーンを変更するとき
}
/*
*	@brief 描画する
*	@details シーンマネージャクラスの描画
*	@param なし
*	@return なし
*/
void SceneManager::Render() { m_pCurrentScene->Render(); }
/*
*	@brief 終了する
*	@details シーンを削除する
*	@param なし
*	@return なし
*/
void SceneManager::Finalize() { DeleteScene(); }
/*
*	@brief シーンを変更する
*	@details 今のシーンを消して新しいシーンを作成する
*	@param sceneID 新しいシーンのID
*	@return なし
*/
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	DeleteScene();// シーンを削除する
	CreateScene(sceneID);// 新しいシーンを作成する
}
/*
*	@brief シーンを作成する
*	@details 新しいシーンを作成する
*	@param sceneID 新しいシーンのID
*	@return なし
*/
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_pCurrentScene == nullptr);// 現在のシーンがnullptrであることを確認
	switch (sceneID)// シーンIDによって処理を分ける
	{
	case IScene::SceneID::TITLE:// タイトルシーン
		m_pCurrentScene = std::make_unique<TitleScene>(sceneID);// タイトルシーンを作成
		break;// 処理を終える
	case IScene::SceneID::SETTING:// 設定シーン
		m_pCurrentScene = std::make_unique<SettingScene>(sceneID);// 設定シーンを作成
		break;// 処理を終える
	case IScene::SceneID::STAGESELECT:// ステージ選択シーン
		m_pCurrentScene = std::make_unique<StageSelectScene>(sceneID);// ステージ選択シーンを作成
		break;// 処理を終える
	case IScene::SceneID::PLAY:// プレイシーン
		m_pCurrentScene = std::make_unique<PlayScene>(sceneID);// プレイシーンを作成
		break;// 処理を終える
	case IScene::SceneID::CLEAR:	// リザルトシーン
	case IScene::SceneID::GAMEOVER:	// CLEARとGAMEOVERは同じ処理
		m_pCurrentScene = std::make_unique<ResultScene>(sceneID);// リザルトシーンを作成
		break;// 処理を終える
	default:// それ以外のシーン
		assert(!"SceneManager::CreateScene::シーン名が存在しません！");// シーン名が存在しない場合はエラー
		// no break
	}
	assert(m_pCurrentScene && "SceneManager::CreateScene::次のシーンが生成されませんでした！");// シーンが生成されていることを確認
	if ((GetSceneID() == IScene::SceneID::STAGESELECT ||// ステージ選択か
		GetSceneID() == IScene::SceneID::CLEAR ||		// クリアか
		GetSceneID() == IScene::SceneID::GAMEOVER) &&	// ゲームオーバーのどれか且つ
		m_stageNumber < SceneManager::STAGE_MAX)		// ステージ番号が5未満の場合
	{
		auto playScene = dynamic_cast<PlayScene*>(m_pCurrentScene.get());// プレイシーンを取得
		assert(playScene);// プレイシーンが取得できていることを確認
		playScene->SetStageNumber(m_stageNumber);// ステージ番号をセット
	}
	if (GetSceneID() == IScene::SceneID::PLAY)// プレイシーンの場合
	{
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());// リザルトシーンを取得
		assert(resultScene);// リザルトシーンが取得できていることを確認
		resultScene->SetStageNumber(m_stageNumber);// ステージ番号をセット
	}
	m_pCurrentScene->Initialize(m_pCommonResources);// シーンの初期化
	SetSceneID(sceneID);// シーンIDをセット
}
/*
*	@brief シーンを削除する
*	@details 現在のシーンを削除する
*	@param なし
*	@return なし
*/
void SceneManager::DeleteScene()
{
	if (!m_pCurrentScene)return;// シーンがnullptrの場合は何もしない
	if (GetSceneID() == IScene::SceneID::STAGESELECT)// 現在のシーンが「セレクトシーン」の場合、ステージ番号を取得する
	{
		auto stageSelectScene = dynamic_cast<StageSelectScene*>(m_pCurrentScene.get());//ステージ番号を取得する
		assert(stageSelectScene);// ステージセレクトシーンが取得できていることを確認
		m_stageNumber = stageSelectScene->GetStageNumber();// ステージ番号を取得する
	}
	if (GetSceneID() == IScene::SceneID::CLEAR ||
		GetSceneID() == IScene::SceneID::GAMEOVER)// 現在のシーンが「リザルトシーン」の場合、ステージ番号を取得する
	{
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());// ステージ番号を取得する
		assert(resultScene);// リザルトシーンが取得できていることを確認
		m_stageNumber = resultScene->GetStageNumber();//ステージ番号を取得する
	}
	m_pCurrentScene.reset();// シーンを削除する
}
