/**
 * @file   Wifi.h
 *
 * @brief  Wi-Fiの電波の強さ・認証と暗号のセキュリティレベルを得るソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "pch.h"
#include <memory>
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Wifi.h"
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
	, m_updateInfo{ nullptr }
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
	m_updateInfo = std::make_unique<UpdateInfo>();
	m_output = std::make_unique<Output>();
	m_memory = std::make_unique<ReleaseMemory>();
}
// 更新処理
void Wifi::Update(float elapsedTime)
{

	// 全データを更新
	m_updateInfo->Set(m_dwResult, m_dwMaxClient, m_dwCurVersion, m_hClient, m_pInterfaceList, m_pNetworkList, m_networkInfos, m_network, m_ssid, m_displayedSSIDs, m_converter, m_count);
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// ソート後の情報を表示
	m_output->DisplayInformation(m_networkInfos, m_count);
	m_output->SetInformation(m_networkInfos);
	// 時間を計測
	m_time += elapsedTime;
	// Wi-Fiを取得できない状態の時
	if (m_dwResult != ERROR_SUCCESS)
	{
		if (m_preWifilevels.size() < 30)
		{
			for (int index = 0; index < 30; index++)
			{
				// 五秒経ったら更新終了
				if (m_time >= 5.0f)
				{

					break;
				}
				m_preWifilevels.push_back(100);// 電波の強さを100に設定
				m_preSSIDLengths.push_back(10);// ssidの文字数を10に設定
				m_preSSIDValues.push_back(1000);// ssidの文字のASCIIコードの合計を1000に設定

			}
		}
		m_wifilevels = m_preWifilevels;
		m_ssidLengths = m_preSSIDLengths;
		m_ssidValues = m_preSSIDValues;
	}
	else// Wi-Fiを取得できる状態の時
	{
		// 取得した数が0の時
		if (m_networkInfos.size() == 0)
		{
			if (m_preWifilevels.size() < 30)
			{
				for (int index = 0; index < 30; index++)
				{
					// 五秒経ったら更新終了
					if (m_time >= 5.0f)
					{
						break;
					}
					m_preWifilevels.push_back(100);// 電波の強さを100に設定
					m_preSSIDLengths.push_back(10);// ssidの文字数を10に設定
					m_preSSIDValues.push_back(1000);// ssidの文字のASCIIコードの合計を1000に設定

				}
			}
			m_wifilevels = m_preWifilevels;
			m_ssidLengths = m_preSSIDLengths;
			m_ssidValues = m_preSSIDValues;
		}
		// 取得した数が1以上の時
		//数値だけ出す
		for (const auto& networkInfo : m_networkInfos)
		{
			// 五秒経ったら更新終了
			if (m_time >= 5.0f)
			{
				m_time = 5.0f;
				break;
			}
			//電波の強さを可変長配列に登録
			m_preWifilevels.push_back(networkInfo.signalQuality);
			//ssidの文字数を可変長配列に登録
			m_preSSIDLengths.push_back((int)(networkInfo.ssid.length()));
			//ssidの文字のASCIIコードの合計を可変長配列に登録
			m_preSSIDValues.push_back(m_output->ConvertSsidToInt(networkInfo.ssid));

			m_wifilevels = m_preWifilevels;
			m_ssidLengths = m_preSSIDLengths;
			m_ssidValues = m_preSSIDValues;
		}
	}

}

void Wifi::Clear()
{
	if (m_dwResult != ERROR_SUCCESS)
	{
		return;
	}
	// メモリの解放とハンドルのクローズ
	m_memory->FreeMemoryAndCloseHandle(m_pInterfaceList, m_hClient, m_networkInfos, m_displayedSSIDs);
	m_preWifilevels.clear();
}

