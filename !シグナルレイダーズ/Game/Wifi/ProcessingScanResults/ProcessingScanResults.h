/**
 * @file   ProcessingScanResults.h
 *
 * @brief  スキャン結果の処理に関するヘッダファイル
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
class ProcessingScanResults
{
	// メンバ関数の宣言
public:
	// コンストラクタ
	ProcessingScanResults();
	// デストラクタ
	~ProcessingScanResults();
	// スキャン結果の処理
	void Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList, 
			 WLAN_AVAILABLE_NETWORK& network, 
			 std::string& ssid,
			 std::set<std::string>& displayedSSIDs,
			 std::wstring_convert<std::codecvt_utf8<wchar_t>>& converter,
			 std::vector<NetworkInfo>& networkInfos);


};
