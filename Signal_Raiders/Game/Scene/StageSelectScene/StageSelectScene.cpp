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
	Finalize();// 終了
}
/*
*	@brief 初期化
*	@details ステージセレクトシーンの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void StageSelectScene::Initialize(CommonResources* resources)
{
	assert(resources);// リソースがnullptrでないことを確認
	m_pCommonResources = resources;// 共通リソースをセット
	auto DR = m_pCommonResources->GetDeviceResources();// デバイスリソースを取得
	m_pFade = std::make_unique<Fade>(m_pCommonResources);// フェードの生成
	m_pFade->Initialize();// フェードの初期化
	m_pFade->SetState(Fade::FadeState::FadeIn);// フェードインに移行
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);// 背景を作成する
	m_pBackGround->Create(DR);// 背景の初期化
	m_pSettingData = std::make_unique<SettingData>();	// 設定ファイルの読み込み
	m_pSettingData->Load();// 設定ファイルの読み込み
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGMの音量を設定 
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SEの音量を設定 
	m_pStageSelectMenu = std::make_unique<StageSelectMenu>();// ステージセレクトメニューを作成
	m_pStageSelectMenu->SetSEVolume(m_SEvolume);// SEの音量を設定
	m_pUI.push_back(std::move(m_pStageSelectMenu));// メニューをUIに登録
	m_pUI.push_back(std::move(std::make_unique<MousePointer>()));// マウスポインターを作成し、UIに登録
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);// UIの初期化
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);// ステージ選択クラス作成
	m_pStageSelect->Create(DR);// ステージ選択の初期化
	m_isChangeScene = false;// シーン変更フラグを初期化する
}
/*
*	@brief 更新
*	@details ステージセレクトシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void StageSelectScene::Update(float elapsedTime)
{
	m_pCommonResources->GetAudioManager()->Update();// オーディオマネージャーの更新
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();	// マウスのトラッカーを取得する
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)// フェードインが終了したら
	{
		for (int it = 0; it < m_pUI.size(); ++it)// 登録したUI分ループ
		{
			auto pMenu = dynamic_cast<StageSelectMenu*>(m_pUI[it].get());// UIをメニューにキャスト
			if (!pMenu) continue;// キャスト失敗したら次へ
			if (mtracker->GetLastState().leftButton && pMenu->GetIsHit())// スペースキー（またはマウス左クリック）が押されたら
			{
				m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);// サウンド再生
				m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウト開始
				m_stageNumber = pMenu->GetMenuIndex();// 選択されたステージ番号を記録
				break; // もう他のUIは見なくていいのでループ抜ける
			}
		}
		// マウスクリックされてない時は、各UIのアニメーション更新だけする
		UpdateContext ctx;// 各種UIに渡す情報をまとめた構造体
		ctx.bulletPoint = 0;// 使わない
		ctx.elapsedTime = elapsedTime;// 時間を渡す
		ctx.dashStamina = 0;// 使わない
		ctx.playerHP = 0;// 使わない
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Update(ctx);// UIの更新
	}
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;// フェードアウトが終了したらシーン変更を可能にする
	m_pCommonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);// BGMの再生
	m_pBackGround->Update(elapsedTime);// 背景の更新
	m_pFade->Update(elapsedTime);// フェードの更新
	m_pStageSelect->Update(elapsedTime);// ステージ選択更新
}
/*
*	@brief 描画
*	@details ステージセレクトシーンの描画を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::Render()
{
	m_pBackGround->Render();// 背景の描画
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)	// メニューと結果を描画
	{
		m_pStageSelect->Render();// ステージ選択の描画
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();// UIの描画
	}
	m_pFade->Render();// フェードの描画
}
/*
*	@brief 終了処理
*	@details 自動で解放されるのでここでは何もしない
*	@param なし
*	@return なし
*/
void StageSelectScene::Finalize() {/* do nothing */ }
/*
*	@brief シーン変更の有無を取得する
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーンID
*/
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	if (!m_isChangeScene)return IScene::SceneID::NONE;// シーン変更がないならすぐ戻る
	// シーン変更がある場合
	for (int it = 0; it < m_pUI.size(); ++it)// 一斉初期化
	{
		auto pMenu = dynamic_cast<StageSelectMenu*>(m_pUI[it].get());// UIをメニューにキャスト
		if (!pMenu)continue;// キャスト失敗したら次へ
		m_pCommonResources->GetAudioManager()->StopSound("TitleBGM");// BGMとSEの停止
		m_pCommonResources->GetAudioManager()->StopSound("SE");// BGMとSEの停止
		if (pMenu->GetMenuIndex() < StageSelectScene::TITLE)return IScene::SceneID::PLAY;// 選ばれたメニューがタイトルに戻るものでなければプレイシーンへ
		else return IScene::SceneID::TITLE;// タイトルシーンへ
	}
	return IScene::SceneID::NONE;// シーン変更がない場合
}

