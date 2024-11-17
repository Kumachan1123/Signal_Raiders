/**
 * @file   GetScanResults.h
 *
 * @brief  スキャン結果の取得するヘッダファイル
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
class GetScanResults
{
	// メンバ関数の宣言
public:
	// コンストラクタ
	GetScanResults();
	// デストラクタ
	~GetScanResults();
	void Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,
			 DWORD& dwResult,
			 HANDLE& hClient,
			 PWLAN_INTERFACE_INFO_LIST& pInterfaceList);
};
