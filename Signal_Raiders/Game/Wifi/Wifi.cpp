/*
*	@file	Wifi.cpp
*	@brief	Wi-Fiの情報を取得するクラス
*/
#include <pch.h>
#include "Wifi.h"
/*
*	@brief	コンストラクタ
*	@details Wi-Fiクラスのコンストラクタ
*	@param	なし
*	@return なし
*/
Wifi::Wifi()
	: m_dwCurVersion{}// バージョン
	, m_hClient{}// クライアントハンドル
	, m_dwMaxClient{}// 最大クライアント数
	, m_dwResult{}// 結果
	, m_pInterfaceList{}// インターフェースのリスト
	, m_pNetworkList{}// スキャン結果のリスト
	, m_exportSSIDs{}// すでに渡したSSID
	, m_count{}// 表示した回数
	, m_networkInfos{}// ネットワーク情報
	, m_currentWifiInfo{}// 現在接続しているWi-Fi情報
	, m_network{}// スキャン結果
	, m_ssid{}// SSID
	, m_converter{}// wstringをstringに変換するための変数
	, m_isGotCurrentWifiInfo(false)// 現在接続しているWi-Fi情報を取得済みか
	, m_time(0.0f)// Wi-Fi取得時間
{
}
/*
*	@brief デストラクタ
*	@details Wi-Fiクラスのデストラクタ(何もしない）
*	@param なし
*	@return なし
*/
Wifi::~Wifi() {/*do nothing*/ }

/*
*	@brief 更新処理
*	@details Wi-Fiクラスの更新処理
*	@param elapsedTime 更新にかかった時間
*	@return なし
*/
void Wifi::Update(float elapsedTime)
{
	if (m_time >= WiFiParameters::MAX_TIME)return;// 五秒経ったら更新終了
	InitHandle();// Wi-Fiハンドルの初期化
	Checkversion();// バージョンの確認
	GetInterfacesList();// インターフェースのリスト取得
	StartScan();// スキャンの開始
	GetScanResults();// スキャン結果の取得
	GetCurrentWifiInfo();// 現在接続しているWi-Fi情報を取得
	ClearExportInfo();// 表示準備
	ProcessingScanResults();// スキャン結果の処理
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());// 電波の強さでソート
	m_time += elapsedTime;// 時間を加算
	if (m_dwResult != ERROR_SUCCESS)// Wi-Fiを取得できない状態の時
	{
		if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)// 設定された敵の最大値未満の場合
		{
			for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)//最大値になるまで繰り返す
			{
				if (m_time >= WiFiParameters::MAX_TIME)break;// 五秒経ったら更新終了
				m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// 電波の強さを100に設定
				m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// 敵の種類を0(通常)に設定
			}
		}
		m_wifilevels = m_preWifilevels;// 電波の強さを可変長配列に登録
		m_enemyTypes = m_preEnemyTypes;// 敵の種類を可変長配列に登録
	}
	else// Wi-Fiを取得できる状態の時
	{
		if (m_networkInfos.size() == 0)// 取得した数が0の時
		{
			if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)// 設定された敵の最大値未満の場合
			{
				for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)//最大値になるまで繰り返す
				{
					if (m_time >= WiFiParameters::MAX_TIME)break;// 五秒経ったら更新終了
					m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// 電波の強さを100に設定
					m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// 敵の種類を0(通常)に設定
				}
			}
			m_wifilevels = m_preWifilevels;// 電波の強さを可変長配列に登録
			m_enemyTypes = m_preEnemyTypes;// 敵の種類を可変長配列に登録
		}
		// 以下、正常にWi-Fiを取得できた場合
		for (const auto& networkInfo : m_networkInfos)// 取得した数だけ繰り返す
		{
			if (m_time >= WiFiParameters::MAX_TIME)// 五秒経ったら更新終了
			{
				m_time = WiFiParameters::MAX_TIME;// 時間を最大値に固定
				break;// ループを抜ける
			}
			int ssidLength = (int)(networkInfo.ssid.length());// ssidの文字数を可変長配列に登録
			int ssidValue = ConvertSsidToInt(networkInfo.ssid);// ssidの文字のASCIIコードの合計を可変長配列に登録
			int enemyType = (ssidValue * ssidLength) % ENEMY_TYPE_MAX;// (文字数×文字のASCIIコードの合計)/ENEMY_TYPE_MAXの余りを敵の種類に変換
			m_preWifilevels.push_back(networkInfo.signalQuality);// 電波の強さを可変長配列に登録
			m_preEnemyTypes.push_back(enemyType);// 敵の種類を設定	
			m_wifilevels = m_preWifilevels;// 電波の強さを可変長配列に登録
			m_enemyTypes = m_preEnemyTypes;// 敵の種類を可変長配列に登録
		}
	}
}
/*
*	@brief	クリア
*	@details Wi-Fiクラスのクリア処理
*	@param なし
*	@return なし
*/
void Wifi::Clear()
{
	if (m_dwResult != ERROR_SUCCESS || m_time >= WiFiParameters::MAX_TIME)return;// 五秒経ったら更新終了
	WlanFreeMemory(m_pInterfaceList);// インターフェースリストのメモリを解放
	WlanCloseHandle(m_hClient, NULL);// ハンドルをクローズ
	m_networkInfos.clear();// Wi-Fi情報を格納する可変長配列をクリア
	m_exportSSIDs.clear();// 表示済みSSIDのセットをクリア
	m_preWifilevels.clear();// 電波の強さをクリア
	m_preEnemyTypes.clear();// 敵の種類をクリア
}
/*
*	@brief	ハンドルの初期化
*	@details Wi-Fiクラスのハンドルの初期化
*	@param なし
*	@return なし
*/
void Wifi::InitHandle()
{
	m_dwMaxClient = 2; // 最大のクライアント数
	m_hClient = NULL;// クライアントハンドル
	m_dwCurVersion = 0;// WLANのバージョン
	m_dwResult = 0;// 結果
}
/*
*	@brief	バージョンの確認
*	@details WlanAPIのクライアントバージョンを確認
*	@param なし
*	@return なし
*/
void Wifi::Checkversion()
{
	m_dwResult = WlanOpenHandle(// WlanAPIのクライアントバージョンを確認
		m_dwMaxClient,     // 要求するクライアントのバージョン（入力）
		NULL,              // 予約（NULLでOK）
		&m_dwCurVersion,   // 実際に使えるバージョンがここに返る（出力）
		&m_hClient);       // 取得されたクライアントハンドル（出力）
	if (m_dwResult != ERROR_SUCCESS) return;// エラー処理
}
/*
*	@brief インターフェースのリスト取得
*	@details インターフェースのリストを取得
*/
void Wifi::GetInterfacesList()
{
	m_pInterfaceList = NULL;// リストポインターを初期化
	m_dwResult = WlanEnumInterfaces(m_hClient, NULL, &m_pInterfaceList);// インターフェースのリスト取得
	if (m_dwResult != ERROR_SUCCESS)// エラー処理
	{
		WlanCloseHandle(m_hClient, NULL);// ハンドルのクローズ
		return;// 処理の中止
	}
}

