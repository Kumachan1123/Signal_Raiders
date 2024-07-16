/**
 * @file   UpdateInfo.h
 *
 * @brief  Wi-Fiの情報を更新するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // 多重インクルードの防止 =====================================================
#pragma once
#include "Game/Wifi/CheckVersion/CheckVersion.h"
#include "Game/Wifi/GetListOfInterfaces/GetListOfInterfaces.h"
#include "Game/Wifi/GetScanResults/GetScanResults.h"
#include "Game/Wifi/StartScan/StartScan.h"
#include "Game/Wifi/InitHandle/InitHandle.h"
#include "Game/Wifi/Preparation/Preparation.h"
#include "Game/Wifi/ProcessingScanResults/ProcessingScanResults.h"
#include "Game/Wifi/Interface/IWifiParts.h"
// クラスの定義 ===============================================================
/**
  * @brief ゲーム
  */
class UpdateInfo
{
	// データメンバの宣言
private:
	// クラスいろいろ
	std::unique_ptr<GetListOfInterfaces>	 m_InterfacesList;
	std::unique_ptr<CheckVersion>			 m_Checkversion;
	std::unique_ptr<StartScan>				 m_StartScan;
	std::unique_ptr<GetScanResults>			 m_GetResults;
	std::unique_ptr <InitHandle>			 m_handle;
	std::unique_ptr<Preparation>			 m_Preparation;
	std::unique_ptr< ProcessingScanResults>	 m_ProcessingScanResults;
	// メンバ関数の宣言

public:
	// コンストラクタ
	UpdateInfo();
	// デストラクタ
	~UpdateInfo();
	void Initialize();
	void Set(DWORD& dwResult,// Wi-Fi関連の宣言
			 DWORD& dwMaxClient,// Wi-Fi関連の宣言
			 DWORD& dwCurVersion,// Wi-Fi関連の宣言
			 HANDLE& hClient,// Wi-Fi関連の宣言
			 PWLAN_INTERFACE_INFO_LIST& pInterfaceList,// インターフェースのリスト取得
			 PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,// スキャン結果の取得
			 std::vector<NetworkInfo>& networkInfos,// ネットワーク情報を格納するvector
			 WLAN_AVAILABLE_NETWORK& network,// スキャン結果の処理に使う変数
			 std::string& ssid,// スキャン結果の処理に使う変数
			 std::set<std::string>& displayedSSIDs,// すでに表示したSSIDを管理するためのset
			 std::wstring_convert<std::codecvt_utf8<wchar_t>>& converter,// スキャン結果の処理に使う変数
			 int& count,// 表示した回数
			 std::vector<int>& preWifilevels
	);
};
