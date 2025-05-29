/*
*	@file Game.cpp
*	@brief ゲームのメインクラス
*/
#include <pch.h>
#include "Game.h"
/*
*	@brief ゲームを終了する
*	@detail ゲームを終了する関数(Main.cppから呼び出される)
*	@param なし
*	@return なし
*/
extern void ExitGame() noexcept;



using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
	: m_deviceResources{}
	, m_timer{}
	, m_commonStates{}
	, m_pCommonResources{}
	, m_debugString{}
	, m_inputManager{}
	, m_sceneManager{}
	, m_audioManager{}
	, m_modelManager{}
	, m_textureManager{}
	, m_fullscreen{ FALSE }
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();// デバイスリソースを作成する
	m_deviceResources->RegisterDeviceNotify(this);// デバイス通知を登録する
}

// Initialize the Direct3D resources required to run.
/*
*	@brief ゲームの初期化
*	@detail ゲームの実行に必要なDirect3Dリソースを初期化
*	@param window ウィンドウハンドル
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Game::Initialize(HWND window, int width, int height)
{
	using namespace DirectX;
	// デバイスリソース関連を設定する
	m_deviceResources->SetWindow(window, width, height);// ウィンドウを設定する
	m_deviceResources->CreateDeviceResources();// デバイスリソースを作成する
	CreateDeviceDependentResources();// デバイス依存リソースを作成する
	m_deviceResources->CreateWindowSizeDependentResources();// ウィンドウサイズ依存リソースを作成する
	CreateWindowSizeDependentResources();// ウィンドウサイズ依存リソースを作成する
	auto device = m_deviceResources->GetD3DDevice();// デバイスを取得する
	auto context = m_deviceResources->GetD3DDeviceContext();// デバイスコンテキストを取得する
	m_inputManager = std::make_unique<mylib::InputManager>(window);// 入力マネージャを作成する
	m_commonStates = std::make_unique<CommonStates>(device);// コモンステートを作成する
	m_debugString = std::make_unique<mylib::DebugString>(// デバッグ文字列を作成する
		device,		// デバイス
		context,	// デバイスコンテキスト
		L"Resources/Fonts/SegoeUI_18.spritefont"// フォントファイルのパス
	);
	m_audioManager = std::make_unique<AudioManager>();// オーディオマネージャーを作成する
	m_pCommonResources = std::make_unique<CommonResources>();// 共通リソースを作成する
	m_modelManager = std::make_unique<ModelManager>();// モデルマネージャを作成する
	m_textureManager = std::make_unique<TextureManager>();// テクスチャマネージャを作成する
	m_pCommonResources->Initialize(// シーンへ渡す共通リソースを設定する
		&m_timer,				// タイマー
		m_deviceResources.get(),// デバイスリソース
		m_commonStates.get(),	// コモンステート
		m_debugString.get(),	// デバッグ文字列
		m_inputManager.get(),	// 入力マネージャ
		m_audioManager.get(),	// オーディオマネージャ
		m_modelManager.get(),	// モデルマネージャ
		m_textureManager.get()	// テクスチャマネージャ
	);

	m_modelManager->SetCommonResources(m_pCommonResources.get());// モデルマネージャに共通リソースを設定する
	m_modelManager->Initialize();// モデルマネージャを初期化する
	m_textureManager->Initialize(m_pCommonResources.get());
	m_sceneManager = std::make_unique<SceneManager>();	// シーンマネージャを作成する
	m_sceneManager->Initialize(m_pCommonResources.get());// シーンマネージャを初期化する
	ShowCursor(FALSE);//カーソルを見えるようにする
}

#pragma region Frame Update
/*
*	@brief ゲームループを実行する
*	@detail ゲームループを実行する関数
*	@param なし
*	@return なし
*/
void Game::Tick()
{
	m_timer.Tick([&]() { Update(m_timer); });// タイマーを更新し、Update関数を呼び出す
	Render();// 描画処理を呼び出す
}
/*
*	@brief ゲームの更新処理
*	@detail ゲームの更新処理を行う関数
*	@param timer タイマー
*	@return なし
*/
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());// 経過時間を取得する
	m_inputManager->Update();// 入力マネージャを更新する
	const auto& keyboardState = m_inputManager->GetKeyboardState();// キーボードステートを取得する
	if (keyboardState.Escape) ExitGame();// 「ECS」キーで終了する
	m_sceneManager->Update(elapsedTime);// シーンマネージャを更新する
}
#pragma endregion

#pragma region Frame Render
/*
*	@brief ゲームの描画処理
*	@detail ゲームの描画処理を行う関数
*	@param なし
*	@return なし
*/
void Game::Render()
{
	if (m_timer.GetFrameCount() == 0)return;// 最初のUpdate前は何も描画しない
	Clear();// 画面をクリアする
	m_deviceResources->PIXBeginEvent(L"Render");// PIXイベントを開始する
	auto context = m_deviceResources->GetD3DDeviceContext();// デバイスコンテキストを取得する
	UNREFERENCED_PARAMETER(context);// デバイスコンテキストの未使用警告を抑制する
	m_sceneManager->Render();// シーンマネージャを描画する
#ifdef _DEBUG
	m_debugString->Render(m_commonStates.get());// デバッグ文字列を描画する
	m_debugString->AddString("fps : %d", m_timer.GetFramesPerSecond());// デバッグ文字列を作成する：FPS
#endif
	m_deviceResources->PIXEndEvent();// PIXイベントを終了する
	m_deviceResources->Present();// 新しいフレームを表示する
}