/*
*	@brief スキャンの開始
*	@details 周囲のWi-Fiアクセスポイントをスキャンする
*	@param なし
*	@return なし
*/
void Wifi::StartScan()
{
	m_dwResult = WlanScan(// 指定したインターフェースでWi-Fiスキャンを開始
		m_hClient, // Wi-Fiクライアントのハンドル
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid, // 使用するアダプタのGUID
		NULL,// スキャンのオプション（特に指定なし）
		NULL, // 予約引数（不使用）
		NULL);// 予約引数（不使用）
	if (m_dwResult != ERROR_SUCCESS)// エラー処理
	{
		WlanFreeMemory(m_pInterfaceList);// メモリの解放
		WlanCloseHandle(m_hClient, NULL);// ハンドルのクローズ
		return;// 処理の中止
	}
}
/*
*	@brief スキャン結果の取得
*	@details スキャンした結果から、周囲の利用可能なWi-Fiネットワーク情報を取得する
*	@param なし
*	@return なし
*/
void Wifi::GetScanResults()
{
	m_pNetworkList = NULL; // ネットワークリストのポインターを初期化
	m_dwResult = WlanGetAvailableNetworkList(// 使用可能なネットワークの一覧を取得
		m_hClient,// Wi-Fiクライアントのハンドル
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// 使うインターフェースのGUID
		0,// オプションのフラグ（特に指定なし）
		NULL,// 使用する言語（基本はNULL）
		&m_pNetworkList);// ネットワークリスト
	if (m_dwResult != ERROR_SUCCESS)return;// エラー処理
}

