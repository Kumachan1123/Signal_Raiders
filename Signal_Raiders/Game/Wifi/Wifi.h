/*
*	@file	Wifi.h
*	@brief	Wi-Fiの情報を取得するクラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Wifi/Parameters/WiFiParameters.h"
// 前方宣言
class CommonResources;
class Output;
class ReleaseMemory;
class Wifi
{
public:// 構造体
	struct CurrentWifiInfo// 現在接続しているWi-Fi情報の構造体
	{
		int ssidLenght;// SSIDの長さ
		int signalQuality;// 電波の強さ
	};
public:// アクセサ
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }//電波の強さを渡す
	std::vector<int> GetEnemyTypes()const { return m_enemyTypes; }//敵の種類を渡す
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }// 電波の強さを渡す
	std::vector<int> GetPreEnemyTypes()const { return m_preEnemyTypes; }// 敵の種類を渡す
	int GetCurrentWifiSSIDLength()const { return m_currentWifiInfo.ssidLenght; }// 接続しているWi-FiのSSIDの長さを渡す
	int GetCurrentWifiSignalQuality()const { return m_currentWifiInfo.signalQuality; }// 接続しているWi-Fiの電波の強さを渡す
	DWORD GetResult()const { return m_dwResult; }// 結果を渡す
	DWORD* GetResult() { return &m_dwResult; }// 結果を渡す(ポインター)
	void SetResult(DWORD result) { m_dwResult = result; }// 結果をセットする
	DWORD GetMaxClient()const { return m_dwMaxClient; }// 最大クライアント数を渡す
	void SetMaxClient(DWORD maxClient) { m_dwMaxClient = maxClient; }// 最大クライアント数をセットする
	DWORD* GetCurVersion() { return &m_dwCurVersion; }// バージョンを渡す(ポインター)
	void SetCurVersion(DWORD curVersion) { m_dwCurVersion = curVersion; }// バージョンをセットする
	HANDLE* GetClient() { return &m_hClient; }// クライアントハンドルを渡す(ポインター)
	void SetClient(HANDLE client) { m_hClient = client; }// クライアントハンドルをセットする
	const PWLAN_INTERFACE_INFO_LIST& GetInterfaceList() { return m_pInterfaceList; }// インターフェースのリストを渡す
	PWLAN_INTERFACE_INFO_LIST* GetPInterfaceList() { return &m_pInterfaceList; }// インターフェースのリストを渡す(ポインター)
	void SetInterfaceList(PWLAN_INTERFACE_INFO_LIST interfaceList) { m_pInterfaceList = interfaceList; }// インターフェースのリストをセットする
	PWLAN_AVAILABLE_NETWORK_LIST GetNetworkList()const { return m_pNetworkList; }// スキャン結果のリストを渡す
	void SetNetworkList(PWLAN_AVAILABLE_NETWORK_LIST networkList) { m_pNetworkList = networkList; }// スキャン結果のリストをセットする
	PWLAN_AVAILABLE_NETWORK_LIST* GetPNetworkList() { return &m_pNetworkList; }// スキャン結果のリストを渡す(ポインター)
	std::vector<NetworkInfo> GetNetworkInfos()const { return m_networkInfos; }// ネットワーク情報を渡す
	void SetNetworkInfos(std::vector<NetworkInfo> networkInfos) { m_networkInfos = networkInfos; }// ネットワーク情報をセットする
	WLAN_AVAILABLE_NETWORK GetNetwork()const { return m_network; }// スキャン結果を渡す
	void SetNetwork(WLAN_AVAILABLE_NETWORK network) { m_network = network; }// スキャン結果をセットする
	std::string GetSSID()const { return m_ssid; }// SSIDを渡す
	void SetSSID(std::string ssid) { m_ssid = ssid; }// SSIDをセットする
	std::set<std::string> GetDisplayedSSIDs()const { return m_exportSSIDs; }// すでに表示したSSIDを渡す
	void SetDisplayedSSIDs(std::set<std::string> displayedSSIDs) { m_exportSSIDs = displayedSSIDs; }// すでに表示したSSIDをセットする
	int GetCount()const { return m_count; }// 表示した回数を渡す
	void SetCount(int count) { m_count = count; }// 表示した回数をセットする
public:// public関数
	Wifi();// コンストラクタ
	~Wifi();// デストラクタ
	void Update(float elapsedTime);// 更新処理
	void Clear();// クリア
private:// private関数
	void InitHandle();// ハンドルの初期化
	void Checkversion();// バージョンの確認
	void GetInterfacesList();// インターフェースのリスト取得
	void StartScan();// スキャンの開始
	void GetScanResults();// スキャン結果の取得
	void ClearExportInfo();// 表示準備
	void GetCurrentWifiInfo();// 現在接続しているWi-Fi情報を取得
	void ProcessingScanResults();// スキャン結果の処理
	int ConvertSsidToInt(const std::string& ssid);// SSIDを無理やり数値に変換する関数（各文字のASCIIコードの合計）
private:// private変数

	// 最大のクライアント数
	DWORD  m_dwMaxClient;
	// クライアントハンドル
	HANDLE m_hClient;
	// WLANのバージョン
	DWORD  m_dwCurVersion;
	// WLANの結果
	DWORD  m_dwResult;
	// インターフェースのリスト取得
	PWLAN_INTERFACE_INFO_LIST m_pInterfaceList;
	// スキャン結果の取得
	PWLAN_AVAILABLE_NETWORK_LIST m_pNetworkList;
	// すでに表示したSSIDを管理するためのset
	std::set<std::string> m_exportSSIDs;
	// 表示した回数
	int m_count;
	// ネットワーク情報を格納するvector
	std::vector<NetworkInfo> m_networkInfos;
	// スキャン結果の処理に使う変数
	WLAN_AVAILABLE_NETWORK m_network;
	// SSIDを格納する変数
	std::string m_ssid;
	// wstringをstringに変換するための変数
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
	// Wi-Fi取得時間（５秒過ぎたら５秒で固定）
	float m_time;
	// ザコ敵の種類の最大値
	const int ENEMY_TYPE_MAX = 2;
};
