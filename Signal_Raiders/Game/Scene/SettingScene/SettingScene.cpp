/*
*	@file	SettingScene.cpp
*	@brief	メニュークラス
*/
#include "pch.h"
#include "SettingScene.h"

const float SettingScene::VOLUME = 0.05f;// 音量の基準値
/*
*	@brief コンストラクタ
*	@details セッティングシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
SettingScene::SettingScene(IScene::SceneID sceneID)
	: m_commonResources{}// 共通リソース
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
	Finalize();// 後始末
}
/*
*	@brief 初期化
*	@details セッティングシーンの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void SettingScene::Initialize(CommonResources* resources)
{
	assert(resources);// リソースがnullptrでないことを確認
	m_commonResources = resources;// 共通リソースをセット
	auto DR = m_commonResources->GetDeviceResources();// デバイスリソースを取得
	m_pFade = std::make_unique<Fade>(m_commonResources);// フェードの生成
	m_pFade->Initialize();// フェードの初期化
	m_pFade->SetState(Fade::FadeState::FadeIn);// フェードインに移行
	m_pBackGround = std::make_unique<BackGround>(m_commonResources);// 背景を作成する
	m_pBackGround->Create(DR);// 背景の初期化
	m_pSettingMenu = std::make_unique<SettingMenu>();// セッティングメニューを作成
	m_pMousePointer = std::make_unique<MousePointer>();// マウスポインターを作成
	m_pSettingBar = std::make_unique<SettingBar>();// セッティングバーを作成
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());// セッティングメニューをセット
	m_pUI.push_back(std::move(m_pSettingMenu));// メニューをUIに登録
	m_pUI.push_back(std::move(m_pSettingBar));// セッティングバーをUIに登録
	m_pUI.push_back(std::move(m_pMousePointer));// マウスポインターをUIに登録
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);// UIの初期化
	m_pSettingData = std::make_unique<SettingData>();// 設定データクラスの作成
	m_pSettingData->Load();// 設定ファイルの読み込み
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGM音量を設定
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SE音量を設定
}
/*
*	@brief 更新
*	@details セッティングシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void SettingScene::Update(float elapsedTime)
{
	UpdateContext ctx;// 各種UIに渡す情報をまとめた構造体
	ctx.bulletPoint = 0;// 使わない
	ctx.dashStamina = 0;//使わない
	ctx.elapsedTime = elapsedTime;//フレーム時間
	ctx.playerHP = 0;//使わない
	UpdateSettingBars(ctx);	// メニューの更新
	UpdateFadeAndMouse(ctx);// フェード状態とマウスの更新
	UpdateBackgroundAndFade(elapsedTime);// 背景とフェードの更新
}
/*
*	@brief 描画
*	@details セッティングシーンの描画を行う
*	@param なし
*	@return なし
*/
void SettingScene::Render()
{
	m_pBackGround->Render();// 背景の描画
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)// フェードインが終わったら
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();// UIの描画
	m_pFade->Render();// フェードの描画
}
/*
*	@brief 終了処理
*	@details セッティングシーンの終了処理を行う
*	@param なし
*	@return なし
*/
void SettingScene::Finalize()
{
	SetVolume();// 音量の設定
}
/*
*	@brief 次のシーンIDを取得
*	@details セッティングシーンの次のシーンIDを取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID SettingScene::GetNextSceneID() const
{

	if (m_isChangeScene)// シーン変更がある場合
	{
		m_commonResources->GetAudioManager()->StopSound("TitleBGM");// BGMの停止
		m_commonResources->GetAudioManager()->StopSound("SE");// SEの停止
		m_commonResources->GetAudioManager()->StopSound("Select");// Selectの停止
		return IScene::SceneID::TITLE;// タイトルシーンに移行
	}
	return IScene::SceneID::NONE;// シーン変更がない場合
}
/*
*	@brief 音量を設定する
*	@details BGMとSEの音量を設定する
*	@param なし
*	@return なし
*/
void SettingScene::SetVolume()
{
	for (int it = 0; it < m_pUI.size(); ++it)// UIの数だけ処理
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(m_pUI[it].get()))// 設定バーのポインターを取得
		{
			// 音量の取得(10分の1で割合を取得)
			float BGMvolume = static_cast<float>(pSettingBar->GetSetting(0)) * 0.1f;// BGM
			float SEvolume = static_cast<float>(pSettingBar->GetSetting(1)) * 0.1f;// SE
			// 設定の変更
			m_pSettingData->SetBGMVolume(pSettingBar->GetSetting(0));// BGM
			m_pSettingData->SetSEVolume(pSettingBar->GetSetting(1));// SE
			// 音量の設定
			m_BGMvolume = VOLUME * BGMvolume;//	BGM音量
			m_SEvolume = VOLUME * SEvolume;// SE音量
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
	std::vector<SettingMenu*> settingMenus;// SettingMenuをリストアップ
	for (auto& settingMenu : m_pUI)// UIの数だけ繰り返す
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(settingMenu.get()))// SettingMenuのポインターを取得
			settingMenus.push_back(pSettingMenu);// SettingMenuをリストアップ
	for (auto& settingBar : m_pUI)// SettingBarをリストアップし、メニュー情報をセット
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(settingBar.get()))// SettingBarのポインターを取得
			for (auto& menu : settingMenus)// SettingMenuの数だけ繰り返す
				pSettingBar->SetStateIDNum(static_cast<SettingMenu::SettingID>(menu->GetMenuIndex()));// メニューのインデックスを取得
		settingBar->Update(ctx);// UIの更新
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
	m_commonResources->GetAudioManager()->Update();// オーディオマネージャーの更新処理
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;// メニューでの選択処理が行われたら
	if (mtracker->GetLastState().leftButton) HandleMenuSelection(ctx);// 左クリックされたらメニュー選択処理を行う
	else UpdateMousePointers(ctx);// マウスポインターの更新
}
/*
*	@brief 背景とフェードの更新
*	@details 背景とフェードの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void SettingScene::UpdateBackgroundAndFade(float elapsedTime)
{
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;// シーン変更フラグを立てる
	m_commonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);// BGMの再生
	m_pBackGround->Update(elapsedTime);// 背景の更新
	m_pFade->Update(elapsedTime);// フェードの更新
}
/*
*	@brief マウスポインターの更新
*	@details マウスポインターの更新を行う
*	@param ctx 更新情報
*	@return なし
*/
void SettingScene::UpdateMousePointers(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)// UIの数だけ繰り返す
		if (auto pMousePointer = dynamic_cast<MousePointer*>(m_pUI[it].get()))// マウスポインターのポインターを取得
			pMousePointer->Update(ctx);// マウスポインターの更新
}
/*
*	@brief メニュー選択の処理
*	@details メニュー選択の処理を行う
*	@param ctx 更新情報
*	@return なし
*/
void SettingScene::HandleMenuSelection(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)// UIの数だけ繰り返す
	{
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(m_pUI[it].get()))// SettingMenuのポインターを取得		
		{
			auto selectID = pSettingMenu->GetSelectIDNum();// 選択されたメニューのIDを取得
			if (selectID == SettingMenu::SelectID::END ||// 「終わる」か
				selectID == SettingMenu::SelectID::APPLY)// 「適用」かが選ばれれば
			{
				m_commonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);// SEの再生
				m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
				return;
			}
			UpdateMousePointers(ctx);// マウスポインターの更新
		}
	}
}
