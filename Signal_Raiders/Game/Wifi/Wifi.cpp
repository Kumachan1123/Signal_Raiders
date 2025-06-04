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
	// 五秒経ったら更新終了
	if (m_time >= WiFiParameters::MAX_TIME)return;
	// Wi-Fiハンドルの初期化
	InitHandle();
	// バージョンの確認
	Checkversion();
	// インターフェースのリスト取得
	GetInterfacesList();
	// スキャンの開始
	StartScan();
	// スキャン結果の取得
	GetScanResults();
	// 現在接続しているWi-Fi情報を取得
	GetCurrentWifiInfo();
	// 出力準備
	ClearExportInfo();
	// スキャン結果の処理
	ProcessingScanResults();
	// 電波の強さでソート
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// 時間を加算
	m_time += elapsedTime;
	// Wi-Fiが取得できたかどうかに応じて分岐
	if (m_dwResult != ERROR_SUCCESS)// Wi-Fiを取得できない状態の時
	{
		// 設定された敵の最大値未満の場合
		if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
		{
			//最大値になるまで繰り返す
			for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
			{
				// 五秒経ったら更新終了
				if (m_time >= WiFiParameters::MAX_TIME)break;
				// 電波の強さを100に設定
				m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);
				// 敵の種類を0(通常)に設定
				m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);
			}
		}
		// 電波の強さを可変長配列に登録
		m_wifilevels = m_preWifilevels;
		// 敵の種類を可変長配列に登録
		m_enemyTypes = m_preEnemyTypes;
	}
	else// Wi-Fiを取得できる状態の時
	{
		// 取得した数が0の時
		if (m_networkInfos.size() == 0)
		{
			// 設定された敵の最大値未満の場合
			if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
			{
				//最大値になるまで繰り返す
				for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
				{
					// 五秒経ったら更新終了
					if (m_time >= WiFiParameters::MAX_TIME)break;
					// 電波の強さを100に設定
					m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);
					// 敵の種類を0(通常)に設定
					m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);
				}
			}
			// 電波の強さを可変長配列に登録
			m_wifilevels = m_preWifilevels;
			// 敵の種類を可変長配列に登録
			m_enemyTypes = m_preEnemyTypes;
		}
		// 以下、正常にWi-Fiを取得できた場合
		// 取得した数だけ繰り返す
		for (const auto& networkInfo : m_networkInfos)
		{
			// 一定期間経ったら更新終了
			if (m_time >= WiFiParameters::MAX_TIME)
			{
				// 時間を最大値に固定
				m_time = WiFiParameters::MAX_TIME;
				// ループを抜ける
				break;
			}
			// ssidの文字数を可変長配列に登録
			int ssidLength = (int)(networkInfo.ssid.length());
			// ssidの文字のASCIIコードの合計を可変長配列に登録
			int ssidValue = ConvertSsidToInt(networkInfo.ssid);
			// (文字数×文字のASCIIコードの合計)/ENEMY_TYPE_MAXの余りを敵の種類に変換
			int enemyType = (ssidValue * ssidLength) % ENEMY_TYPE_MAX;
			// 電波の強さを可変長配列に登録
			m_preWifilevels.push_back(networkInfo.signalQuality);
			// 敵の種類を設定	
			m_preEnemyTypes.push_back(enemyType);
			// 電波の強さを可変長配列に登録
			m_wifilevels = m_preWifilevels;
			// 敵の種類を可変長配列に登録
			m_enemyTypes = m_preEnemyTypes;
		}
	}
	// 各種ポインターをクリア
	Clear();
}
/*
*	@brief	クリア
*	@details Wi-Fiクラスのクリア処理
*	@param なし
*	@return なし
*/
void Wifi::Clear()
{
	// 五秒経ったら更新終了
	if (m_dwResult != ERROR_SUCCESS || m_time >= WiFiParameters::MAX_TIME)return;
	// インターフェースリストのメモリを解放
	WlanFreeMemory(m_pInterfaceList);
	// ハンドルをクローズ
	WlanCloseHandle(m_hClient, NULL);
	// Wi-Fi情報を格納する可変長配列をクリア
	m_networkInfos.clear();
	// 表示済みSSIDのセットをクリア
	m_exportSSIDs.clear();
	// 電波の強さをクリア
	m_preWifilevels.clear();
	// 敵の種類をクリア
	m_preEnemyTypes.clear();
}
/*
*	@brief	ハンドルの初期化
*	@details Wi-Fiクラスのハンドルの初期化
*	@param なし
*	@return なし
*/
void Wifi::InitHandle()
{
	// 最大のクライアント数
	m_dwMaxClient = 2;
	// クライアントハンドル
	m_hClient = NULL;
	// WLANのバージョン
	m_dwCurVersion = 0;
	// 結果
	m_dwResult = 0;
}
/*
*	@brief	バージョンの確認
*	@details WlanAPIのクライアントバージョンを確認
*	@param なし
*	@return なし
*/
void Wifi::Checkversion()
{
	// WlanAPIのクライアントバージョンを確認
	m_dwResult = WlanOpenHandle(
		m_dwMaxClient,     // 要求するクライアントのバージョン（入力）
		NULL,              // 予約（NULLでOK）
		&m_dwCurVersion,   // 実際に使えるバージョンがここに返る（出力）
		&m_hClient);       // 取得されたクライアントハンドル（出力）
	// バージョンの確認結果が成功でない場合は処理を中止
	if (m_dwResult != ERROR_SUCCESS) return;
}
/*
*	@brief インターフェースのリスト取得
*	@details インターフェースのリストを取得
*/
void Wifi::GetInterfacesList()
{
	// リストポインターを初期化
	m_pInterfaceList = NULL;
	// インターフェースのリスト取得
	m_dwResult = WlanEnumInterfaces(m_hClient, NULL, &m_pInterfaceList);
	// インターフェースのリスト取得に失敗した場合は処理を中止
	if (m_dwResult != ERROR_SUCCESS)
	{
		// ハンドルのクローズ
		WlanCloseHandle(m_hClient, NULL);
		// 処理の中止
		return;
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
	// 指定したインターフェースでWi-Fiスキャンを開始
	m_dwResult = WlanScan(
		m_hClient, // Wi-Fiクライアントのハンドル
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid, // 使用するアダプタのGUID
		NULL,// スキャンのオプション（特に指定なし）
		NULL, // 予約引数（不使用）
		NULL);// 予約引数（不使用）
	// スキャンの開始結果が成功でない場合は処理を中止
	if (m_dwResult != ERROR_SUCCESS)
	{
		// メモリの解放
		WlanFreeMemory(m_pInterfaceList);
		// ハンドルのクローズ
		WlanCloseHandle(m_hClient, NULL);
		// 処理の中止
		return;
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
	// ネットワークリストのポインターを初期化
	m_pNetworkList = NULL;
	// 使用可能なネットワークの一覧を取得
	m_dwResult = WlanGetAvailableNetworkList(
		m_hClient,// Wi-Fiクライアントのハンドル
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// 使うインターフェースのGUID
		0,// オプションのフラグ（特に指定なし）
		NULL,// 使用する言語（基本はNULL）
		&m_pNetworkList);// ネットワークリスト
	// スキャン結果の取得に失敗した場合は処理を中止
	if (m_dwResult != ERROR_SUCCESS)return;
}

/*
*	@brief	外部に渡すWi-Fi情報を初期化
*	@details ゲームのロジックや他のシステムに渡すためのSSIDや電波情報をクリアする
*	@param なし
*	@return なし
*/
void Wifi::ClearExportInfo()
{
	// 表示するSSIDをクリア
	m_exportSSIDs.clear();
	// ネットワーク情報をクリア
	m_networkInfos.clear();
}
/*
*	@brief	現在接続しているWi-Fi情報を取得
*	@details 現在接続しているWi-FiのSSIDや電波の強さを取得する
*	@param なし
*	@return なし
*/
void Wifi::GetCurrentWifiInfo()
{
	// すでに取得済みなら終了
	if (m_isGotCurrentWifiInfo)return;
	// 接続情報のポインター
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	// 接続情報のサイズ
	DWORD connectInfoSize = 0;
	// データの種類
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;
	// 現在接続しているWi-Fi情報を取得
	m_dwResult = WlanQueryInterface(
		m_hClient,// Wi-Fiクライアントのハンドル
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// 使用するインターフェースのGUID
		wlan_intf_opcode_current_connection,// 現在の接続情報を取得
		NULL,// 予約引数（不使用）
		&connectInfoSize,// 接続情報のサイズ（出力）
		(PVOID*)&pConnectInfo,// 接続情報のポインター（出力）
		&opCode); // データの種類（不使用）
	// 接続状態やハンドルの状態に応じて分岐
	if (m_dwResult == ERROR_SUCCESS && pConnectInfo != NULL)// 接続情報の取得結果が成功した場合
	{
		// ssidをstd::wstringに変換
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		// SSIDをstd::wstringに変換
		std::wstring wssid = std::wstring(
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID,		// SSIDのポインター
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID +		// SSIDのポインターの長さ
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength); // SSIDの長さ
		// ssidをstd::stringに変換
		std::string ssid = m_converter.to_bytes(wssid);
		// SSIDの長さを取得して数値化
		m_currentWifiInfo.ssidLenght = (int)(ssid.length());
		// 電波の強さを取得
		m_currentWifiInfo.signalQuality = pConnectInfo->wlanAssociationAttributes.wlanSignalQuality;
	}
	else// 情報の取得に失敗した場合
	{
		// SSIDの長さを0に設定
		m_currentWifiInfo.ssidLenght = 0;
		// 電波の強さを0に設定
		m_currentWifiInfo.signalQuality = 0;
		// 処理の中止
		return;
	}
	// 取得済みフラグをtrueに設定
	m_isGotCurrentWifiInfo = true;
	// メモリの解放
	WlanFreeMemory(pConnectInfo);
}
/*
*	@brief	スキャン結果の処理
*	@details スキャン結果を処理して、SSIDや電波の強さを取得する
*	@param なし
*	@return なし
*/
void Wifi::ProcessingScanResults()
{
	// パラメータが不正な時やハンドルが不正な時は処理を中止
	if (m_dwResult == ERROR_INVALID_PARAMETER || m_dwResult == ERROR_INVALID_HANDLE)return;
	// 取得した結果の数処理
	for (DWORD i = 0; i < m_pNetworkList->dwNumberOfItems; i++)
	{
		// スキャン結果を取得
		m_network = m_pNetworkList->Network[i];
		// SSIDの長さが0の時
		m_ssid = (m_network.dot11Ssid.uSSIDLength == 0)
			? "非公開のネットワーク"// SSIDを非公開のネットワークに設定
			: m_converter.to_bytes(// SSIDを文字列に変換
				std::wstring(// SSIDをstd::wstringに変換
					m_network.dot11Ssid.ucSSID, // SSIDの先頭ポインター
					m_network.dot11Ssid.ucSSID + m_network.dot11Ssid.uSSIDLength));// 終点を計算して範囲指定
		// すでに表示したSSIDの時はスキップ
		if (m_exportSSIDs.find(m_ssid) != m_exportSSIDs.end())continue;
		// 表示したSSIDをセット
		m_exportSSIDs.insert(m_ssid);
		// ネットワーク情報の構造体を初期化
		NetworkInfo networkInfo;
		// SSIDをセット
		networkInfo.ssid = m_ssid;
		// 電波の強さをセット
		networkInfo.signalQuality = m_network.wlanSignalQuality;
		// ネットワーク情報をベクターに追加
		m_networkInfos.push_back(networkInfo);
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
	// 合計値
	int sum = 0;
	// 文字ごとのASCIIコードを合計
	for (char c : ssid)sum += static_cast<int>(c);
	// 合計値を絶対値に変換して返す
	return abs(sum);
}