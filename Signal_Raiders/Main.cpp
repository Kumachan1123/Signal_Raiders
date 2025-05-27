/*
*	@file Main.cpp
*	@brief ゲームの画面を表示するメイン関数
*/
// ヘッダーファイル
#include <pch.h>
#include "Game.h"
#include <Libraries/MyLib/MemoryLeakDetector.h>

using namespace DirectX;

#ifdef __clang__// clangコンパイラを使用している場合
#pragma clang diagnostic ignored "-Wcovered-switch-default"// デフォルトのケースがカバーされていないという警告を無視
#pragma clang diagnostic ignored "-Wswitch-enum"// 列挙型のswitch文に関する警告を無視
#endif
#pragma warning(disable : 4061)// enum値がswitch文でカバーされていないという警告を無視
// ウィンドウスタイルの定義（タイトルバー、システムメニュー、最小化ボタン、最大化ボタンを含む）
#define WS_MY_WINDOW    (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

namespace
{
	std::unique_ptr<Game> g_game;// ゲームオブジェクトのグローバルポインター
}
// アプリケーション名
LPCWSTR g_szAppName = L"シグナルレイダーズ";
// Windowsプロシージャの宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// ゲーム終了処理の宣言
void ExitGame() noexcept;

// NVIDIAとAMDのグラフィックカードの最適化設定
extern "C"// C言語リンケージを使用してエクスポートするための宣言
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;// NVIDIA Optimusを有効にするためのフラグ
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; // AMDの高性能モードを要求するためのフラグ
}

// アプリのエントリーポイント（WinMain）
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#if defined(_DEBUG)// デバッグビルドの場合
	mylib::MemoryLeakDetector();// メモリリーク検出器を初期化
#endif
	UNREFERENCED_PARAMETER(hPrevInstance);//未使用警告非表示
	UNREFERENCED_PARAMETER(lpCmdLine);// 未使用警告非表示
	if (!XMVerifyCPUSupport())return 1;// CPUがDirectX Mathライブラリをサポートしていない場合は終了
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);// COMライブラリを初期化(マルチスレッドベースの初期化を使用)
	if (FAILED(hr))	return 1;// COMの初期化に失敗した場合は終了
	g_game = std::make_unique<Game>();// ゲームオブジェクトを作成
	static bool s_fullscreen = true;// フルスクリーンモードの初期値を設定(true: フルスクリーン、false: ウィンドウモード)
#ifdef _DEBUG// デバッグビルドの場合
	// 画面モード選択
	if (MessageBox(NULL, L"フルスクリーンにしますか？", L"画面モード設定", MB_YESNO) == IDYES)// ユーザーが「はい」を選択した場合
		s_fullscreen = true;// フルスクリーンモードに設定
	else	// ユーザーが「いいえ」を選択した場合
		s_fullscreen = false;// ウィンドウモードに設定
