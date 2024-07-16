/**
 * @file   UpdateInfo.h
 *
 * @brief  Wi-Fiの情報を更新するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
UpdateInfo::UpdateInfo()
	: m_InterfacesList{ nullptr }
	, m_Checkversion{ nullptr }
	, m_StartScan{ nullptr }
	, m_GetResults{ nullptr }
	, m_handle{ nullptr }
	, m_Preparation{ nullptr }
	, m_ProcessingScanResults{ nullptr }
{
}
/**
 * @brief デストラクタ
 */
UpdateInfo::~UpdateInfo()
{
}
void UpdateInfo::Initialize()
{
	m_InterfacesList = std::make_unique<GetListOfInterfaces>();
	m_Checkversion = std::make_unique<CheckVersion>();
	m_StartScan = std::make_unique<StartScan>();
	m_GetResults = std::make_unique<GetScanResults>();
	m_Preparation = std::make_unique<Preparation>();
	m_ProcessingScanResults = std::make_unique< ProcessingScanResults>();
}
void UpdateInfo::Set(DWORD& dwResult,
					 DWORD& dwMaxClient,
					 DWORD& dwCurVersion,
					 HANDLE& hClient,
					 PWLAN_INTERFACE_INFO_LIST& pInterfaceList,
					 PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,
					 std::vector<NetworkInfo>& networkInfos,
					 WLAN_AVAILABLE_NETWORK& network,
					 std::string& ssid,
					 std::set<std::string>& displayedSSIDs,
					 std::wstring_convert<std::codecvt_utf8<wchar_t>>& converter,
					 int& count,
					 std::vector<int>& preWifilevels)
{
	// Wi-Fiハンドルの初期化
	m_handle->Set(dwResult, dwMaxClient, dwCurVersion, hClient);
	// バージョンの確認
	m_Checkversion->Set(dwResult, dwMaxClient, dwCurVersion, hClient);
	// インターフェースのリスト取得
	m_InterfacesList->Set(pInterfaceList, dwResult, hClient);
	// スキャンの開始
	m_StartScan->Set(dwResult, hClient, pInterfaceList);
	// スキャン結果の取得
	m_GetResults->Set(pNetworkList, dwResult, hClient, pInterfaceList, networkInfos, preWifilevels);
	// 表示準備
	m_Preparation->Set(displayedSSIDs, count, networkInfos);
	// スキャン結果の処理
	m_ProcessingScanResults->Set(pNetworkList, network, ssid, displayedSSIDs, converter, networkInfos);
}