/*
*	@brief	外部に渡すWi-Fi情報を初期化
*	@details ゲームのロジックや他のシステムに渡すためのSSIDや電波情報をクリアする
*	@param なし
*	@return なし
*/
void Wifi::ClearExportInfo()
{
	m_exportSSIDs.clear();// 表示するSSIDをクリア
	m_networkInfos.clear();// ネットワーク情報をクリア
}
/*
*	@brief	現在接続しているWi-Fi情報を取得
*	@details 現在接続しているWi-FiのSSIDや電波の強さを取得する
*	@param なし
*	@return なし
*/
void Wifi::GetCurrentWifiInfo()
{
	if (m_isGotCurrentWifiInfo)return;// すでに取得済みなら終了
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;// 接続情報のポインター
	DWORD connectInfoSize = 0;// 接続情報のサイズ
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;// データの種類
	m_dwResult = WlanQueryInterface(// 現在接続しているWi-Fi情報を取得
		m_hClient,// Wi-Fiクライアントのハンドル
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// 使用するインターフェースのGUID
		wlan_intf_opcode_current_connection,// 現在の接続情報を取得
		NULL,// 予約引数（不使用）
		&connectInfoSize,// 接続情報のサイズ（出力）
		(PVOID*)&pConnectInfo,// 接続情報のポインター（出力）
		&opCode); // データの種類（不使用）
	if (m_dwResult == ERROR_SUCCESS && pConnectInfo != NULL)// 情報の取得に成功した場合
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;// ssidをstd::wstringに変換
		std::wstring wssid = std::wstring(// SSIDをstd::wstringに変換
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID,		// SSIDのポインター
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID +		// SSIDのポインターの長さ
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength); // SSIDの長さ
		std::string ssid = m_converter.to_bytes(wssid);// ssidをstd::stringに変換
		m_currentWifiInfo.ssidLenght = (int)(ssid.length());// SSIDの長さを取得して数値化
		m_currentWifiInfo.signalQuality = pConnectInfo->wlanAssociationAttributes.wlanSignalQuality;// 電波の強さを取得
	}
	else// 情報の取得に失敗した場合
	{
		m_currentWifiInfo.ssidLenght = 0;// SSIDの長さを0に設定
		m_currentWifiInfo.signalQuality = 0; // 電波の強さを0に設定
		return;// 処理の中止
	}
	m_isGotCurrentWifiInfo = true;// 取得済みフラグをtrueに設定
	WlanFreeMemory(pConnectInfo); // メモリの解放
}
/*
*	@brief	スキャン結果の処理
*	@details スキャン結果を処理して、SSIDや電波の強さを取得する
*	@param なし
*	@return なし
*/
void Wifi::ProcessingScanResults()
{
	if (m_dwResult == ERROR_INVALID_PARAMETER ||	// パラメータが不正な時や
		m_dwResult == ERROR_INVALID_HANDLE)return;	// ハンドルが不正な時は処理を中止
	for (DWORD i = 0; i < m_pNetworkList->dwNumberOfItems; i++)// 取得した結果の数処理
	{
		m_network = m_pNetworkList->Network[i];// スキャン結果を取得
		m_ssid = (m_network.dot11Ssid.uSSIDLength == 0) // SSIDの長さが0の時
			? "非公開のネットワーク"// SSIDを非公開のネットワークに設定
			: m_converter.to_bytes(// SSIDを文字列に変換
				std::wstring(// SSIDをstd::wstringに変換
					m_network.dot11Ssid.ucSSID, // SSIDの先頭ポインター
					m_network.dot11Ssid.ucSSID + m_network.dot11Ssid.uSSIDLength));// 終点を計算して範囲指定
		if (m_exportSSIDs.find(m_ssid) != m_exportSSIDs.end())continue;// すでに表示したSSIDの時はスキップ
		m_exportSSIDs.insert(m_ssid); // 表示したSSIDをセット
		NetworkInfo networkInfo;// ネットワーク情報の構造体を初期化
		networkInfo.ssid = m_ssid; // SSIDをセット
		networkInfo.signalQuality = m_network.wlanSignalQuality; // 電波の強さをセット
		m_networkInfos.push_back(networkInfo); // ネットワーク情報をベクターに追加
	}
}
/*
*	@brief SSIDを無理やり数値に変換する関数
*	@details SSIDを無理やり数値に変換する関数（各文字のASCIIコードの合計）
*	@param ssid SSID
*	@return 変換した数値
*/
int Wifi::ConvertSsidToInt(const std::string& ssid)
{
	int sum = 0;// 合計値
	for (char c : ssid)sum += static_cast<int>(c); // 文字ごとのASCIIコードを合計
	return abs(sum);// 合計値を絶対値に変換して返す
}