/*
*	@file	PlayScene.cpp
*	@brief	プレイシーンクラス
*/
#include <pch.h>
#include "PlayScene.h"

// 音量の基準
const float PlayScene::VOLUME = 0.05f;
// ゲームスタート時間
const float PlayScene::GAME_START_TIME = 5.0f;
// 待ち時間	
const float PlayScene::WAIT_TIME = 1.0f;
// 危険信号を表示するプレイヤーの体力
const float PlayScene::CRISIS_HP = 20.0f;

/*
*	@brief コンストラクタ
*	@details プレイシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
PlayScene::PlayScene(IScene::SceneID sceneID)
	: m_pCommonResources{}// 共通リソース
	, m_projection{}// プロジェクション行列
	, m_isChangeScene{ false }// シーン変更フラグ
	, m_isResetHP{ false }// HPリセットフラグ
	, m_pStage{  }// 地面
	, m_pEffect{}// エフェクト
	, m_pFade{}// フェード
	, m_fadeState{ }// フェード状態
	, m_BGMvolume{ PlayScene::VOLUME }// BGMの音量
	, m_SEvolume{ PlayScene::VOLUME }// SEの音量
	, m_mouseSensitivity{ }// マウス感度
	, m_nowSceneID{ sceneID }// 現在のシーンID
	, m_time{ 0.0f }// 経過時間
	, m_waitTime{ 0.0f }// 待ち時間
	, m_stageNumber{ 0 }// ステージ番号
	, m_pBloom{ Bloom::GetInstance() }// ブルームエフェクト
{
}
/*
*	@brief デストラクタ
*	@details プレイシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
PlayScene::~PlayScene() { Finalize(); }
/*
*	@brief 初期化
*	@details プレイシーンクラスの初期化
*	@param resources 共通リソース
*	@return なし
*/
void PlayScene::Initialize(CommonResources* resources)
{
	// リソースが nullptr でないことを確認
	assert(resources);
	// 共通リソースを取得
	m_pCommonResources = resources;
	// デバイスリソースを取得
	auto DR = m_pCommonResources->GetDeviceResources();
	// 設定データを生成
	m_pSettingData = std::make_unique<SettingData>();
	// 設定ファイルからデータを読み込む
	m_pSettingData->Load();
	// BGMの音量を設定（共通のVOLUME値を掛ける）
	m_BGMvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	// SEの音量を設定（共通のVOLUME値を掛ける）
	m_SEvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// マウス感度を設定
	m_mouseSensitivity = static_cast<float>(m_pSettingData->GetMouseSensitivity());
	// 地面（ステージ）を生成
	m_pStage = std::make_unique<Stage>();
	// 地面の初期化
	m_pStage->Initialize(resources);
	// 壁の生成（共通リソースを渡す）
	m_pWall = std::make_unique<Wall>(resources);
	// 壁の作成（デバイスリソースを渡す）
	m_pWall->Create(DR);
	// 空の生成（ステージ番号を元に）
	m_pSky = std::make_unique<Sky>(m_stageNumber);
	// 空の初期化
	m_pSky->Initialize(resources);
	// プレイヤーの生成（共通リソースを渡す）
	m_pPlayer = std::make_unique<Player>(resources);
	// 敵マネージャーを生成（共通リソースを渡す）
	m_pEnemyManager = std::make_unique<EnemyManager>(resources);
	// ステージ番号を敵マネージャーに設定
	m_pEnemyManager->SetStageNumber(m_stageNumber);
	// プレイヤーの効果音の音量を設定
	m_pPlayer->SetVolume(m_SEvolume);
	// BGMよりSEの方が大きい場合は補正する
	if (m_SEvolume > m_BGMvolume)	m_pPlayer->SetVolumeCorrection(m_SEvolume - m_BGMvolume);
	// プレイヤーのマウス感度を設定
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);
	// プレイヤーを初期化（敵マネージャーを渡す）
	m_pPlayer->Initialize(m_pEnemyManager.get());
	// 敵マネージャーを初期化（プレイヤーを渡す）
	m_pEnemyManager->Initialize(m_pPlayer.get());
	// 敵の効果音の音量を設定
	m_pEnemyManager->SetSEVolume(m_SEvolume);
	// 敵に壁情報を渡す
	m_pEnemyManager->SetWall(m_pWall.get());
	// 弾マネージャーを生成（共通リソースを渡す）
	m_pBulletManager = std::make_unique<BulletManager>(resources);
	// 弾マネージャーを初期化（プレイヤー・敵マネージャーを渡す）
	m_pBulletManager->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// プレイヤーに弾マネージャーを渡す
	m_pPlayer->SetBulletManager(m_pBulletManager.get());
	// 敵マネージャーに弾マネージャーを渡す
	m_pEnemyManager->SetBulletManager(m_pBulletManager.get());
	// 敵カウンターを生成
	m_pEnemyCounter = std::make_unique<EnemyCounter>();
	// 敵カウンターを初期化
	m_pEnemyCounter->Initialize(resources);
	// ゴールUIを生成（共通リソースを渡す）
	m_pGoal = std::make_unique<Goal>(m_pCommonResources);
	// ゴールUIの作成（デバイスリソースを渡す）
	m_pGoal->Create(DR);
	// 危険状態のUIを生成（共通リソースを渡す）
	m_pCrisis = std::make_unique<Crisis>(m_pCommonResources);
	// 危険状態のUIを作成（デバイスリソースを渡す）
	m_pCrisis->Create(DR);
	// Wi-FiローディングUIを生成
	m_pWifiLoading = std::make_unique<WifiLoading>();
	// Wi-FiローディングUIを初期化
	m_pWifiLoading->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// プレイ中に表示されるUIたちを登録
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayerHP>()));     // プレイヤーHP
	m_pPlayerUI.push_back(std::move(std::make_unique<DashGauge>()));    // ダッシュゲージ
	m_pPlayerUI.push_back(std::move(std::make_unique<BulletGauge>()));  // 弾ゲージ
	m_pPlayerUI.push_back(std::move(std::make_unique<Reticle>()));      // 照準
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayGuide>()));    // 操作説明ガイド
	// 登録されたUIを全て初期化する
	for (int it = 0; it < m_pPlayerUI.size(); ++it)	m_pPlayerUI[it]->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// フェードを生成
	m_pFade = std::make_unique<Fade>(m_pCommonResources);
	// フェードの初期化
	m_pFade->Initialize();
	// フェードイン状態に設定
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// レーダーを生成（共通リソースを渡す）
	m_pRadar = std::make_unique<Radar>(resources);
	// レーダーの初期化（プレイヤーと敵マネージャーを渡す）
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// ボス登場演出を生成
	m_pBossAppear = std::make_unique<BossAppear>();
	// ボス登場演出を初期化
	m_pBossAppear->Initialize(m_pCommonResources);
	// ボス登場演出のSE音量を設定
	m_pBossAppear->SetSEVolume(m_SEvolume);
	// ブルームエフェクトのポストプロセス生成
	m_pBloom->CreatePostProcess(resources);
	// マウスカーソルの生成
	m_pMousePointer = std::make_unique<MousePointer>();
	// マウスカーソルの初期化
	m_pMousePointer->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
	// ゲーム終了前確認画面を生成
	m_pGameEndChecker = std::make_unique<GameEndChecker>();
	// 修了確認画面のUIを「ステージを選びなおす」に変更
	m_pGameEndChecker->SetIsStageSelect(true);
	// ゲーム終了前確認画面を初期化
	m_pGameEndChecker->Initialize(m_pCommonResources, Screen::WIDTH, Screen::HEIGHT);
}
/*
*	@brief 更新する
*	@details プレイシーンクラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void PlayScene::Update(float elapsedTime)
{

	// オーディオマネージャーの更新
	m_pCommonResources->GetAudioManager()->Update();
	// BGMを再生する
	m_pCommonResources->GetAudioManager()->PlaySound("PlayBGM", m_BGMvolume);
	// ゲーム終了前の確認が有効な場合
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// ゲーム終了前の確認処理を行う
		UpdateCheckGameEnd(elapsedTime);
		// UIに渡す更新情報をまとめた構造体を準備する
		UpdateContext ctx{};
		ctx.elapsedTime = elapsedTime;
		// マウスポインターを更新する
		m_pMousePointer->Update(ctx);
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
		// ゲームを更新する
		UpdateGame(elapsedTime);
	}
	// 画面遷移フェード処理
	m_pFade->Update(elapsedTime);
	// フェードアウトが終了したらシーン変更を可能にする
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
}
/*
*	@brief 描画する
*	@details プレイシーンクラスの描画
*	@param なし
*	@return なし
*/
void PlayScene::Render()
{
	using namespace DirectX::SimpleMath;

	// カメラからビュー行列を取得する
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	// カメラからプロジェクション行列を取得する
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	// 天球のワールド行列(サイズを10倍してプレイヤーを中心地とする)
	Matrix skyWorld = Matrix::Identity
		* Matrix::CreateScale(10.0f)
		* Matrix::CreateTranslation(m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// ワールド行列を初期化する
	Matrix world = Matrix::Identity;
	// オフスクリーンにオブジェクトを描画する
	m_pBloom->ChangeOffScreenRT();
	// 天球描画
	m_pSky->Render(view, projection, skyWorld);
	// 地面描画
	m_pStage->Render(view, projection, world);
	// 壁描画
	m_pWall->Render(view, projection);
	// 敵を描画する
	m_pEnemyManager->Render();
	// プレイヤーを描画する
	m_pPlayer->Render();
	// 弾を描画する
	m_pBulletManager->Render();
	// ブルームエフェクトをかける
	m_pBloom->PostProcess();
	// ゲーム終了前の確認処理が有効な場合
	if (m_pGameEndChecker->GetIsGameEndCheck())
	{
		// ゲーム終了前の確認画面を描画する
		m_pGameEndChecker->Render();
		// マウスポインターを描画する
		m_pMousePointer->Render();
	}
	else// ゲーム終了前の確認処理が無効な場合
	{
		// ゲーム開始プレイシーンに切り替えてから5秒間は猶予時間	
		if (m_time >= PlayScene::GAME_START_TIME)
		{
			// HPが10以下で危機状態描画
			if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Render();
			// UIを描画する
			for (int it = 0; it < m_pPlayerUI.size(); ++it)m_pPlayerUI[it]->Render();
			// 敵カウンターを描画する
			m_pEnemyCounter->Render();
			// レーダーを描画する
			m_pRadar->Render();
			// ボス登場演出が有効な場合、更新する
			if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Render();
		}
		else // ゲーム開始から5秒間
		{
			// 指示画像を表示
			m_pGoal->Render();
			// Wi-Fiローディングを表示
			m_pWifiLoading->Render();
		}
	}

	// フェードの描画
	m_pFade->Render();
#ifdef _DEBUG// デバッグモードでのみ実行
	// デバッグ情報を表示する
	auto debugString = m_pCommonResources->GetDebugString();
	// 接続しているWi-Fiの電波強度
	debugString->AddString("Power:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality());
	// 接続しているWi-FiのSSIDの長さ
	debugString->AddString("SSID:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());
	// プレイヤーHP
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());
	// プレイヤーHPの最大値
	debugString->AddString("MAXHP:%f", m_pPlayer->GetMaxPlayerHP());
#endif
}
/*
*	@brief 終了する
*	@details プレイシーンクラスの終了
*	@param なし
*	@return なし
*/
void PlayScene::Finalize()
{
	// 空を解放する
	m_pSky.reset();
}
/*
*	@brief ゲームの更新
*	@details ゲームの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void PlayScene::UpdateGame(float elapsedTime)
{
	// 経過時間を加算する
	m_time += elapsedTime;
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();
	// 壁の更新
	m_pWall->Update(elapsedTime);
	// 敵の更新
	m_pEnemyManager->Update(elapsedTime);
	// プレイヤーの更新
	m_pPlayer->Update(elapsedTime);
	// 各種UIに渡す情報をまとめた構造体
	UpdateContext ctx{};
	// 経過時間
	ctx.elapsedTime = elapsedTime;
	// ダッシュ時間
	ctx.dashStamina = m_pPlayer->GetDashTime();
	// 弾の数
	ctx.bulletPoint = float(m_pPlayer->GetBulletManager()->GetPlayerBulletCount());
	// 弾の更新
	m_pBulletManager->Update(elapsedTime);
	// ゲーム開始プレイシーンに切り替えてから5秒間は猶予時間
	if (m_time <= PlayScene::GAME_START_TIME)
	{
		// 指示画像を更新
		m_pGoal->Update(elapsedTime);
		// Wi-Fiローディングを更新
		m_pWifiLoading->Update(ctx);
	}
	else// 5秒以上経過したらゲームスタート
	{
		// HP更新
		m_pPlayer->SetMaxPlayerHP((float)(m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality()));
		// HP上限が設定されたので改めて一度だけHPを設定しなおす
		if (m_isResetHP == false)
		{
			// UIを更新する
			for (int it = 0; it < m_pPlayerUI.size(); ++it)
			{
				// プレイヤーHPだったら
				auto pHP = dynamic_cast<PlayerHP*>(m_pPlayerUI[it].get());
				// 体力をプレイヤーHPのUIに渡す
				if (pHP)pHP->SetMaxHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			}
			// 体力をプレイヤークラスに渡す
			m_pPlayer->SetPlayerHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			// HPを設定したのでフラグを立てる
			m_isResetHP = true;
		}
		// プレイヤーHPを取得
		ctx.playerHP = m_pPlayer->GetPlayerHP();
		// 体力が10以下になったら危機状態更新
		if (m_pPlayer->GetPlayerHP() <= PlayScene::CRISIS_HP)m_pCrisis->Update(elapsedTime);
		// 登録した分だけUIを更新する
		for (int it = 0; it < m_pPlayerUI.size(); ++it)	m_pPlayerUI[it]->Update(ctx);
		// レーダーを更新する
		m_pRadar->Update(elapsedTime);
		// 敵の総数を取得
		m_pEnemyCounter->SetEnemyIndex(m_pEnemyManager->GetEnemyIndex());
		// 現在の敵の数を取得
		m_pEnemyCounter->SetNowEnemy(m_pEnemyManager->GetEnemySize());
		// 敵カウンターの更新
		m_pEnemyCounter->Update(elapsedTime);
		// 以下の条件を満たす場合、フェードアウト状態にする
		if (m_pPlayer->GetPlayerHP() <= 0.0f ||// プレイヤーのHPが0以下か、
			(m_pEnemyManager->GetEnemySize() <= 0 &&// 敵がいなくて
				m_pEnemyManager->GetisBorned() &&// 生成が完了していて
				m_pEnemyManager->GetIsBossAlive() == false))// ボスがいないなら
		{
			// 待ち時間を加算する
			m_waitTime += elapsedTime;
		}
		// 待ち時間が1秒以上ならフェードアウト状態にする
		if (m_waitTime >= PlayScene::WAIT_TIME)	m_pFade->SetState(Fade::FadeState::FadeOut);
		// ボス登場演出を更新する
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Update(elapsedTime);
	}
}
/*
*	@brief ゲーム終了前の確認処理
*	@details ゲーム終了前の確認処理を行う
*	@param elapsedTime	経過時間
*	@return なし
*/
void PlayScene::UpdateCheckGameEnd(float elapsedTime)
{
	// マウスの状態を取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// UIの更新に必要な情報をまとめた構造体
	UpdateContext ctx{};
	// 経過時間を渡す
	ctx.elapsedTime = elapsedTime;
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
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// 終了フラグが立ってるならゲームを終了する
		if (m_pGameEndChecker->GetIsEndGame())//PostQuitMessage(0);
			return IScene::SceneID::STAGESELECT;
		// BGMを停止する
		m_pCommonResources->GetAudioManager()->StopSound("PlayBGM");
		// プレイヤーのHPが0以下なら
		if (m_pPlayer->GetPlayerHP() <= 0.0f)
		{
			// ゲームオーバーシーンへ遷移
			return IScene::SceneID::GAMEOVER;
		}
		else
		{
			// 敵がいないならクリアシーンへ遷移
			return IScene::SceneID::CLEAR;
		}
	}
	// シーン変更がない場合は何もしない
	return IScene::SceneID::NONE;
}