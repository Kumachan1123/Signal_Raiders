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
	, m_isFade{ false }// フェードフラグ
	, m_BGMvolume{ VOLUME }// BGM音量
	, m_SEvolume{ VOLUME }// SE音量
	, m_pDR{}// デバイスリソース
	, m_pFade{}// フェードクラスのポインター
	, m_fadeState{ }// フェードの状態
	, m_pBackGround{ nullptr }// 背景クラスのポインター
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
	// マウスカーソル用のポインターUIを作成する
	m_pMousePointer = std::make_unique<MousePointer>();
	// 音量バーなどのUIを含むセッティングバーを作成する
	m_pSettingBar = std::make_unique<SettingBar>();
	// セッティングバーにセッティングメニューを関連付ける
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());
	// セッティングメニューをUIリストに追加する
	m_pUI.push_back(std::move(m_pSettingMenu));
	// セッティングバーをUIリストに追加する
	m_pUI.push_back(std::move(m_pSettingBar));
	// マウスポインターをUIリストに追加する
	m_pUI.push_back(std::move(m_pMousePointer));
	// UIすべてに対して初期化処理を行う
	for (int it = 0; it < m_pUI.size(); ++it)
		m_pUI[it]->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
}
/*
*	@brief 更新
*	@details セッティングシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void SettingScene::Update(float elapsedTime)
{
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
	// メニューの更新
	UpdateSettingBars(ctx);
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
	// フェードインが終わったら
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// UIの描画
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();
	}
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
	// UIの数だけ処理
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		// 設定バーのポインターを取得
		if (auto pSettingBar = dynamic_cast<SettingBar*>(m_pUI[it].get()))
		{
			// 音量の取得(10分の1で割合を取得)
			// BGM
			float BGMvolume = static_cast<float>(pSettingBar->GetSetting(0)) * 0.1f;
			// SE
			float SEvolume = static_cast<float>(pSettingBar->GetSetting(1)) * 0.1f;
			// 設定の変更
			// BGM
			m_pSettingData->SetBGMVolume(pSettingBar->GetSetting(0));
			// SE
			m_pSettingData->SetSEVolume(pSettingBar->GetSetting(1));
			// 音量の設定
			//	BGM音量
			m_BGMvolume = VOLUME * BGMvolume;
			// SE音量
			m_SEvolume = VOLUME * SEvolume;
			return;
		}
	}
}
/*
*	@brief 設定バーの更新
*	@details 設定バーの更新を行う
*	@param ctx 更新情報
*	@return なし
*/
void SettingScene::UpdateSettingBars(const UpdateContext& ctx)
{
	// SettingMenuをリストアップ
	std::vector<SettingMenu*> settingMenus;
	// UIの数だけ繰り返す
	for (auto& settingMenu : m_pUI)
	{
		// SettingMenuのポインターを取得し、成功したら登録
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(settingMenu.get()))settingMenus.push_back(pSettingMenu);
	}
	// SettingBarをリストアップし、メニュー情報をセット
	for (auto& settingBar : m_pUI)
	{
		// SettingBarのポインターを取得
		if (auto pSettingBar = dynamic_cast<SettingBar*>(settingBar.get()))
		{
			// SettingMenuの数だけ繰り返し、メニューのインデックスを取得
			for (auto& menu : settingMenus)
				pSettingBar->SetStateIDNum(static_cast<SettingMenu::SettingID>(menu->GetMenuIndex()));
		}
		// UIの更新
		settingBar->Update(ctx);
	}
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
	// マウスのトラッカーを取得する
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// フェードの状態を確認し、フェードインが完了していない場合は何もしない
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;
	// 左クリックされたら
	if (mtracker->GetLastState().leftButton)
	{
		// メニュー選択の処理を行う
		HandleMenuSelection(ctx);
	}
	else// 左クリックされていない場合
	{
		// マウスポインターの更新
		UpdateMousePointers(ctx);
	}
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
*	@brief マウスポインターの更新
*	@details マウスポインターの更新を行う
*	@param ctx 更新情報
*	@return なし
*/
void SettingScene::UpdateMousePointers(const UpdateContext& ctx)
{
	// UIの数だけ繰り返す
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		// マウスポインターのポインターを取得
		if (auto pMousePointer = dynamic_cast<MousePointer*>(m_pUI[it].get()))
		{
			// マウスポインターの更新
			pMousePointer->Update(ctx);
		}
	}
}
/*
*	@brief メニュー選択の処理
*	@details メニュー選択の処理を行う
*	@param ctx 更新情報
*	@return なし
*/
void SettingScene::HandleMenuSelection(const UpdateContext& ctx)
{
	// UIの数だけ繰り返す
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		// SettingMenuのポインターを取得		
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(m_pUI[it].get()))
		{
			// 選択されたメニューのIDを取得
			auto selectID = pSettingMenu->GetSelectIDNum();
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
			// マウスポインターの更新
			UpdateMousePointers(ctx);
		}
	}
}
