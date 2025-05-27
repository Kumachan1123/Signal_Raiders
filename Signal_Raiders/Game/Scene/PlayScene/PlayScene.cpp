/*
*	@file	PlayScene.cpp
*	@brief	プレイシーンクラス
*/
#include "pch.h"
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
	assert(resources);// リソースがnullptrでないことを確認
	m_pCommonResources = resources;// 共通リソースを取得
	auto DR = m_pCommonResources->GetDeviceResources();// デバイスリソースを取得
	m_pSettingData = std::make_unique<SettingData>();// 設定データクラスを生成する
	m_pSettingData->Load();// 設定データを取得する
	m_BGMvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGMの音量を設定する
	m_SEvolume = PlayScene::VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SEの音量を設定する
	m_mouseSensitivity = static_cast<float>(m_pSettingData->GetMouseSensitivity());// マウス感度を設定する
	m_pStage = std::make_unique<Stage>();// 地面（ステージ生成）
	m_pStage->Initialize(resources);// 地面を初期化する
	m_pWall = std::make_unique<Wall>(resources);// 壁生成
	m_pWall->Create(DR);// 壁を生成する
	m_sky = std::make_unique<Sky>(m_stageNumber);// 空生成
	m_sky->Initialize(resources);// 空を初期化する
	m_pPlayer = std::make_unique<Player>(resources);// プレイヤーを初期化する
	m_pEnemyManager = std::make_unique<EnemyManager>(resources);// 敵全体を初期化する
	m_pEnemyManager->SetStageNumber(m_stageNumber);// ステージ番号を設定する
	m_pPlayer->SetVolume(m_SEvolume);// プレイヤーが出す効果音の音量を設定する
	if (m_SEvolume > m_BGMvolume)m_pPlayer->SetVolumeCorrection(m_SEvolume - m_BGMvolume);// プレイヤーが出す効果音の音量を補正する
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);// マウス感度を設定する
	m_pPlayer->Initialize(m_pEnemyManager.get());// プレイヤーを初期化する
	m_pEnemyManager->Initialize(m_pPlayer.get());// 敵を初期化する
	m_pEnemyManager->SetSEVolume(m_SEvolume);// 敵が出す効果音の音量を設定する
	m_pEnemyManager->SetWall(m_pWall.get());// 敵に壁の情報を渡す
	m_pBulletManager = std::make_unique<BulletManager>(resources);// 弾マネージャーを初期化する
	m_pBulletManager->Initialize(m_pPlayer.get(), m_pEnemyManager.get());// 弾マネージャーを初期化する
	m_pPlayer->SetBulletManager(m_pBulletManager.get());// プレイヤーと敵マネージャーに弾マネージャーを渡す
	m_pEnemyManager->SetBulletManager(m_pBulletManager.get());// 敵マネージャーに弾マネージャーを渡す
	m_pEnemyCounter = std::make_unique<EnemyCounter>();// 敵カウンターを生成する
	m_pEnemyCounter->Initialize(resources);// 敵カウンターを初期化する
	m_pGoal = std::make_unique<Goal>(m_pCommonResources);// 指示画像を生成する
	m_pGoal->Create(DR);// 指示画像を生成する
	m_pCrisis = std::make_unique<Crisis>(m_pCommonResources);// 危険状態表示を生成する
	m_pCrisis->Create(DR);// 危険状態表示を生成する
	m_pWifiLoading = std::make_unique<WifiLoading>();// Wi-Fiローディング(ゲーム開始から５秒間のみ表示するUI）を生成する
	m_pWifiLoading->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);// Wi-Fiローディングを初期化する
	// プレイ中に表示されるUIたちを登録
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayerHP>()));		// プレイヤーHP
	m_pPlayerUI.push_back(std::move(std::make_unique<DashGauge>()));	// ダッシュゲージ
	m_pPlayerUI.push_back(std::move(std::make_unique<BulletGauge>()));	// 弾ゲージ
	m_pPlayerUI.push_back(std::move(std::make_unique<Reticle>()));		// 照準
	m_pPlayerUI.push_back(std::move(std::make_unique<PlayGuide>()));	// 操作説明
	for (int it = 0; it < m_pPlayerUI.size(); ++it)// 登録した分だけ初期化する
		m_pPlayerUI[it]->Initialize(m_pCommonResources, Screen::UI_WIDTH, Screen::UI_HEIGHT);// UIを初期化
	m_pFade = std::make_unique<Fade>(m_pCommonResources);// フェードを生成する
	m_pFade->Initialize();// フェードの初期化
	m_pFade->SetState(Fade::FadeState::FadeIn);// フェードイン状態にする
	m_pRadar = std::make_unique<Radar>(resources);// レーダーを初期化する
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemyManager.get());// レーダーを初期化する
	m_pBossAppear = std::make_unique<BossAppear>();// ボス登場演出を生成する
	m_pBossAppear->Initialize(m_pCommonResources);// ボス登場演出を初期化する
	m_pBossAppear->SetSEVolume(m_SEvolume);// ボス登場演出のSE音量を設定する
	m_pBloom->CreatePostProcess(resources);// ブルームエフェクトの生成する
}
/*
*	@brief 更新する
*	@details プレイシーンクラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void PlayScene::Update(float elapsedTime)
{
	m_time += elapsedTime;// 経過時間
	m_pCommonResources->GetAudioManager()->PlaySound("PlayBGM", m_BGMvolume);// 二重再生しない
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();// カメラが向いている方向を取得する
	m_pWall->Update(elapsedTime);// 壁の更新
	m_pCommonResources->GetAudioManager()->Update();// オーディオマネージャーの更新
	m_pEnemyManager->Update(elapsedTime);// 敵の更新
	m_pPlayer->Update(elapsedTime);// プレイヤーの更新
	UpdateContext ctx;// 各種UIに渡す情報をまとめた構造体
	ctx.elapsedTime = elapsedTime;// 経過時間
	ctx.dashStamina = m_pPlayer->GetDashTime();// ダッシュ時間
	ctx.bulletPoint = float(m_pPlayer->GetBulletManager()->GetPlayerBulletCount());// 弾の数
	m_pBulletManager->Update(elapsedTime);// 弾の更新
	if (m_time <= PlayScene::GAME_START_TIME)// ゲーム開始から5秒間は
	{
		m_pGoal->Update(elapsedTime);// 指示画像を更新
		m_pWifiLoading->Update(ctx);// Wi-Fiローディングを更新
	}
	else// 5秒以上経過したら
	{
		m_pPlayer->SetMaxPlayerHP((float)(m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality()));// HP更新
		if (m_isResetHP == false)// HP上限が設定されたので改めて一度だけHPを設定しなおす
		{
			for (int it = 0; it < m_pPlayerUI.size(); ++it)// UIを更新する
			{
				auto pHP = dynamic_cast<PlayerHP*>(m_pPlayerUI[it].get());// プレイヤーHPだったら
				if (pHP)pHP->SetMaxHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());// 体力をプレイヤーHPのUIに渡す
			}
			m_pPlayer->SetPlayerHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());// 体力をプレイヤークラスに渡す
			m_isResetHP = true;// HPを設定したのでフラグを立てる
		}
		ctx.playerHP = m_pPlayer->GetPlayerHP();// プレイヤーHPを取得
		if (m_pPlayer->GetPlayerHP() <= PlayScene::CRISIS_HP)m_pCrisis->Update(elapsedTime);// 体力が10以下になったら危機状態更新
		for (int it = 0; it < m_pPlayerUI.size(); ++it)// 登録した分だけ更新する
			m_pPlayerUI[it]->Update(ctx);// UIを更新する
		m_pRadar->Update(elapsedTime);// レーダーを更新する
		m_pEnemyCounter->SetEnemyIndex(m_pEnemyManager->GetEnemyIndex());// 敵の総数を取得
		m_pEnemyCounter->SetNowEnemy(m_pEnemyManager->GetEnemySize());// 現在の敵の数を取得
		m_pEnemyCounter->Update(elapsedTime);// 敵カウンターの更新
		if (m_pPlayer->GetPlayerHP() <= 0.0f ||// プレイヤーのHPが0以下か、
			(m_pEnemyManager->GetEnemySize() <= 0 &&// 敵がいなくて
				m_pEnemyManager->GetisBorned() &&// 生成が完了していて
				m_pEnemyManager->GetIsBossAlive() == false))// ボスがいないなら
		{
			m_waitTime += elapsedTime;// 待ち時間を加算する
		}
		if (m_waitTime >= PlayScene::WAIT_TIME)// 待ち時間が1秒以上なら
			m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウト状態にする
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Update(elapsedTime);// ボス登場演出を更新する
	}
	m_pFade->Update(elapsedTime);// 画面遷移フェード処理
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;// フェードアウトが終了したらシーン変更を可能にする
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

	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();// カメラからビュー行列を取得する
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();// カメラからプロジェクション行列を取得する
	Matrix skyWorld = Matrix::Identity * Matrix::CreateScale(10);;// スカイボックスのワールド行列(サイズ10倍)
	Matrix world = Matrix::Identity;// ワールド行列
	m_pBloom->ChangeOffScreenRT();// オフスクリーンにオブジェクトを描画する
	m_sky->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());// 天球描画
	m_pStage->Render(view, projection, world, Vector3(0, 0, 0));// 地面描画
	m_pWall->Render(view, projection);// 壁描画
	m_pEnemyManager->Render();// 敵を描画する
	m_pPlayer->Render();// プレイヤーを描画する
	m_pBulletManager->Render();// 弾を描画する
	m_pBloom->PostProcess();// ブルームエフェクトをかける
	if (m_time >= PlayScene::GAME_START_TIME)// ゲーム開始から5秒以上経過したら
	{
		if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Render();// HPが10以下で危機状態描画
		for (int it = 0; it < m_pPlayerUI.size(); ++it)m_pPlayerUI[it]->Render();// UIを描画する
		m_pEnemyCounter->Render();// 敵カウンターを描画する
		m_pRadar->Render();// レーダーを描画する
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Render();// ボス登場演出が有効な場合、更新する
	}
	else // ゲーム開始から5秒間
	{
		m_pGoal->Render();// 指示画像を表示
		m_pWifiLoading->Render();// Wi-Fiローディングを表示
	}
	m_pFade->Render();// フェードの描画
#ifdef _DEBUG
	auto debugString = m_pCommonResources->GetDebugString();// デバッグ情報を表示する
	debugString->AddString("Power:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality());// 接続しているWi-Fiの電波強度
	debugString->AddString("SSID:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());// 接続しているWi-FiのSSIDの長さ
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());// プレイヤーHP
	debugString->AddString("MAXHP:%f", m_pPlayer->GetMaxPlayerHP());// プレイヤーHPの最大値
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
	m_sky.reset();// 空を解放する
}
/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーン変更の有無(NONE::何もしない、GAMEOVER:ゲームオーバー、CLEAR:クリア)
*/
IScene::SceneID PlayScene::GetNextSceneID() const
{
	if (m_isChangeScene)// シーン変更がある場合
	{
		m_pCommonResources->GetAudioManager()->StopSound("PlayBGM");// BGMを停止する
		if (m_pPlayer->GetPlayerHP() <= 0.0f)return IScene::SceneID::GAMEOVER;// プレイヤーのHPが0以下ならゲームオーバーシーンへ
		else return IScene::SceneID::CLEAR;// 敵がいないならクリアシーンへ
	}
	return IScene::SceneID::NONE;// シーン変更がない場合は何もしない
}

