/*
*	@file	Wifi.h
*	@brief	Wi-Fiの情報を取得するクラス
*/
#pragma once
#ifndef WIFI_DEFINED
#define WIFI_DEFINED
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

// Wi-Fiの情報を取得するクラス
class Wifi
{
public:
	// 構造体
	// 現在接続しているWi-Fi情報の構造体
	struct CurrentWifiInfo
	{
		int ssidLenght;// SSIDの長さ
		int signalQuality;// 電波の強さ
	};
public:
	// アクセサ
	//電波の強さを渡す
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }
	//敵の種類を渡す
	std::vector<int> GetEnemyTypes()const { return m_enemyTypes; }
	// 接続しているWi-FiのSSIDの長さを渡す
	int GetCurrentWifiSSIDLength()const { return m_currentWifiInfo.ssidLenght; }
	// 接続しているWi-Fiの電波の強さを渡す
	int GetCurrentWifiSignalQuality()const { return m_currentWifiInfo.signalQuality; }
public:
	// public関数
	// コンストラクタ
	Wifi();
	// デストラクタ
	~Wifi();
	// 更新処理
	void Update(float elapsedTime);
	// クリア
	void Clear();
private:
	// private関数
	// ハンドルの初期化
	void InitHandle();
	// バージョンの確認
	void Checkversion();
	// インターフェースのリスト取得
	void GetInterfacesList();
	// スキャンの開始
	void StartScan();
	// スキャン結果の取得
	void GetScanResults();
	// 表示準備
	void ClearExportInfo();
	// 現在接続しているWi-Fi情報を取得
	void GetCurrentWifiInfo();
	// スキャン結果の処理
	void ProcessingScanResults();
	// SSIDを無理やり数値に変換する関数（各文字のASCIIコードの合計）
	int ConvertSsidToInt(const std::string& ssid);
private:
	// private変数
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
	// プレイ開始5秒間に使う配列
	// Wi-Fiの強さを格納する配列
	std::vector<int> m_preWifilevels;
	// 敵の種類(ssidの文字のASCIIコードの合計をssidの文字数で割ったもののあまり)を格納する配列
	std::vector<int> m_preEnemyTypes;
	// 五秒後に以下の配列にWi-Fiの情報を格納する（一度入れたら情報は更新しない）
	// Wi-Fiの強さを格納する配列
	std::vector<int> m_wifilevels;
	// 敵の種類(ssidの文字のASCIIコードの合計をssidの文字数で割ったもののあまり)を格納する配列
	std::vector<int> m_enemyTypes;
	// 今接続しているWi-Fi情報
	CurrentWifiInfo m_currentWifiInfo;
	// 今接続しているWi-Fi情報を取得済みか
	bool m_isGotCurrentWifiInfo;
	// Wi-Fi取得時間（５秒過ぎたら５秒で固定）
	float m_time;
	// ザコ敵の種類の最大値
	const int ENEMY_TYPE_MAX = 3;
};
#endif // WIFI_DEFINED
