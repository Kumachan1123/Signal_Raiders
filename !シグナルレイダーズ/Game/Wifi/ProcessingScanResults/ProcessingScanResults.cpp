/**
 * @file   ProcessingScanResults.h
 *
 * @brief  スキャン結果の処理に関するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/ProcessingScanResults/ProcessingScanResults.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
ProcessingScanResults::ProcessingScanResults()
{
}
/**
 * @brief デストラクタ
 */
ProcessingScanResults::~ProcessingScanResults()
{
}
// スキャン結果の処理
void ProcessingScanResults::Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,
									WLAN_AVAILABLE_NETWORK& network,
									std::string& ssid,
									std::set<std::string>& displayedSSIDs,
									std::wstring_convert<std::codecvt_utf8<wchar_t>>& converter,
									std::vector<NetworkInfo>& networkInfos)
{
	for (DWORD i = 0; i < pNetworkList->dwNumberOfItems; i++)
	{

		network = pNetworkList->Network[i];


		ssid = (network.dot11Ssid.uSSIDLength == 0) ? "非公開のネットワーク" : converter.to_bytes(std::wstring(network.dot11Ssid.ucSSID, network.dot11Ssid.ucSSID + network.dot11Ssid.uSSIDLength));

		// すでに表示したSSIDの場合はスキップ
		if (displayedSSIDs.find(ssid) == displayedSSIDs.end())
		{
			// ネットワーク情報をvectorに追加
			networkInfos.push_back({ ssid, network.wlanSignalQuality, network.dot11DefaultCipherAlgorithm, network.dot11DefaultAuthAlgorithm });

			// すでに表示したSSIDをsetに追加
			displayedSSIDs.insert(ssid);

		}
	}
}
