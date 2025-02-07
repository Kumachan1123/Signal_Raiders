/**
 * @file   Wifi.h
 *
 * @brief  Wi-Fiの電波の強さを得るソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "pch.h"
#include <memory>
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Wifi/Parameters/WiFiParameters.h"
#include "Game/CommonResources.h"

// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
Wifi::Wifi()
	: m_dwCurVersion{}
	, m_hClient{}
	, m_dwMaxClient{}
	, m_dwResult{}
	, m_pInterfaceList{}
	, m_pNetworkList{}
	, m_displayedSSIDs{}
	, m_count{}
	, m_networkInfos{}
	, m_network{}
	, m_ssid{}
	, m_converter{}
	, m_output{ nullptr }
	, m_memory{ nullptr }
	, m_time(0.0f)
{
}
/**
 * @brief デストラクタ
 */
Wifi::~Wifi()
{
}
// 初期化処理
void Wifi::Initialize()
{

	m_output = std::make_unique<Output>();
	m_memory = std::make_unique<ReleaseMemory>();
}
// 更新処理
void Wifi::Update(float elapsedTime)
{
	// 五秒経ったら更新終了
	if (m_time >= WiFiParameters::MAX_TIME)
	{
		return;
	}
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
	// 表示準備
	SetUp();
	// スキャン結果の処理
	ProcessingScanResults();
	// 電波の強さでソート
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// ソート後の情報を表示
	m_output->DisplayInformation(m_networkInfos, m_count);
	m_output->SetInformation(m_networkInfos);
	// 時間を計測
	m_time += elapsedTime;
	// Wi-Fiを取得できない状態の時
	if (m_dwResult != ERROR_SUCCESS)
	{
		if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
		{
			for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
			{
				// 五秒経ったら更新終了
				if (m_time >= WiFiParameters::MAX_TIME)
				{

					break;
				}
				m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// 電波の強さを100に設定

				m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// 敵の種類を0(通常)に設定
			}
		}
		m_wifilevels = m_preWifilevels;		// 電波の強さを可変長配列に登録
		m_enemyTypes = m_preEnemyTypes;		// 敵の種類を可変長配列に登録
	}
	else// Wi-Fiを取得できる状態の時
	{
		// 取得した数が0の時
		if (m_networkInfos.size() == 0)
		{
			if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
			{
				for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
				{
					// 五秒経ったら更新終了
					if (m_time >= WiFiParameters::MAX_TIME)
					{
						break;
					}
					m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// 電波の強さを100に設定
					m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// 敵の種類を0(通常)に設定
				}
			}
			m_wifilevels = m_preWifilevels;		// 電波の強さを可変長配列に登録
			m_enemyTypes = m_preEnemyTypes;		// 敵の種類を可変長配列に登録
		}
		// 取得した数が1以上の時
		// 数値だけ出す
		for (const auto& networkInfo : m_networkInfos)
		{
			// 五秒経ったら更新終了
			if (m_time >= WiFiParameters::MAX_TIME)
			{
				m_time = WiFiParameters::MAX_TIME;
				break;
			}
			//ssidの文字数を可変長配列に登録
			int ssidLength = (int)(networkInfo.ssid.length());
			//ssidの文字のASCIIコードの合計を可変長配列に登録
			int ssidValue = m_output->ConvertSsidToInt(networkInfo.ssid);
			int enemyType = (ssidValue % ssidLength) % ENEMY_TYPE_MAX;
			//電波の強さを可変長配列に登録
			m_preWifilevels.push_back(networkInfo.signalQuality);

			m_preEnemyTypes.push_back(enemyType);// 敵の種類を設定	
			m_wifilevels = m_preWifilevels;		// 電波の強さを可変長配列に登録
			m_enemyTypes = m_preEnemyTypes;		// 敵の種類を可変長配列に登録

		}
	}

}

// クリア処理
void Wifi::Clear()
{
	if (m_dwResult != ERROR_SUCCESS || m_time >= WiFiParameters::MAX_TIME)
	{

		return;
	}
	// メモリの解放とハンドルのクローズ
	m_memory->FreeMemoryAndCloseHandle(m_pInterfaceList, m_hClient, m_networkInfos, m_displayedSSIDs);
	m_preWifilevels.clear();
	m_preEnemyTypes.clear();
}

// ハンドルの初期化
void Wifi::InitHandle()
{
	m_dwMaxClient = 2; // 最大のクライアント数
	m_hClient = NULL;
	m_dwCurVersion = 0;
	m_dwResult = 0;
}

// バージョンの確認
void Wifi::Checkversion()
{
	m_dwResult = WlanOpenHandle(m_dwMaxClient, NULL, &m_dwCurVersion, &m_hClient);
	if (m_dwResult != ERROR_SUCCESS) return;
}

// インターフェースのリスト取得
void Wifi::GetInterfacesList()
{
	m_pInterfaceList = NULL;
	m_dwResult = WlanEnumInterfaces(m_hClient, NULL, &m_pInterfaceList);

	if (m_dwResult != ERROR_SUCCESS)
	{
		WlanCloseHandle(m_hClient, NULL);
		return;
	}
}

// スキャンの開始
void Wifi::StartScan()
{
	m_dwResult = WlanScan(m_hClient, &m_pInterfaceList->InterfaceInfo[0].InterfaceGuid, NULL, NULL, NULL);
	if (m_dwResult != ERROR_SUCCESS)
	{
		WlanFreeMemory(m_pInterfaceList);
		WlanCloseHandle(m_hClient, NULL);
		return;
	}
}

// スキャン結果の取得
void Wifi::GetScanResults()
{
	m_pNetworkList = NULL;
	m_dwResult = WlanGetAvailableNetworkList(
		m_hClient,
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,
		0,
		NULL,
		&m_pNetworkList
	);
	if (m_dwResult != ERROR_SUCCESS)
	{

		return;
	}
}

// 表示準備
void Wifi::SetUp()
{
	m_displayedSSIDs.clear();
	m_networkInfos.clear();
}

// スキャン結果の処理
void Wifi::ProcessingScanResults()
{
	for (DWORD i = 0; i < m_pNetworkList->dwNumberOfItems; i++)
	{
		m_network = m_pNetworkList->Network[i];
		m_ssid = (m_network.dot11Ssid.uSSIDLength == 0) ? "非公開のネットワーク" : m_converter.to_bytes(std::wstring(m_network.dot11Ssid.ucSSID, m_network.dot11Ssid.ucSSID + m_network.dot11Ssid.uSSIDLength));
		if (m_displayedSSIDs.find(m_ssid) != m_displayedSSIDs.end())
		{
			continue;
		}
		m_displayedSSIDs.insert(m_ssid);
		NetworkInfo networkInfo;
		networkInfo.ssid = m_ssid;
		networkInfo.signalQuality = m_network.wlanSignalQuality;
		m_networkInfos.push_back(networkInfo);
	}
}

