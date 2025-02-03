/**
 * @file   CheckVersion.h
 *
 * @brief  Wi-Fiの電波の強さ・認証と暗号のセキュリティレベルを得るソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "pch.h"
#include "Game/Screen.h"
#include <sstream>
#include "Game/Wifi/CheckVersion/CheckVersion.h"

// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
CheckVersion::CheckVersion()
{
}
/**
 * @brief デストラクタ
 */
CheckVersion::~CheckVersion()
{
}
// バージョンの確認
void CheckVersion::Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient)
{
	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) return;
}

void CheckVersion::Check(Wifi* pWifi)
{
	auto result = WlanOpenHandle(pWifi->GetMaxClient(), NULL, pWifi->GetCurVersion(), pWifi->GetClient());
	pWifi->SetResult(result);
}
