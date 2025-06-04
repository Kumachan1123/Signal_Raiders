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
	// ステージセレクトメニューをUIリストに登録する
	m_pUI.push_back(std::move(m_pStageSelectMenu));
	// マウスポインターUIを生成してリストに登録する
	m_pUI.push_back(std::move(std::make_unique<MousePointer>()));
	// 全UIに対して初期化処理を行う
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// ステージ選択処理クラスを生成する
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// ステージ選択画面の初期化を行う
	m_pStageSelect->Create(DR);
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
	// オーディオマネージャーの更新
	m_pCommonResources->GetAudioManager()->Update();
	// マウスのトラッカーを取得する
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// フェードインが終了したら
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// 登録したUI分ループ
		for (int it = 0; it < m_pUI.size(); ++it)
		{
			// UIをメニューにキャスト
			auto pMenu = dynamic_cast<StageSelectMenu*>(m_pUI[it].get());
			// キャスト失敗したら次へ
			if (!pMenu) continue;
			// スペースキー（またはマウス左クリック）が押されたら
			if (mtracker->GetLastState().leftButton && pMenu->GetIsHit())
			{
				// サウンド再生
				m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
				// フェードアウト開始
				m_pFade->SetState(Fade::FadeState::FadeOut);
				// 選択されたステージ番号を記録
				m_stageNumber = pMenu->GetMenuIndex();
				// もう他のUIは見なくていいのでループ抜ける
				break;
			}
		}
		// マウスクリックされてない時は、各UIのアニメーション更新だけする
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
		// UIの更新
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Update(ctx);
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

