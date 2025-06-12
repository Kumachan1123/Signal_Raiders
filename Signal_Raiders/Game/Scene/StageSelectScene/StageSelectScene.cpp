/*
*	@file	StageSelectScene.cpp
*	@brief	ステージセレクトシーンクラス
*/
#include <pch.h>
#include "StageSelectScene.h"
// 音量の基準値
const float StageSelectScene::VOLUME = 0.05f;
// タイトルに戻る値
const int StageSelectScene::TITLE = 5;
/*
*	@brief コンストラクタ
*	@details ステージセレクトシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
StageSelectScene::StageSelectScene(IScene::SceneID sceneID)
	: m_pCommonResources{}// 共通リソース
	, m_isChangeScene{}// シーン変更フラグ
	, m_BGMvolume{ VOLUME }// BGM音量
	, m_SEvolume{ VOLUME }// SE音量
	, m_elapsedTime{ 0.0f }// 経過時間
	, m_pFade{}// フェードクラスのポインター
	, m_pBackGround{ nullptr }// 背景クラスのポインター
	, m_pTexturePath{}// テクスチャパス
	, m_nowSceneID{ sceneID }// 現在のシーンID
	, m_stageNumber{}// ステージ番号
{
}
/*
*	@brief デストラクタ
*	@details ステージセレクトシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
StageSelectScene::~StageSelectScene()
{
	// 終了
	Finalize();
}
/*
*	@brief 初期化
*	@details ステージセレクトシーンの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void StageSelectScene::Initialize(CommonResources* resources)
{
	// リソースがnullptrでないことを確認
	assert(resources);
	// 共通リソースをセットする
	m_pCommonResources = resources;
	// デバイス関連のリソースを取得する
	auto DR = m_pCommonResources->GetDeviceResources();
	// フェードオブジェクトを生成する
	m_pFade = std::make_unique<Fade>(m_pCommonResources);
	// フェード処理を初期化する
	m_pFade->Initialize();
	// フェードをフェードイン状態に設定する
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// 背景オブジェクトを生成する
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);
	// 背景を初期化して描画可能にする
	m_pBackGround->Create(DR);
	// 設定データ用オブジェクトを生成する
	m_pSettingData = std::make_unique<SettingData>();
	// 設定ファイルを読み込む
	m_pSettingData->Load();
	// BGMの音量を設定ファイルから読み取り、係数をかけて反映
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	// SEの音量も同様に設定する
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// ステージセレクトメニューを生成する
	m_pStageSelectMenu = std::make_unique<StageSelectMenu>();
	// 効果音の音量をステージセレクトメニューに設定する
	m_pStageSelectMenu->SetSEVolume(m_SEvolume);
	// ステージセレクトメニューを初期化する
	m_pStageSelectMenu->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// マウスポインターUIを生成する
	m_pMousePointer = std::make_unique<MousePointer>();
	// マウスポインターの初期化を行う
	m_pMousePointer->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// ステージ選択処理クラスを生成する
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// ステージ選択画面の初期化を行う
	m_pStageSelect->Create(DR);
	// ゲーム終了前確認オブジェクトを作成する
	m_pGameEndChecker = std::make_unique<GameEndChecker>();
	// ゲーム終了前確認にSEの音量を設定する
	m_pGameEndChecker->SetSEVolume(m_SEvolume);
	// ゲーム終了前確認を初期化する
	m_pGameEndChecker->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// シーン変更フラグを初期化
	m_isChangeScene = false;
}
/*
*	@brief 更新
*	@details ステージセレクトシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void StageSelectScene::Update(float elapsedTime)
{
	// フレーム時間を保存
	m_elapsedTime = elapsedTime;
	// オーディオマネージャーの更新
	m_pCommonResources->GetAudioManager()->Update();
	// マウスの状態を取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 各種UIに渡す情報をまとめた構造体
	UpdateContext ctx;
	// 使わない
	ctx.bulletPoint = 0;
	// 時間を渡す
	ctx.elapsedTime = elapsedTime;
	// 使わない
	ctx.dashStamina = 0;
	// 使わない
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
	// フェードインが終了したら
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// 左クリックされたら
		if (MyMouse::IsLeftMouseButtonPressed(mouseState) && m_pStageSelectMenu->GetIsHit())
		{
			// サウンド再生
			m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
			// フェードアウト開始
			m_pFade->SetState(Fade::FadeState::FadeOut);
			// 選択されたステージ番号を記録
			m_stageNumber = m_pStageSelectMenu->GetMenuIndex();
		}
		else// 左クリックされていない場合
		{
			// マウスポインターの更新
			m_pMousePointer->Update(ctx);
			// ステージセレクトメニューの更新
			m_pStageSelectMenu->Update(ctx);
		}
	}
	// フェードアウトが終了したらシーン変更を可能にする
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
	// BGMの再生
	m_pCommonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);
	// 背景の更新
	m_pBackGround->Update(elapsedTime);
	// フェードの更新
	m_pFade->Update(elapsedTime);
	// ステージ選択更新
	m_pStageSelect->Update(elapsedTime);
}
/*
*	@brief 描画
*	@details ステージセレクトシーンの描画を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::Render()
{
	// 背景の描画
	m_pBackGround->Render();
	// ゲーム終了前の確認処理が有効な場合
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// ゲーム終了前の確認画面を描画する
		m_pGameEndChecker->Render();
	}
	else
	{
		// メニューと結果を描画
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
		{
			// ステージ選択の描画
			m_pStageSelect->Render();
			// ステージセレクトメニューの描画
			m_pStageSelectMenu->Render();
		}
	}
	// マウスポインターの描画
	m_pMousePointer->Render();
	// フェードの描画
	m_pFade->Render();
}
/*
*	@brief 終了処理
*	@details 自動で解放されるのでここでは何もしない
*	@param なし
*	@return なし
*/
void StageSelectScene::Finalize() {/* do nothing */ }

/*
*	@brief ゲーム終了前の確認処理
*	@details ゲーム終了前の確認処理を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::UpdateCheckGameEnd()
{
	// マウスの状態
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
*	@brief シーン変更の有無を取得する
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーンID
*/
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// BGMの停止
		m_pCommonResources->GetAudioManager()->StopSound("TitleBGM");
		// SEの停止
		m_pCommonResources->GetAudioManager()->StopSound("SE");
		// 終了フラグが立ってるならゲームを終了する
		if (m_pGameEndChecker->GetIsEndGame())PostQuitMessage(0);
		// 選ばれたメニューがタイトルに戻るものでなければ
		if (m_pStageSelectMenu->GetMenuIndex() < StageSelectScene::TITLE)
		{
			// プレイシーンへ遷移
			return IScene::SceneID::PLAY;
		}
		else// 選ばれたメニューがタイトルに戻るものであれば
		{
			// タイトルシーンへ遷移
			return IScene::SceneID::TITLE;
		}
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