#endif
	// ウィンドウクラスを登録し、ウィンドウを作成する
	{

		WNDCLASSEXW wcex = {};// Windowsのウィンドウクラスを定義する構造体
		wcex.cbSize = sizeof(WNDCLASSEXW);// 構造体のサイズを設定
		wcex.style = CS_HREDRAW | CS_VREDRAW; // ウィンドウのスタイルを設定（水平および垂直のリサイズ時に再描画）
		wcex.lpfnWndProc = WndProc; // ウィンドウプロシージャの関数ポインタを設定
		wcex.hInstance = hInstance; // アプリケーションのインスタンスハンドルを設定
		wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON"); // アプリケーションのアイコンをロード
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW); // デフォルトのカーソルを設定
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); // ウィンドウの背景色を設定
		wcex.lpszClassName = L"_3DPGWindowClass";// ウィンドウクラス名を設定
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON"); // 小さいアイコンをロード
		if (!RegisterClassExW(&wcex)) 	return 1;// ウィンドウクラスの登録に失敗した場合は終了
		// ウィンドウの初期化
		int w, h;
		if (s_fullscreen == true)// フルスクリーンの場合
		{
			w = 1920;// 幅を1920に設定
			h = 1080; // 高さを1080に設定
		}
		else g_game->GetDefaultSize(w, h);// ウィンドウモードの場合はゲームのデフォルトサイズを取得
		RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };// ウィンドウのクライアント領域の矩形を定義
		AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);// ウィンドウのクライアント領域の矩形を調整
		HWND hwnd = CreateWindowExW(// ウィンドウを作成
			0,						// 拡張スタイルなし
			L"_3DPGWindowClass",	// ウィンドウクラス名
			g_szAppName,			// アプリケーション名
			WS_MY_WINDOW,			// ウィンドウスタイル（タイトルバー、システムメニュー、最小化ボタン、最大化ボタンを含む）
			CW_USEDEFAULT,			// ウィンドウ位置（デフォルト位置）
			CW_USEDEFAULT,			// ウィンドウ位置（デフォルト位置）
			rc.right - rc.left,		// ウィンドウの幅（調整後の矩形の幅）
			rc.bottom - rc.top,		// ウィンドウの高さ（調整後の矩形の高さ）
			nullptr,				// 親ウィンドウなし
			nullptr,				// メニューハンドルなし
			hInstance,				// アプリケーションのインスタンスハンドル
			nullptr					// アプリケーションのパラメータなし
		);
		if (!hwnd)	return 1;// ウィンドウの作成に失敗した場合は終了
		ShowWindow(hwnd, nCmdShow);// ウィンドウを表示する
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));// ウィンドウのユーザーデータにゲームオブジェクトのポインタを設定
		GetClientRect(hwnd, &rc);// ウィンドウのクライアント領域の矩形を取得
		g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);// ゲームの初期化を行う
		if (s_fullscreen) g_game->SetFullscreenState(TRUE);// フルスクリーンモードの場合、ゲームのフルスクリーン状態を設定
	}
	MSG msg = {};// メッセージ構造体を初期化
	while (WM_QUIT != msg.message)// メッセージがWM_QUITでない限りループを続ける
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))// メッセージがキューにあるかを確認し、あれば取り出して処理
		{
			TranslateMessage(&msg);// メッセージを変換
			DispatchMessage(&msg); // メッセージをディスパッチ
		}
		else
			g_game->Tick();		// メッセージがない場合はゲームのTick関数を呼び出して更新処理を行う
	}
	if (s_fullscreen) g_game->SetFullscreenState(FALSE);// フルスクリーンモードの場合、ゲームのフルスクリーン状態を解除
	g_game.reset(); // ゲームオブジェクトをリセットしてメモリを解放
	CoUninitialize(); // COMライブラリの終了処理
	return static_cast<int>(msg.wParam);// メッセージのwParamを返す（アプリケーションの終了コード）
}
/*
*	@brief ウィンドウプロシージャ
*	@details ウィンドウに送られるメッセージを処理する関数
*	@param hWnd ウィンドウハンドル
*	@param message メッセージの識別子
*	@param wParam メッセージに関連する追加情報
*	@param lParam メッセージに関連する追加情報
*	@return メッセージの処理結果
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 状態を保持するための静的変数
	static bool s_in_sizemove = false;// ウィンドウのサイズ変更中かどうかを示すフラグ
	static bool s_in_suspend = false; // アプリケーションがサスペンド状態かどうかを示すフラグ
	static bool s_minimized = false;  // アプリケーションが最小化されているかどうかを示すフラグ
	static bool s_fullscreen = false; // アプリケーションがフルスクリーンモードかどうかを示すフラグ

	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));// ウィンドウのユーザーデータからゲームオブジェクトのポインタを取得

	switch (message)// メッセージの種類に応じて処理を分岐
	{
	case WM_PAINT:// ウィンドウの再描画要求
		if (s_in_sizemove && game)game->Tick();// サイズ変更中は自前で描画
		else								   // サイズ変更中でない場合は 
		{
			PAINTSTRUCT ps;	// 描画のための構造体を初期化
			std::ignore = BeginPaint(hWnd, &ps); // ウィンドウの描画を開始
			EndPaint(hWnd, &ps); // 描画を終了
		}
		break;// switch文の終了
	case WM_DISPLAYCHANGE:// ディスプレイの設定が変更されたとき
		if (game)game->OnDisplayChange();// ゲームオブジェクトにディスプレイ変更を通知
		break;// switch文の終了
	case WM_MOVE:// ウィンドウが移動されたとき
		if (game)game->OnWindowMoved();// ゲームオブジェクトにウィンドウ移動を通知
		break;// switch文の終了
	case WM_ENTERSIZEMOVE:// ウィンドウのサイズ変更が開始されたとき
		s_in_sizemove = true;// サイズ変更中フラグを設定
		break;// switch文の終了
	case WM_EXITSIZEMOVE:// ウィンドウのサイズ変更が終了したとき
		s_in_sizemove = false;// サイズ変更中フラグを解除
		if (game)// ゲームオブジェクトが存在する場合
		{
			RECT rc;// ウィンドウのクライアント領域の矩形を定義
			GetClientRect(hWnd, &rc);// ウィンドウのクライアント領域の矩形を取得
			game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);// ゲームオブジェクトにウィンドウサイズ変更を通知
		}
		break;// switch文の終了
	case WM_GETMINMAXINFO:// ウィンドウの最小および最大サイズを取得するためのメッセージ
		if (lParam)// lParamがnullptrでない場合
		{
			auto info = reinterpret_cast<MINMAXINFO*>(lParam); // lParamをMINMAXINFO構造体にキャスト
			info->ptMinTrackSize.x = 320; // 最小トラックサイズの幅を320に設定
			info->ptMinTrackSize.y = 180; // 最小トラックサイズの高さを180に設定
		}
		break;// switch文の終了
	case WM_ACTIVATEAPP:
		if (game)// ゲームオブジェクトが存在する場合
		{
			if (wParam)game->OnActivated();	// アプリケーションがアクティブになった場合はOnActivatedを呼び出す
			else game->OnDeactivated();		// アプリケーションが非アクティブになった場合はOnDeactivatedを呼び出す
		}
		Keyboard::ProcessMessage(message, wParam, lParam);			// キーボードのメッセージを処理
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);	// マウスのメッセージを処理
		break;// switch文の終了
	case WM_POWERBROADCAST: // システムの電源状態が変更されたときに送信されるメッセージ
		switch (wParam) // wParamの値に応じて処理を分岐
		{
		case PBT_APMQUERYSUSPEND:// システムがサスペンド状態に入る前に送信されるメッセージ
			if (!s_in_suspend && game)	game->OnSuspending();// ゲームオブジェクトにサスペンドを通知
			s_in_suspend = true; // サスペンド状態フラグを設定
			return TRUE; // メッセージの処理が成功したことを示すTRUEを返す
		case PBT_APMRESUMESUSPEND: // システムがサスペンド状態から復帰したときに送信されるメッセージ
			if (!s_minimized)// ウィンドウが最小化されていない場合
			{
				if (s_in_suspend && game)game->OnResuming();// ゲームオブジェクトに復帰を通知
				s_in_suspend = false;// サスペンド状態フラグを解除
			}
			return TRUE;// メッセージの処理が成功したことを示すTRUEを返す
		}
		break;// switch文の終了
	case WM_DESTROY:// ウィンドウが破棄されるときに送信されるメッセージ
		PostQuitMessage(0);// メッセージキューにWM_QUITメッセージをポストして、アプリケーションを終了するように指示
		break;// switch文の終了
	case WM_ACTIVATE:		// ウィンドウがアクティブか非アクティブになったとき
	case WM_INPUT:			// DirectInputやRawInputを使用している
	case WM_MOUSEMOVE:		// マウスが移動したとき
	case WM_LBUTTONDOWN:	// 左ボタンが押されたとき
	case WM_LBUTTONUP:		// 左ボタンが離されたとき
	case WM_RBUTTONDOWN:	// 右ボタンが押されたとき
	case WM_RBUTTONUP:		// 右ボタンが離されたとき
	case WM_MBUTTONDOWN:	// 中ボタンが押されたとき
	case WM_MBUTTONUP:		// 中ボタンが離されたとき
	case WM_MOUSEWHEEL:		// マウスホイールが回転したとき
	case WM_XBUTTONDOWN:	// サイドボタンが押されたとき
	case WM_XBUTTONUP:		// サイドボタンが離されたとき
	case WM_MOUSEHOVER:		// マウスがホバー状態になったとき
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);// マウスのメッセージを処理
		break;// switch文の終了
	case WM_KEYDOWN:		// キーが押されたとき
	case WM_KEYUP:			// キーが離されたとき
	case WM_SYSKEYUP:		// システムキーが離されたとき
		Keyboard::ProcessMessage(message, wParam, lParam);// キーボードのメッセージを処理
		break;// switch文の終了
	case WM_SYSKEYDOWN:		// システムキーが押されたとき
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// Alt+Enterキーが押された場合、フルスクリーンモードを切り替える
			if (s_fullscreen)// フルスクリーンモードの場合
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_MY_WINDOW);// ウィンドウスタイルを通常のウィンドウスタイルに設定
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);			// 拡張スタイルを通常のウィンドウスタイルに設定
				int width = 1920; // 幅を1920に設定
				int height = 1080; // 高さを1080に設定
				if (game)game->GetDefaultSize(width, height);// ゲームオブジェクトが存在する場合、デフォルトのウィンドウサイズを取得
				ShowWindow(hWnd, SW_SHOWNORMAL);// ウィンドウを通常の状態で表示
				// ウィンドウを元のサイズに戻すときにサイズを補正する
				RECT rc{};// ウィンドウのクライアント領域の矩形を初期化
				AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);// ウィンドウのクライアント領域の矩形を調整
				SetWindowPos(// ウィンドウの位置とサイズを設定
					hWnd, HWND_TOP,											// ウィンドウを最前面に表示
					0, 0,													// ウィンドウの位置を(0, 0)に設定
					width + rc.right - rc.left, height + rc.bottom - rc.top,// ウィンドウのサイズを調整
					SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED			// ウィンドウの位置を変更せず、Zオーダーを変更せず、フレームを再描画する
				);
			}
			else// ウィンドウモードの場合
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);// ウィンドウスタイルをポップアップスタイルに設定
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST); // 拡張スタイルを最前面に設定
				SetWindowPos(// ウィンドウの位置とサイズを変更せず、Zオーダーを変更せず、フレームを再描画する
					hWnd,															// ウィンドウハンドル
					HWND_TOP,														// ウィンドウを最前面に表示
					0, 0, 0, 0,														// ウィンドウの位置を(0, 0)に設定
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);		// ウィンドウの位置を変更せず、サイズを変更せず、Zオーダーを変更せず、フレームを再描画する
				ShowWindow(hWnd, SW_SHOWMAXIMIZED); // ウィンドウを最大化して表示
			}
			s_fullscreen = !s_fullscreen; // フルスクリーン状態を切り替える
		}
		break;

	case WM_MENUCHAR:// メニューのキャラクター入力が発生したとき
		return MAKELRESULT(0, MNC_CLOSE);// メニューがアクティブで、ユーザーがニーモニックやアクセラレータキーに対応しないキーを押した場合は無視
	}
	return DefWindowProc(hWnd, message, wParam, lParam);// デフォルトのウィンドウプロシージャを呼び出して、メッセージを処理する
}

/*
*	@brief ゲームを終了する関数
*	@details この関数はゲームを終了するために使用される
*	@param なし
*	@return なし
*/
void ExitGame() noexcept
{
	PostQuitMessage(0);// メッセージキューにWM_QUITメッセージをポストして、アプリケーションを終了するように指示
}
