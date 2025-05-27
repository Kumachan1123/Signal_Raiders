/*
*	@file	TitleScene.cpp
*	@brief	タイトルシーンクラス
*/
#include "pch.h"
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
	, m_nowSceneID{ sceneID }// シーンID
	, m_BGMvolume{ VOLUME }// BGM音量
	, m_SEvolume{ VOLUME }// SE音量
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
	assert(resources);// リソースがnullptrでないことを確認
	m_pCommonResources = resources;// 共通リソースを取得
	auto DR = m_pCommonResources->GetDeviceResources();// デバイスリソースを取得
	m_pFade = std::make_unique<Fade>(m_pCommonResources);// フェードの作成
	m_pFade->Initialize();// フェードの初期化
	m_pFade->SetState(Fade::FadeState::FadeIn);// フェードインに移行
	m_pBackGround = std::make_unique<BackGround>(m_pCommonResources);// 背景の作成
	m_pBackGround->Create(DR);// 背景の初期化
	m_pTitleLogo = std::make_unique<TitleLogo>(m_pCommonResources);// タイトルロゴを作成
	m_pTitleLogo->Create(DR);// タイトルロゴの初期化
	m_pSettingData = std::make_unique<SettingData>();// 設定データの作成
	m_pSettingData->Load();// 設定ファイルの読み込み
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGMの音量を設定 
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SEの音量を設定 
	m_pTitleMenu = std::make_unique<TitleMenu>();// タイトルメニューを作成
	m_pTitleMenu->SetSEVolume(m_SEvolume);// SEの音量を設定
	m_pUI.push_back(std::move(m_pTitleMenu));	// メニューを作成
	m_pUI.push_back(std::move(std::make_unique<MousePointer>()));	// マウスポインターを作成
	for (int it = 0; it < m_pUI.size(); ++it)// 一斉初期化
		m_pUI[it]->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);// UIの初期化

}
/*
*	@brief 更新する
*	@details タイトルシーンの更新を行う
*	@param elapsedTime フレーム時間
*	@return なし
*/
void TitleScene::Update(float elapsedTime)
{
	m_pCommonResources->GetAudioManager()->Update();// オーディオマネージャーの更新処理
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)// メニューでの選択処理が行われたら
	{
		for (int it = 0; it < m_pUI.size(); ++it)
		{
			if (auto pMenu = dynamic_cast<TitleMenu*>(m_pUI[it].get()))// UIがメニューの場合
			{
				if (mtracker->GetLastState().leftButton && pMenu->GetIsHit())// 左クリックされていて、UIにカーソルが当たっている場合
				{
					m_pCommonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);// SEの再生
					m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
					break; // もう他のUIは見なくていいのでループ抜ける
				}
			}
		}
		UpdateContext ctx;// UIの更新に必要な情報をまとめた構造体
		ctx.elapsedTime = elapsedTime;// フレーム時間を代入
		ctx.playerHP = 0;// 使わない値
		ctx.dashStamina = 0;// 使わない値
		ctx.bulletPoint = 0;// 使わない値
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Update(ctx);// UIの更新
	}
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;// フェードアウトが終了したらシーン変更を可能にする
	m_pCommonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);// BGMの再生
	m_pBackGround->Update(elapsedTime);// 背景の更新
	m_pFade->Update(elapsedTime);// フェードの更新
	m_pTitleLogo->Update(elapsedTime);// タイトルロゴの更新
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
	m_pBackGround->Render();// 背景の描画
	m_pTitleLogo->Render();// タイトルロゴの描画
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)// 画面遷移中は描画しない
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();// UIの描画
	m_pFade->Render();// フェードの描画
#ifdef _DEBUG// デバッグビルド時のみ実行
	auto debugString = m_pCommonResources->GetDebugString();// デバッグ情報を表示する
	auto& mousestate = m_pCommonResources->GetInputManager()->GetMouseState();// マウスの状態を取得する
	Vector2 pos = Vector2(static_cast<float>(mousestate.x), static_cast<float>(mousestate.y));// ウィンドウ上のマウス座標を取得する
	debugString->AddString("MouseX:%f  MouseY:%f", pos.x, pos.y);// マウス座標を表示する
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
	if (!m_isChangeScene)return IScene::SceneID::NONE;// シーン変更がないならすぐ戻る
	// 以下、シーン変更がある場合
	m_pCommonResources->GetAudioManager()->StopSound("TitleBGM");// BGMの停止
	m_pCommonResources->GetAudioManager()->StopSound("SE");// SEの停止
	for (int it = 0; it < m_pUI.size(); ++it)// pMenuを探す
	{
		auto pMenu = dynamic_cast<TitleMenu*>(m_pUI[it].get());// UIをメニューにキャスト
		if (!pMenu)continue;// メニューが見つからなかったら次のUIへ
		switch (pMenu->GetSceneNum())// シーンIDを取得
		{
		case TitleMenu::SceneID::STAGESELECT:// ステージセレクトだったら
			return IScene::SceneID::STAGESELECT;// ステージセレクトに遷移
			break;
		case TitleMenu::SceneID::SETTING:// 設定画面だったら
			return IScene::SceneID::SETTING;// 設定画面に遷移
			break;
		case TitleMenu::SceneID::END:// ゲーム終了だったら
			ExitGame();// ゲームを終了する
			break;
		default:// それ以外
			break;
		}
	}
	return IScene::SceneID::NONE;// 何もなかったらNONEを返す
}