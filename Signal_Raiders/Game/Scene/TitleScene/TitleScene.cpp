/*
*	@file	TitleScene.cpp
*	@brief	タイトルシーンクラス
*/
#include <pch.h>
#include "TitleScene.h"
// 音量の基準
const float TitleScene::VOLUME = 0.05f;
/*
*	@brief ゲーム終了関数
*	@details ゲームを終了する関数(main.cppに実装済みのものを呼び出す)
*	@param なし
*	@return なし
*/
extern void ExitGame() noexcept;
/*
*	@brief コンストラクタ
*	@details タイトルシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
TitleScene::TitleScene(IScene::SceneID sceneID)
	: m_pCommonResources{}// 共通リソース
	, m_isChangeScene{ false }// シーン変更フラグ
	, m_pDR{}// デバイスリソース
	, m_pFade{}// フェード
	, m_fadeState{}// フェード状態
	, m_pBackGround{ nullptr }// 背景
	, m_pTitleLogo{ nullptr }// タイトルロゴ
	, m_pTitleMenu{ nullptr }// タイトルメニュー
	, m_pSettingData{ nullptr }// 設定データ
	, m_pMousePointer{ nullptr }// マウスポインター
	, m_pGameEndChecker{ nullptr }// ゲーム終了前確認画面
	, m_nowSceneID{ sceneID }// シーンID
	, m_BGMvolume{ VOLUME }// BGM音量
	, m_SEvolume{ VOLUME }// SE音量
	, m_elapsedTime{ 0.0f }// 経過時間
{}
/*
*	@brief デストラクタ
*	@details Finalize()を呼ぶが、何もしない
*	@param なし
*	@return なし
*/
TitleScene::~TitleScene() { Finalize(); }
/*
*	@brief 初期化する
*	@details タイトルシーンの初期化を行う
*	@param resources 共通リソース
*	@return なし
*/
void TitleScene::Initialize(CommonResources* resources)
{
	// 共通リソースが null ではないことを確認
	assert(resources);
	// 共通リソースをセットする
	m_pCommonResources = resources;
	// デバイスリソースを取得する
	auto DR = m_pCommonResources->GetDeviceResources();
	// フェード演出用のオブジェクトを生成する
	m_pFade = std::make_unique<Fade>(m_pCommonResources);
	// フェード処理を初期化する
	m_pFade->Initialize();
	// フェードをフェードイン状態に設定する
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// 背景描画用オブジェクトを生成する
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);
	// 背景を初期化し、描画準備を整える
	m_pBackGround->Create(DR);
	// タイトルロゴオブジェクトを作成する
	m_pTitleLogo = std::make_unique<TitleLogo>(m_pCommonResources);
	// タイトルロゴを初期化する
	m_pTitleLogo->Create(DR);
	// 設定データオブジェクトを生成する
	m_pSettingData = std::make_unique<SettingData>();
	// 設定ファイルを読み込む
	m_pSettingData->Load();
	// BGMの音量を設定ファイルから取得して反映する
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	// SEの音量を設定ファイルから取得して反映する
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// タイトルメニューオブジェクトを作成する
	m_pTitleMenu = std::make_unique<TitleMenu>();
	// タイトルメニューにSEの音量を設定する
	m_pTitleMenu->SetSEVolume(m_SEvolume);
	// タイトルメニューを初期化する
	m_pTitleMenu->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// マウスポインターUIを作成する
	m_pMousePointer = std::make_unique<MousePointer>();
	// マウスポインターUIを初期化する
	m_pMousePointer->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// ゲーム終了前確認オブジェクトを作成する
	m_pGameEndChecker = std::make_unique<GameEndChecker>();
	// ゲーム終了前確認にSEの音量を設定する
	m_pGameEndChecker->SetSEVolume(m_SEvolume);
	// ゲーム終了前確認を初期化する
	m_pGameEndChecker->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
}
/*
*	@brief 更新する
*	@details タイトルシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void TitleScene::Update(float elapsedTime)
{
	// フレーム時間を保存
	m_elapsedTime = elapsedTime;
	// オーディオマネージャーの更新処理
	m_pCommonResources->GetAudioManager()->Update();
	// マウスの状態を取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// キーボードの状態を取得する
	auto& keyboardState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	//　ESCキーが押されたらゲーム終了確認を有効にする
	if (keyboardState.Escape && !m_pGameEndChecker->GetIsGameEndCheck())
	{
		// ゲーム終了前の確認処理を有効化する
		m_pGameEndChecker->SetIsGameEndCheck(true);
		// ESCキーのSEを再生する
		m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
		// 次のシーンIDをENDに設定する
		m_pTitleMenu->SetSceneNum(TitleMenu::SceneID::END);
	}
	// UIの更新に必要な情報をまとめた構造体
	UpdateContext ctx{};
	// フレーム時間を代入
	ctx.elapsedTime = elapsedTime;
	// ゲーム終了前の確認処理
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// ゲーム終了前の確認処理を行う
		UpdateCheckGameEnd();
	}
	else
	{
		// メニューでの選択処理が行われたら
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
		{
			// メニューの更新を行う
			m_pTitleMenu->Update(ctx);
			// 左クリックされていて、UIにカーソルが当たっている場合
			if (MyMouse::IsLeftMouseButtonPressed(mouseState) && m_pTitleMenu->GetIsHit())
			{
				// SEの再生
				m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
				// 選択されたメニューのシーンIDを取得
				if (m_pTitleMenu->GetSceneNum() != TitleMenu::SceneID::END)// 終了以外なら
				{
					// フェードアウトに移行
					m_pFade->SetState(Fade::FadeState::FadeOut);
				}
				else// 終了なら
				{
					// ゲーム終了前の確認処理を有効化する
					m_pGameEndChecker->SetIsGameEndCheck(true);
				}
			}
		}
	}
	// マウスポインターの更新
	m_pMousePointer->Update(ctx);
	// フェードアウトが終了したらシーン変更を可能にする
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
	// BGMの再生
	m_pCommonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);
	// 背景の更新
	m_pBackGround->Update(elapsedTime);
	// フェードの更新
	m_pFade->Update(elapsedTime);
	// タイトルロゴの更新
	m_pTitleLogo->Update(elapsedTime);

}
/*
*	@brief 描画する
*	@details タイトルシーンの描画を行う(背景、ロゴ、UI)
*	@param なし
*	@return なし
*/
void TitleScene::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
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
		// フェードインが終了したらUIを描画する
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
		{
			// タイトルロゴの描画
			m_pTitleLogo->Render();
			// タイトルメニューの描画
			m_pTitleMenu->Render();
		}
	}
	// マウスカーソルを描画する
	m_pMousePointer->Render();
	// フェードの描画
	m_pFade->Render();
