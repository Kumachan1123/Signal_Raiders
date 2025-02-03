/**
 * @file   StartScan.h
 *
 * @brief  スキャンの開始に関するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/StartScan/StartScan.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
StartScan::StartScan()
{
}
/**
 * @brief デストラクタ
 */
StartScan::~StartScan()
{
}
// スキャンの開始
void StartScan::Set(DWORD& dwResult, HANDLE& hClient, PWLAN_INTERFACE_INFO_LIST& pInterfaceList)
{
	dwResult = WlanScan(hClient, &pInterfaceList->InterfaceInfo[0].InterfaceGuid, NULL, NULL, NULL);
	if (dwResult != ERROR_SUCCESS)
	{
		WlanFreeMemory(pInterfaceList);
		WlanCloseHandle(hClient, NULL);
		return;
	}
}

void StartScan::Scan(Wifi* pWifi)
{
	PWLAN_INTERFACE_INFO_LIST pInterfaceList = pWifi->GetInterfaceList();
	const GUID* pInterfaceGuid = &pInterfaceList->InterfaceInfo[0].InterfaceGuid;
	// WlanScan 呼び出し
	pWifi->SetResult(WlanScan(
		pWifi->GetClient(),
		pInterfaceGuid,
		NULL, NULL, NULL));	if (pWifi->GetResult() != ERROR_SUCCESS)
	{
		WlanFreeMemory(pWifi->GetInterfaceList());
		WlanCloseHandle(pWifi->GetClient(), NULL);
		return;
	}
}
