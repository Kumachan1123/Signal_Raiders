/*
	@file	ResultScene.cpp
	@brief　リザルトシーンクラス
*/
#include <pch.h>
#include "ResultScene.h"
// 音量の基準
const float ResultScene::VOLUME = 0.05f;
// ステージ選択に移動する値
const int ResultScene::STAGE_SELECT = 5;
/*
*	@brief コンストラクタ
*	@details リザルトシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
ResultScene::ResultScene(IScene::SceneID sceneID)
	: m_pCommonResources{}// 共通リソース
	, m_isChangeScene{}// シーン変更フラグ
	, m_BGMvolume{ VOLUME }// BGM音量
	, m_SEvolume{ VOLUME }// SE音量
	, m_pFade{ nullptr }// フェードクラスのポインター
	, m_pBackGround{ nullptr }// 背景クラスのポインター
	, m_nowSceneID{ sceneID }// 現在のシーンID
	, m_stageNumber{ 0 }// ステージ番号
{
}
/*
*	@brief デストラクタ
*	@details リザルトシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
ResultScene::~ResultScene() { Finalize(); }
/*
*	@brief 初期化
*	@details リザルトシーンの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void ResultScene::Initialize(CommonResources* resources)
{
	assert(resources);// リソースがnullptrでないことを確認
	m_pCommonResources = resources;// 共通リソースをセット
	auto DR = m_pCommonResources->GetDeviceResources();// デバイスリソースを取得
	m_pFade = std::make_unique<Fade>(m_pCommonResources);// フェードの生成
	m_pFade->Initialize();// フェードの初期化
	m_pFade->SetState(Fade::FadeState::FadeIn);// フェードインに移行
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);// 背景を作成する
	m_pBackGround->Create(DR);// 背景の初期化
	m_pSettingData = std::make_unique<SettingData>();// 設定データの初期化
	m_pSettingData->Load();// 設定ファイルの読み込み
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGM音量を設定
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SE音量を設定
	m_pResultMenu = std::make_unique<ResultMenu>();// リザルトメニューを作成
	m_pResultMenu->SetSEVolume(m_SEvolume);// SEの音量を設定
	m_pUI.push_back(std::move(m_pResultMenu));// メニューを作成し、UIに登録
	m_pUI.push_back(std::move(std::make_unique<MousePointer>()));// マウスポインターを作成し、UIに登録
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);// UIの初期化
	// 結果に応じて変わるテクスチャパスマップ
	m_pResultTexturePathMap[IScene::SceneID::GAMEOVER] = L"Resources/Textures/GameOver.png";// ゲームオーバー
	m_pResultTexturePathMap[IScene::SceneID::CLEAR] = L"Resources/Textures/Clear.png";// クリア
	m_pResult = std::make_unique<Result>(m_pCommonResources);// 結果クラス作成
	auto textureMap = m_pResultTexturePathMap.find(m_nowSceneID);// ステートによって顔のモデルを変更
	if (textureMap != m_pResultTexturePathMap.end() && textureMap->second)// マップに存在する場合
		m_pResult->Create(DR, textureMap->second);// 結果クラスの初期化
	m_isChangeScene = false;// シーン変更フラグを初期化する
}
/*
*	@brief 更新
*	@details リザルトシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void ResultScene::Update(float elapsedTime)
{
	m_pCommonResources->GetAudioManager()->Update();// オーディオマネージャーの更新
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	UpdateContext ctx;// 各種UIに渡す情報をまとめた構造体
	ctx.bulletPoint = 0;// 使わない
	ctx.dashStamina = 0;// 使わない
	ctx.elapsedTime = elapsedTime;// フレーム時間
	ctx.playerHP = 0;// 使わない
	for (int it = 0; it < m_pUI.size(); ++it)// 登録したUIの分だけ更新
	{
		m_pUI[it]->Update(ctx);// UIの更新
		auto pMenu = dynamic_cast<ResultMenu*>(m_pUI[it].get());// メニューUIのポインタを取得
		if (!pMenu) continue;// メニューUIでない場合はスキップ
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd &&// フェードインが終わっていて
			mtracker->GetLastState().leftButton &&// マウス左ボタンが押されていて
			pMenu->GetIsHit())// メニューにヒットしている場合
		{
			m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);// SEの再生
			m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
			ResultMenu::SceneID id = pMenu->GetSceneNum();// 選択されたメニューのIDを取得
			if (id == ResultMenu::SceneID::SELECT_STAGE)
				m_stageNumber = STAGE_SELECT;// ステージを選ぶ
		}
	}
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;	// フェードアウトが終了したら
	m_pCommonResources->GetAudioManager()->PlaySound("ResultBGM", m_BGMvolume);// BGMの再生
	m_pBackGround->Update(elapsedTime);// 背景の更新
	m_pFade->Update(elapsedTime);// フェードの更新
	m_pResult->Update(elapsedTime);// 結果の更新
}
/*
*	@brief 描画
*	@details リザルトシーンの描画を行う
*	@param なし
*	@return なし
*/
void ResultScene::Render()
{
	m_pBackGround->Render();// 背景の描画
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)// メニューと結果を描画
	{
		m_pResult->Render();// 結果の描画
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();// UIの描画
	}
	m_pFade->Render();// フェードの描画
}
/*
*	@brief 終了処理
*	@details リザルトシーンの終了処理を行う
*	@param なし
*	@return なし
*/
void ResultScene::Finalize()
{
	// do nothing
}
/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーン変更の有無(NONE::何もしない、GAMEOVER:ゲームオーバー、CLEAR:クリア)
*/
IScene::SceneID ResultScene::GetNextSceneID() const
{
	if (m_isChangeScene)// シーン変更がある場合
	{
		m_pCommonResources->GetAudioManager()->StopSound("ResultBGM");// BGM停止
		m_pCommonResources->GetAudioManager()->StopSound("SE");// SE停止
		for (int it = 0; it < m_pUI.size(); ++it)// 登録したUIの分だけ処理
		{
			if (auto pMenu = dynamic_cast<ResultMenu*>(m_pUI[it].get()))// メニューのポインターを取得
			{
				switch (pMenu->GetSceneNum())// 選択されたメニューのIDを取得
				{
				case ResultMenu::SceneID::REPLAY:// もう一回やるが選ばれたら
					return IScene::SceneID::PLAY;// 同じステージに戻る
					break;
				case ResultMenu::SceneID::SELECT_STAGE:// ステージを選ぶが選ばれたら
					return IScene::SceneID::STAGESELECT;// ステージ選択に戻る
					break;
				default:// それ以外の選択肢
					break;// 何もしない
				}
			}
		}
	}
	return IScene::SceneID::NONE;// シーン変更がない場合は何もしない
}



