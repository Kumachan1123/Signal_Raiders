/**
 * @file   Wifi.h
 *
 * @brief  Wi-Fiの電波の強さ・認証と暗号のセキュリティレベルを得るヘッダファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */

 // 多重インクルードの防止 =====================================================
#pragma once
#include "Game/CommonResources.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"

#include "Game/Wifi/Wifi.h"
#include "Libraries/MyLib/DebugString.h"



// クラスの定義 ===============================================================
/**
  * @brief ゲーム
  */
class CommonResources;
class Output;
class ReleaseMemory;

class Wifi
{
public:
	struct CurrentWifiInfo// 現在接続しているWi-Fi情報の構造体
	{
		int ssidValue;// 数値化したSSID
		int signalQuality;// 電波の強さ
	};
	// データメンバの宣言
private:

	// wi-fi関連の変数
	std::unique_ptr<Output>					 m_output;
	std::unique_ptr<ReleaseMemory>			 m_memory;
	// Wi-Fi関連の宣言
	DWORD  m_dwMaxClient; // 最大のクライアント数
	HANDLE m_hClient;
	DWORD  m_dwCurVersion;
	DWORD  m_dwResult;
	// インターフェースのリスト取得
	PWLAN_INTERFACE_INFO_LIST m_pInterfaceList;
	// スキャン結果の取得
	PWLAN_AVAILABLE_NETWORK_LIST m_pNetworkList;
	// すでに表示したSSIDを管理するためのset
	std::set<std::string> m_displayedSSIDs;
	// 表示した回数
	int m_count;
	// ネットワーク情報を格納するvector
	std::vector<NetworkInfo> m_networkInfos;
	// スキャン結果の処理に使う変数
	WLAN_AVAILABLE_NETWORK m_network;
	std::string m_ssid;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_converter;

	// 最初の五秒はこの配列にWi-Fiの強さを格納する
	std::vector<int> m_preWifilevels;// Wi-Fiの強さ
	std::vector<int> m_preEnemyTypes;// 敵の種類(ssidの文字のASCIIコードの合計をssidの文字数で割ったもののあまり)

	// 五秒後に以下の配列にWi-Fiの情報を格納する（一度入れたら情報は更新しない）
	std::vector<int> m_wifilevels;// Wi-Fiの強さ
	std::vector<int> m_enemyTypes;// 敵の種類(ssidの文字のASCIIコードの合計をssidの文字数で割ったもののあまり)

	// 今接続しているWi-Fi情報
	CurrentWifiInfo m_currentWifiInfo;
	// 今接続しているWi-Fi情報を取得済みか
	bool m_isGotCurrentWifiInfo;
	float m_time;				  // Wi-Fi取得時間（５秒過ぎたら５秒で固定）
	const int ENEMY_TYPE_MAX = 2;// ザコ敵の種類の最大値
public:
	// コンストラクタ
	Wifi();
	// デストラクタ
	~Wifi();
	// 操作
public:
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// クリア
	void Clear();
private:
	void InitHandle();// ハンドルの初期化
	void Checkversion();// バージョンの確認
	void GetInterfacesList();// インターフェースのリスト取得
	void StartScan();// スキャンの開始
	void GetScanResults();// スキャン結果の取得
	void SetUp();// 表示準備
	void GetCurrentWifiInfo();// 現在接続しているWi-Fi情報を取得
	void ProcessingScanResults();// スキャン結果の処理
public:
	// 取得した情報を送る
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }//電波の強さを渡す
	std::vector<int> GetEnemyTypes()const { return m_enemyTypes; }//敵の種類を渡す
	// 更新が終わったかどうかを調べるためのゲッター
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }
	std::vector<int> GetPreEnemyTypes()const { return m_preEnemyTypes; }
	// 今接続しているWi-Fi情報を取得する
	int GetCurrentWifiSSIDValue()const { return m_currentWifiInfo.ssidValue; }
	int GetCurrentWifiSignalQuality()const { return m_currentWifiInfo.signalQuality; }
	DWORD GetResult()const { return m_dwResult; }
	DWORD* GetResult() { return &m_dwResult; }
	void SetResult(DWORD result) { m_dwResult = result; }
	DWORD GetMaxClient()const { return m_dwMaxClient; }
	void SetMaxClient(DWORD maxClient) { m_dwMaxClient = maxClient; }
	DWORD* GetCurVersion() { return &m_dwCurVersion; }
	void SetCurVersion(DWORD curVersion) { m_dwCurVersion = curVersion; }
	HANDLE* GetClient() { return &m_hClient; }
	void SetClient(HANDLE client) { m_hClient = client; }
	const PWLAN_INTERFACE_INFO_LIST& GetInterfaceList() { return m_pInterfaceList; }
	PWLAN_INTERFACE_INFO_LIST* GetPInterfaceList() { return &m_pInterfaceList; }
	void SetInterfaceList(PWLAN_INTERFACE_INFO_LIST interfaceList) { m_pInterfaceList = interfaceList; }
	PWLAN_AVAILABLE_NETWORK_LIST GetNetworkList()const { return m_pNetworkList; }
	void SetNetworkList(PWLAN_AVAILABLE_NETWORK_LIST networkList) { m_pNetworkList = networkList; }
	PWLAN_AVAILABLE_NETWORK_LIST* GetPNetworkList() { return &m_pNetworkList; }
	std::vector<NetworkInfo> GetNetworkInfos()const { return m_networkInfos; }
	void SetNetworkInfos(std::vector<NetworkInfo> networkInfos) { m_networkInfos = networkInfos; }
	WLAN_AVAILABLE_NETWORK GetNetwork()const { return m_network; }
	void SetNetwork(WLAN_AVAILABLE_NETWORK network) { m_network = network; }
	std::string GetSSID()const { return m_ssid; }
	void SetSSID(std::string ssid) { m_ssid = ssid; }
	std::set<std::string> GetDisplayedSSIDs()const { return m_displayedSSIDs; }
	void SetDisplayedSSIDs(std::set<std::string> displayedSSIDs) { m_displayedSSIDs = displayedSSIDs; }
	int GetCount()const { return m_count; }
	void SetCount(int count) { m_count = count; }
};
