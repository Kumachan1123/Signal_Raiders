/**
 * @file   GetListOfInterfaces.h
 *
 * @brief インターフェースのリスト取得するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "pch.h"
#include "Game/Screen.h"
#include "Game/Wifi/GetListOfInterfaces/GetListOfInterfaces.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
GetListOfInterfaces::GetListOfInterfaces()
{
}
/**
 * @brief デストラクタ
 */
GetListOfInterfaces::~GetListOfInterfaces()
{
}
// インターフェースのリスト取得
void GetListOfInterfaces::Set(PWLAN_INTERFACE_INFO_LIST& pInterfaceList, DWORD& dwResult, HANDLE& hClient)
{
	pInterfaceList = NULL;
	dwResult = WlanEnumInterfaces(hClient, NULL, &pInterfaceList);
	
	if (dwResult != ERROR_SUCCESS)
	{
		WlanCloseHandle(hClient, NULL);
		return;
	}
}



