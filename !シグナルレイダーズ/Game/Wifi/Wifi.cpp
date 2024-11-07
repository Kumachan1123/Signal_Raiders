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
	: dwCurVersion{}
	, hClient{}
	, dwMaxClient{}
	, dwResult{}
	, pInterfaceList{}
	, pNetworkList{}
	, displayedSSIDs{}
	, count{}
	, m_networkInfos{}
	, network{}
	, ssid{}
	, cipherSecurityLevel{}
	, authSecurityLevel{}
	, converter{}
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
	m_updateInfo->Set(dwResult, dwMaxClient, dwCurVersion, hClient, pInterfaceList, pNetworkList, m_networkInfos, network, ssid, displayedSSIDs, converter, count);
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// ソート後の情報を表示
	m_output->DisplayInformation(m_networkInfos, count, cipherSecurityLevel, authSecurityLevel);
	m_output->SetInformation(m_networkInfos);
	// 時間を計測
	m_time += elapsedTime;
	// Wi-Fiを取得できない状態の時
	if (dwResult != ERROR_SUCCESS)
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
				m_preWifilevels.push_back(100);

			}
		}

		m_wifilevels = m_preWifilevels;
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
					m_preWifilevels.push_back(100);
				}
			}
			m_wifilevels = m_preWifilevels;
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
			m_wifilevels = m_preWifilevels;
		}
	}

}

void Wifi::Clear()
{
	if (dwResult != ERROR_SUCCESS)
	{
		return;
	}
	// メモリの解放とハンドルのクローズ
	m_memory->FreeMemoryAndCloseHandle(pInterfaceList, hClient, m_networkInfos, displayedSSIDs);
	m_preWifilevels.clear();
}

