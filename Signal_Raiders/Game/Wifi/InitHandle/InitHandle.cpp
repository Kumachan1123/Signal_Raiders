/**
 * @file   InitHandle.h
 *
 * @brief  ハンドルの初期化に関するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/InitHandle/InitHandle.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
InitHandle::InitHandle()
{
}
/**
 * @brief デストラクタ
 */
InitHandle::~InitHandle()
{
}
// Wi-Fiハンドルの初期化
void InitHandle::Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient)
{
	dwMaxClient = 2; // 最大のクライアント数
	hClient = NULL;
	dwCurVersion = 0;
	dwResult = 0;
}

void InitHandle::Initialize(Wifi* pWifi)
{
	pWifi->SetMaxClient(2);
	pWifi->SetClient(NULL);
	pWifi->SetCurVersion(0);
	pWifi->SetResult(0);
}