/*
*	@brief 画面をクリアする
*	@detail 画面をクリアする関数
*	@param なし
*	@return なし
*/
void Game::Clear()
{
	using namespace DirectX;
	m_deviceResources->PIXBeginEvent(L"Clear");// PIXイベントを開始する
	auto context = m_deviceResources->GetD3DDeviceContext();// デバイスコンテキストを取得する
	auto renderTarget = m_deviceResources->GetRenderTargetView();// レンダーターゲットビューを取得する
	auto depthStencil = m_deviceResources->GetDepthStencilView();// 深度ステンシルビューを取得する
	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);// レンダリングターゲットビューをクリアする
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);// 深度ステンシルビューをクリアする
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);// レンダーターゲットと深度ステンシルビューを設定する
	auto const viewport = m_deviceResources->GetScreenViewport();// スクリーンビューポートを取得する
	context->RSSetViewports(1, &viewport); // ビューポートを設定する
	m_deviceResources->PIXEndEvent(); // PIXイベントを終了する
}
#pragma endregion

#pragma region Message Handlers
/*
*	@brief ゲームがアクティブ化されたときの処理
*	@detail ゲームがアクティブウィンドウになったときの処理
*	@param なし
*	@return なし
*/
void Game::OnActivated() {/*do nothing*/ }
/*
*	@brief ゲームが非アクティブ化されたときの処理
*	@detail ゲームが非アクティブウィンドウになったときの処理
*	@param なし
*	@return なし
*/
void Game::OnDeactivated() {/*do nothing*/ }
/*
*	@brief ゲームが一時停止されたときの処理
*	@detail ゲームが一時停止されたときの処理
*	@param なし
*	@return なし
*/
void Game::OnSuspending() {/*do nothing*/ }
/*
*	@brief ゲームが再開されたときの処理
*	@detail ゲームが再開されたときの処理
*	@param なし
*	@return なし
*/
void Game::OnResuming() { m_timer.ResetElapsedTime(); }// タイマーをリセットする
/*
*	@brief ウィンドウが移動されたときの処理
*	@detail ウィンドウが移動されたときの処理
*	@param なし
*	@return なし
*/
void Game::OnWindowMoved()
{
	auto const r = m_deviceResources->GetOutputSize();// ウィンドウのサイズを取得する
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);// ウィンドウサイズが変更されたことを通知する
	BOOL fullscreen = FALSE;// フルスクリーンか調べる
	m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);// フルスクリーン状態を取得する
	if (m_fullscreen != fullscreen)// フルスクリーンが解除されてしまった時の処理
	{
		m_fullscreen = fullscreen;// フルスクリーン状態を更新する
		m_deviceResources->CreateWindowSizeDependentResources();// ウィンドウサイズ依存リソースを作成する
	}
}
/*
*	@brief ディスプレイの変更があったときの処理
*	@detail ディスプレイの変更があったときの処理
*	@param なし
*	@return なし
*/
void Game::OnDisplayChange() { m_deviceResources->UpdateColorSpace(); }// カラースペースを更新する
/*
*	@brief ウィンドウサイズが変更されたときの処理
*	@detail ウィンドウサイズが変更されたときの処理
*	@param width 新しいウィンドウの幅
*	@param height 新しいウィンドウの高さ
*	@return なし
*/
void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))return;// ウィンドウサイズが変更されていない場合は何もしない
	CreateWindowSizeDependentResources();// ウィンドウサイズ依存リソースを作成する
}

/*
*	@brief デフォルトのウィンドウサイズを取得する
*	@detail デフォルトのウィンドウサイズを取得する関数
*	@param width デフォルトのウィンドウの幅
*	@param height デフォルトのウィンドウの高さ
*	@return なし
*/
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
	width = Screen::WIDTH;// デフォルトのウィンドウの幅を設定
	height = Screen::HEIGHT;// デフォルトのウィンドウの高さを設定
}
#pragma endregion

#pragma region Direct3D Resources
/*
*	@brief デバイス依存リソースを作成する
*	@detail デバイス依存リソースを作成する関数
*	@param なし
*	@return なし
*/
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();// デバイスを取得する
	UNREFERENCED_PARAMETER(device);// デバイスの未使用警告を抑制する
}
/*
*	@brief ウィンドウサイズ依存リソースを作成する
*	@detail ウィンドウサイズ依存リソースを作成する関数
*	@param なし
*	@return なし
*/
void Game::CreateWindowSizeDependentResources() {/*do nothing*/ }
/*
*	@brief デバイスが失われたときの処理
*	@detail デバイスが失われたときの処理を行う関数
*	@param なし
*	@return なし
*/
void Game::OnDeviceLost() {/*do nothing*/ }
/*
*	@brief デバイスが復元されたときの処理
*	@detail デバイス・ウィンドウサイズ依存リソースを再作成する関数
*	@param なし
*	@return なし
*/
void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();// デバイス依存リソースを作成
	CreateWindowSizeDependentResources();// ウィンドウサイズ依存リソースを作成
}
/*
*	@brief フルスクリーン状態を設定する
*	@detail フルスクリーン状態を設定する関数
*	@param value フルスクリーン状態（TRUE: フルスクリーン、FALSE: ウィンドウモード）
*	@return なし
*/
void Game::SetFullscreenState(BOOL value)
{
	m_fullscreen = value;// フルスクリーン状態を更新
	m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);// フルスクリーン状態を設定
	if (value) m_deviceResources->CreateWindowSizeDependentResources();// フルスクリーン状態に応じてウィンドウサイズ依存リソースを作成
}
#pragma endregion
