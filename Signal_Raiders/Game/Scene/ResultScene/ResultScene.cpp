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
	// メニューを作成し、UIに登録
	m_pUI.push_back(std::move(m_pResultMenu));
	// マウスポインターを作成し、UIに登録
	m_pUI.push_back(std::move(std::make_unique<MousePointer>()));
	// UIの初期化
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
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
	// オーディオマネージャーを更新する（サウンドの再生や状態の反映など）
	m_pCommonResources->GetAudioManager()->Update();
	// マウスの状態を追跡するトラッカーを取得する
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// UIに渡す更新情報をまとめた構造体を準備する
	UpdateContext ctx;
	// 今回は弾の情報は使わないので0にしておく
	ctx.bulletPoint = 0;
	// 今回はダッシュのスタミナも使わないので0にしておく
	ctx.dashStamina = 0;
	// 経過時間（フレーム時間）を設定する
	ctx.elapsedTime = elapsedTime;
	// プレイヤーのHPも使わないので0にしておく
	ctx.playerHP = 0;
	// 登録されているUIすべてに対して更新処理を行う
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		// UIを更新する
		m_pUI[it]->Update(ctx);
		// このUIが結果画面のメニューかどうかを確認する
		auto pMenu = dynamic_cast<ResultMenu*>(m_pUI[it].get());
		// メニューUIでなければスキップする
		if (!pMenu) continue;
		// 以下の条件がすべて満たされた場合にメニュー選択を処理する：
		// ・フェードインが完了している
		// ・マウスの左ボタンが押されている
		// ・メニュー項目にマウスカーソルがヒットしている
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd &&
			mtracker->GetLastState().leftButton && pMenu->GetIsHit())
		{
			// 効果音を再生する
			m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
			// フェードアウトに切り替える
			m_pFade->SetState(Fade::FadeState::FadeOut);
			// 選択されたシーン番号を取得する
			ResultMenu::SceneID id = pMenu->GetSceneNum();
			// ステージセレクトが選ばれた場合はシーン番号を変更する
			if (id == ResultMenu::SceneID::SELECT_STAGE) m_stageNumber = STAGE_SELECT;
		}
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
	// メニューと結果を描画
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// 結果の描画
		m_pResult->Render();
		// UIの描画
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();
	}
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
		// 登録したUIの分だけ処理
		for (int it = 0; it < m_pUI.size(); ++it)
		{
			// メニューのポインターを取得
			if (auto pMenu = dynamic_cast<ResultMenu*>(m_pUI[it].get()))
			{
				// 選択されたメニューのIDを取得して、シーンIDを返す
				switch (pMenu->GetSceneNum())// 選択されたメニューのIDを取得
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
		}
	}
	// シーン変更がない場合は何もしない
	return IScene::SceneID::NONE;
}