#ifdef _DEBUG// デバッグビルド時のみ実行
	// デバッグ情報を表示する
	auto debugString = m_pCommonResources->GetDebugString();
	// マウスの状態を取得する
	auto& mousestate = m_pCommonResources->GetInputManager()->GetMouseState();
	// ウィンドウ上のマウス座標を取得する
	Vector2 pos = Vector2(static_cast<float>(mousestate.x), static_cast<float>(mousestate.y));
	// マウス座標を表示する
	debugString->AddString("MouseX:%f  MouseY:%f", pos.x, pos.y);
	// 左クリックの状態を表示する	
	debugString->AddString("LeftButton:%s", MyMouse::IsLeftMouseButtonPressed(mousestate) ? "true" : "false");
#endif
}
/*
*	@brief 終了する
*	@details 何もしない
*	@param なし
*	@return なし
*/
void TitleScene::Finalize() {/*do nothing*/ }
/*
* 	@brief 次のシーンIDを取得する
*	@details タイトルシーンの次のシーンIDを取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// シーン変更がないならすぐ戻る
	if (!m_isChangeScene)return IScene::SceneID::NONE;
	// 以下、シーン変更がある場合
	// BGMの停止
	m_pCommonResources->GetAudioManager()->StopSound("TitleBGM");
	// SEの停止
	m_pCommonResources->GetAudioManager()->StopSound("SE");
	// シーンIDを取得し、値によって分岐
	switch (m_pTitleMenu->GetSceneNum())
	{
	case TitleMenu::SceneID::STAGESELECT:// ステージセレクトだったら
		// ステージセレクトに遷移
		return IScene::SceneID::STAGESELECT;
		break;
	case TitleMenu::SceneID::SETTING:// 設定画面だったら
		// 設定画面に遷移
		return IScene::SceneID::SETTING;
		break;
	case TitleMenu::SceneID::END:// ゲーム終了だったら
		// ゲームを終了する
		PostQuitMessage(0);
		break;
	default:// それ以外
		// 何もしない
		break;
	}
	// 何もなかったらNONEを返す
	return IScene::SceneID::NONE;
}

/*
*	@brief ゲーム終了前の確認処理
*	@details ゲーム終了前の確認処理を行う
*	@param なし
*	@return なし
*/
void TitleScene::UpdateCheckGameEnd()
{
	// マウスステートを取得する
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