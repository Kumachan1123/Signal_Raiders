/**
 * @file   ReleaseMemory.h
 *
 * @brief  情報表示に関するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
ReleaseMemory::ReleaseMemory()
{
}
/**
 * @brief デストラクタ
 */
ReleaseMemory::~ReleaseMemory()
{
}
// メモリの解放とハンドルのクローズ
void ReleaseMemory::FreeMemoryAndCloseHandle(PWLAN_INTERFACE_INFO_LIST& pInterfaceList, HANDLE& hClient, std::vector<NetworkInfo>& networkInfos, std::set<std::string>& displayedSSIDs)
{
	WlanFreeMemory(pInterfaceList);
	WlanCloseHandle(hClient, NULL);
	networkInfos.clear();
	displayedSSIDs.clear();
}