/**
 * @file   ReleaseMemory.h
 *
 * @brief  情報表示に関するヘッダファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // 多重インクルードの防止 =====================================================
#pragma once
#include "Game/Wifi/Interface/IWifiParts.h"
// クラスの定義 ===============================================================
/**
  * @brief ゲーム
  */
  // ネットワーク情報を格納する構造体
class ReleaseMemory
{
	// メンバ関数の宣言
public:
	// コンストラクタ
	ReleaseMemory();
	// デストラクタ
	~ReleaseMemory();
	// メモリの解放とハンドルのクローズ
	void FreeMemoryAndCloseHandle(PWLAN_INTERFACE_INFO_LIST& pInterfaceList,HANDLE& hClient, std::vector<NetworkInfo>& networkInfos, std::set<std::string>& displayedSSIDs);
};
