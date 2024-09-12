/**
 * @file   GetScanResults.h
 *
 * @brief  スキャン結果の取得するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */


 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/GetScanResults/GetScanResults.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
GetScanResults::GetScanResults()
{
}
/**
 * @brief デストラクタ
 */
GetScanResults::~GetScanResults()
{
}
// スキャン結果の取得
void GetScanResults::Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList, DWORD& dwResult, HANDLE& hClient, PWLAN_INTERFACE_INFO_LIST& pInterfaceList)
{
	pNetworkList = NULL;
	dwResult =
		WlanGetAvailableNetworkList(hClient,
									&pInterfaceList->InterfaceInfo[0].InterfaceGuid,
									0,
									NULL,
									&pNetworkList);
	if (dwResult != ERROR_SUCCESS)
	{
		return;
	}
}




