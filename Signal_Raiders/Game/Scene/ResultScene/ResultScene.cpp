/*
*	@file	ResultScene.cpp
*	@brief　リザルトシーンクラス
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
	, m_elapsedTime{ 0.0f }// 経過時間
	, m_pSettingData{ nullptr }// 設定データクラスのポインター
	, m_pResult{}// リザルトクラスのポインター
	, m_pGameEndChecker{ nullptr }// ゲーム終了前確認画面のポインター
	, m_pResultMenu{ nullptr }// リザルトメニュークラスのポインター
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
	// リソースがnullptrでないことを確認
	assert(resources);
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソースを取得
	auto pDR = m_pCommonResources->GetDeviceResources();
	// フェードの生成
	m_pFade = std::make_unique<Fade>(m_pCommonResources);
	// フェードの初期化
	m_pFade->Initialize();
	// フェードインに移行
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// 背景を作成する
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);
	// 背景の初期化
	m_pBackGround->Create(pDR);
	// 設定データの初期化
	m_pSettingData = std::make_unique<SettingData>();
	// 設定ファイルの読み込み
	m_pSettingData->Load();
	// BGM音量を設定
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	// SE音量を設定
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// リザルトメニューを作成
	m_pResultMenu = std::make_unique<ResultMenu>();
	// SEの音量を設定
	m_pResultMenu->SetSEVolume(m_SEvolume);
	// リザルトメニューの初期化
	m_pResultMenu->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// マウスポインターUIを作成する
	m_pMousePointer = std::make_unique<MousePointer>();
	// マウスポインターUIを初期化する
	m_pMousePointer->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// 結果に応じて変わるテクスチャパスマップ
	// ゲームオーバー
	m_pResultTexturePathMap[IScene::SceneID::GAMEOVER] = L"Resources/Textures/GameOver.png";
	// クリア
	m_pResultTexturePathMap[IScene::SceneID::CLEAR] = L"Resources/Textures/Clear.png";
	// 結果クラス作成
	m_pResult = std::make_unique<Result>(m_pCommonResources);
	// シーンIDを設定
	m_pResult->SetSceneID(m_nowSceneID);
	// 結果クラスの初期化
	m_pResult->Create(pDR);
	// ゲーム終了前確認オブジェクトを作成する
	m_pGameEndChecker = std::make_unique<GameEndChecker>();
	// ゲーム終了前確認にSEの音量を設定する
	m_pGameEndChecker->SetSEVolume(m_SEvolume);
	// ゲーム終了前確認を初期化する
	m_pGameEndChecker->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// シーン変更フラグを初期化する
	m_isChangeScene = false;
}
/*
*	@brief 更新
*	@details リザルトシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void ResultScene::Update(float elapsedTime)
{
	// フレーム時間を保存
	m_elapsedTime = elapsedTime;
	// オーディオマネージャーを更新する（サウンドの再生や状態の反映など）
	m_pCommonResources->GetAudioManager()->Update();
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// UIに渡す更新情報をまとめた構造体を準備する
	UpdateContext ctx{};
	// 今回は弾の情報は使わないので0にしておく
	ctx.bulletPoint = 0;
	// 今回はダッシュのスタミナも使わないので0にしておく
	ctx.dashStamina = 0;
	// 経過時間（フレーム時間）を設定する
	ctx.elapsedTime = elapsedTime;
	// プレイヤーのHPも使わないので0にしておく
	ctx.playerHP = 0;
	// ゲーム終了前の確認処理
	if (m_pGameEndChecker->GetIsGameEndCheck())// ゲーム終了前の確認が有効な場合
	{
		// ゲーム終了前の確認処理を行う
		UpdateCheckGameEnd();
	}
	else// ゲーム終了前の確認が無効な場合
	{
		// キーボードの状態を取得する
		auto& keyboardState = m_pCommonResources->GetInputManager()->GetKeyboardState();
		//　ESCキーが押されたらゲーム終了確認を有効にする
		if (keyboardState.Escape && !m_pGameEndChecker->GetIsGameEndCheck())
		{
			// ゲーム終了前の確認処理を有効化する
			m_pGameEndChecker->SetIsGameEndCheck(true);
			// ESCキーのSEを再生する
			m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		}
	}
	// メニューの更新
	m_pResultMenu->Update(ctx);
	// マウスポインターの更新
	m_pMousePointer->Update(ctx);
	// 以下の条件がすべて満たされた場合にメニュー選択を処理する：
	// ・フェードインが完了している
	// ・マウスの左ボタンが押されている
	// ・メニュー項目にマウスカーソルがヒットしている
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd &&
		MyMouse::IsLeftMouseButtonPressed(mouseState) && m_pResultMenu->GetIsHit())
	{
		// 効果音を再生する
		m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		// フェードアウトに切り替える
		m_pFade->SetState(Fade::FadeState::FadeOut);
		// 選択されたシーン番号を取得する
		ResultMenu::SceneID id = m_pResultMenu->GetSceneNum();
		// ステージセレクトが選ばれた場合はシーン番号を変更する
		if (id == ResultMenu::SceneID::SELECT_STAGE) m_stageNumber = STAGE_SELECT;
	}
	// フェードアウトが完了していたら、シーン遷移フラグを立てる
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
	// 結果画面用のBGMを再生する
	m_pCommonResources->GetAudioManager()->PlaySound("ResultBGM", m_BGMvolume);
	// 背景のアニメーションなどを更新する
	m_pBackGround->Update(elapsedTime);
	// フェード効果の状態を更新する
	m_pFade->Update(elapsedTime);
	// リザルト情報（スコアや評価など）を更新する
	m_pResult->Update(elapsedTime);
}

/*
*	@brief 描画
*	@details リザルトシーンの描画を行う
*	@param なし
*	@return なし
*/
void ResultScene::Render()
{
	// 背景の描画
	m_pBackGround->Render();
	// ゲーム終了前の確認処理が有効な場合
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// ゲーム終了前の確認画面を描画する
		m_pGameEndChecker->Render();
	}
	else// ゲーム終了前の確認処理が無効な場合
	{
		// メニューと結果を描画
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
		{
			// 結果の描画
			m_pResult->Render();
			// リザルトメニューの描画
			m_pResultMenu->Render();
		}
	}
	// マウスポインターの描画
	m_pMousePointer->Render();
	// フェードの描画
	m_pFade->Render();
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
*	@brief ゲーム終了前の確認処理
*	@details ゲーム終了前の確認処理を行う
*	@param なし
*	@return なし
*/
void ResultScene::UpdateCheckGameEnd()
{
	// マウスの状態を取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// UIの更新に必要な情報をまとめた構造体
	UpdateContext ctx{};
	// 経過時間を渡す
	ctx.elapsedTime = m_elapsedTime;
	// ゲーム終了前の確認画面の更新
	m_pGameEndChecker->Update(ctx);
	// 左クリックされていて、UIにカーソルが当たっている場合
	if (MyMouse::IsLeftMouseButtonPressed(mouseState) && m_pGameEndChecker->GetIsHit())
	{
		// SEの再生
		m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		// ゲームをやめるかどうかのフラグを取得
		if (m_pGameEndChecker->GetIsEndGame())// ゲームをやめる場合
		{
			// フェードアウトに移行
			m_pFade->SetState(Fade::FadeState::FadeOut);
		}
		else// ゲームをやめない場合
		{
			// ゲーム終了前の確認フラグを無効化する
			m_pGameEndChecker->SetIsGameEndCheck(false);
		}
	}
}
/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーン変更の有無(NONE::何もしない、GAMEOVER:ゲームオーバー、CLEAR:クリア)
*/
IScene::SceneID ResultScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// BGM停止
		m_pCommonResources->GetAudioManager()->StopSound("ResultBGM");
		// SE停止
		m_pCommonResources->GetAudioManager()->StopSound("SE");
		// 終了フラグが立ってるならゲームを終了する
		if (m_pGameEndChecker->GetIsEndGame())PostQuitMessage(0);
		// 選択されたメニューのIDを取得して、シーンIDを返す
		switch (m_pResultMenu->GetSceneNum())// 選択されたメニューのIDを取得
		{
		case ResultMenu::SceneID::REPLAY:// もう一回やるが選ばれたら
			// 同じステージに戻る
			return IScene::SceneID::PLAY;
			break;
		case ResultMenu::SceneID::SELECT_STAGE:// ステージを選ぶが選ばれたら
			// ステージ選択に戻る
			return IScene::SceneID::STAGESELECT;
			break;
		default:// それ以外の選択肢
			// 何もしない
			break;
		}
	}
	// シーン変更がない場合は何もしない
	return IScene::SceneID::NONE;
}