/*
*	@file	SettingScene.cpp
*	@brief	メニュークラス
*/
#include <pch.h>
#include "SettingScene.h"

// 音量の基準値
const float SettingScene::VOLUME = 0.05f;
/*
*	@brief コンストラクタ
*	@details セッティングシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
SettingScene::SettingScene(IScene::SceneID sceneID)
	: m_pCommonResources{}// 共通リソース
	, m_isChangeScene{ false }// シーン変更フラグ
	, m_BGMvolume{ VOLUME }// BGM音量
	, m_SEvolume{ VOLUME }// SE音量
	, m_pDR{}// デバイスリソース
	, m_pFade{}// フェードクラスのポインター
	, m_fadeState{ }// フェードの状態
	, m_elapsedTime{ 0.0f }// 経過時間
	, m_pBackGround{ nullptr }// 背景クラスのポインター
	, m_pMousePointer{ nullptr }// マウスポインタークラスのポインター
	, m_pGameEndChecker{ nullptr }// ゲーム終了前確認画面のポインター
	, m_nowSceneID{ sceneID }// 現在のシーンID
	, m_pSettingMenu{}// メニュークラスのポインター
	, m_pSettingBar{}// 設定バークラスのポインター
	, m_pSettingData{}// 設定データクラスのポインター
{}
/*
*	@brief デストラクタ
*	@details セッティングシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
SettingScene::~SettingScene()
{
	// 後始末
	Finalize();
}
/*
*	@brief 初期化
*	@details セッティングシーンの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void SettingScene::Initialize(CommonResources* resources)
{
	// リソースがnullptrでないことを確認
	assert(resources);
	// 共通リソースをメンバにセットする
	m_pCommonResources = resources;
	// DirectX用のデバイスリソースを取得する
	auto DR = m_pCommonResources->GetDeviceResources();
	// フェード演出を生成する
	m_pFade = std::make_unique<Fade>(m_pCommonResources);
	// フェードの初期化を行う
	m_pFade->Initialize();
	// 最初はフェードイン状態に設定しておく
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// 背景描画用のクラスを生成する
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);
	// 背景のモデルやテクスチャなどを初期化する
	m_pBackGround->Create(DR);
	// 設定データを管理するクラスを作成する
	m_pSettingData = std::make_unique<SettingData>();
	// 設定ファイルを読み込んで値を取得する
	m_pSettingData->Load();
	// BGM音量を設定ファイルから読み込み、調整する
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	// SE音量も同様に設定ファイルから取得して調整する
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// 設定画面のメニューUIを生成する
	m_pSettingMenu = std::make_unique<SettingMenu>();
	// メニューに対してSEの音量を適用する
	m_pSettingMenu->SetSEVolume(m_SEvolume);
	// メニューの初期化を行う
	m_pSettingMenu->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// マウスカーソル用のポインターUIを作成する
	m_pMousePointer = std::make_unique<MousePointer>();
	// マウスポインターの初期化を行う
	m_pMousePointer->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// 音量バーなどのUIを含むセッティングバーを作成する
	m_pSettingBar = std::make_unique<SettingBar>();
	// セッティングバーにセッティングメニューを関連付ける
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());
	// セッティングバーの初期化を行う
	m_pSettingBar->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// ゲーム終了前確認のためのクラスを生成する
	m_pGameEndChecker = std::make_unique<GameEndChecker>();
	// ゲーム終了前確認にSEの音量を設定する
	m_pGameEndChecker->SetSEVolume(m_SEvolume);
	// ゲーム終了前確認の初期化を行う
	m_pGameEndChecker->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
}
/*
*	@brief 更新
*	@details セッティングシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void SettingScene::Update(float elapsedTime)
{
	// フレーム時間を保存する
	m_elapsedTime = elapsedTime;
	// 各種UIに渡す情報をまとめた構造体
	UpdateContext ctx;
	// 使わない
	ctx.bulletPoint = 0;
	// 使わない
	ctx.dashStamina = 0;
	//フレーム時間
	ctx.elapsedTime = elapsedTime;
	//使わない
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
		//　ESCキーが押されたらゲーム修了確認を有効にする
		if (keyboardState.Escape && !m_pGameEndChecker->GetIsGameEndCheck())
		{
			// ゲーム終了前の確認処理を有効化する
			m_pGameEndChecker->SetIsGameEndCheck(true);
			// ESCキーのSEを再生する
			m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		}
	}
	// 設定バーの更新
	m_pSettingBar->Update(ctx);
	// 設定メニューの更新
	m_pSettingMenu->Update(ctx);
	// フェード状態とマウスの更新
	UpdateFadeAndMouse(ctx);
	// 背景とフェードの更新
	UpdateBackgroundAndFade(elapsedTime);
}
/*
*	@brief 描画
*	@details セッティングシーンの描画を行う
*	@param なし
*	@return なし
*/
void SettingScene::Render()
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
		// フェードインが終わったら
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
		{
			// 各種UIの描画
			// 設定メニューの描画
			m_pSettingMenu->Render();
			// 設定バーの描画
			m_pSettingBar->Render();
		}
	}
	// マウスカーソルの描画
	m_pMousePointer->Render();
	// フェードの描画
	m_pFade->Render();
}
/*
*	@brief 終了処理
*	@details セッティングシーンの終了処理を行う
*	@param なし
*	@return なし
*/
void SettingScene::Finalize()
{
	// 音量の設定
	SetVolume();
}
/*
*	@brief 次のシーンIDを取得
*	@details セッティングシーンの次のシーンIDを取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID SettingScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// BGMの停止
		m_pCommonResources->GetAudioManager()->StopSound("TitleBGM");
		// 決定音の停止
		m_pCommonResources->GetAudioManager()->StopSound("SE");
		// 選択音の停止
		m_pCommonResources->GetAudioManager()->StopSound("Select");
		// 終了フラグが立ってるならゲームを終了する
		if (m_pGameEndChecker->GetIsEndGame())PostQuitMessage(0);
		// タイトルシーンに移行
		return IScene::SceneID::TITLE;
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}
/*
*	@brief 音量を設定する
*	@details BGMとSEの音量を設定する
*	@param なし
*	@return なし
*/
void SettingScene::SetVolume()
{
	// 音量の取得(10分の1で割合を取得)
	// BGM
	float BGMvolume = static_cast<float>(m_pSettingBar->GetSetting(0)) * 0.1f;
	// SE
	float SEvolume = static_cast<float>(m_pSettingBar->GetSetting(1)) * 0.1f;
	// 設定の変更
	// BGM
	m_pSettingData->SetBGMVolume(m_pSettingBar->GetSetting(0));
	// SE
	m_pSettingData->SetSEVolume(m_pSettingBar->GetSetting(1));
	// 音量の設定
	//	BGM音量
	m_BGMvolume = VOLUME * BGMvolume;
	// SE音量
	m_SEvolume = VOLUME * SEvolume;
}
/*
*	@brief フェードとマウスの更新
*	@details フェードとマウスの更新を行う
*	@param ctx 更新情報
*	@return なし
*/
void SettingScene::UpdateFadeAndMouse(const UpdateContext& ctx)
{
	// オーディオマネージャーの更新処理
	m_pCommonResources->GetAudioManager()->Update();
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// フェードの状態を確認し、フェードインが完了していない場合は何もしない
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;
	// 左クリックされたら
	if (MyMouse::IsLeftMouseButtonPressed(mouseState))
	{
		// メニュー選択の処理を行う
		HandleMenuSelection();
	}
	// マウスポインターの更新を行う
	m_pMousePointer->Update(ctx);
}
/*
*	@brief 背景とフェードの更新
*	@details 背景とフェードの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void SettingScene::UpdateBackgroundAndFade(float elapsedTime)
{
	// フェードアウトが完了していたらシーン変更フラグを立てる
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
	// BGMの再生
	m_pCommonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);
	// 背景の更新
	m_pBackGround->Update(elapsedTime);
	// フェードの更新
	m_pFade->Update(elapsedTime);
}
/*
*	@brief メニュー選択の処理
*	@details メニュー選択の処理を行う
*	@param なし
*	@return なし
*/
void SettingScene::HandleMenuSelection()
{
	// 選択されたメニューのIDを取得
	auto selectID = m_pSettingMenu->GetSelectIDNum();
	// 「終わる」か「適用」が選ばれれば
	if (selectID == SettingMenu::SelectID::END ||
		selectID == SettingMenu::SelectID::APPLY)
	{
		// SEの再生
		m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		// フェードアウトに移行
		m_pFade->SetState(Fade::FadeState::FadeOut);
		// もう他のUIは見なくていいのでこの処理を終える
		return;
	}

}
/*
*	@brief ゲーム終了前の確認処理
*	@details ゲーム終了前の確認処理を行う
*	@param なし
*	@return なし
*/
void SettingScene::UpdateCheckGameEnd()
{
	// マウスの状態を取得
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
