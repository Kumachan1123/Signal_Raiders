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
	, m_UpdateInfo{ nullptr }
	, m_Output{ nullptr }
	, m_Memory{ nullptr }
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
	m_UpdateInfo = std::make_unique<UpdateInfo>();
	m_Output = std::make_unique<Output>();
	m_Memory = std::make_unique<ReleaseMemory>();
}
// 更新処理
void Wifi::Update(float elapsedTime)
{
	// 全データを更新
	m_UpdateInfo->Set(dwResult, dwMaxClient, dwCurVersion, hClient, pInterfaceList, pNetworkList, m_networkInfos, network, ssid, displayedSSIDs, converter, count, m_preWifilevels);
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// ソート後の情報を表示
	m_Output->DisplayInformation(m_networkInfos, count, cipherSecurityLevel, authSecurityLevel);
	m_Output->SetInformation(m_networkInfos);
	// 時間を計測
	m_time += elapsedTime;
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
// 描画処理
void Wifi::Render(mylib::DebugString* debugString)
{
#ifdef _DEBUG
	debugString->AddString("Size:%i", m_preWifilevels.size());
	for (const auto& datas : m_wifilevels)
	{
		//debugString->AddString("Signal Quality:%i ", datas);
	}
#endif
	// メモリの解放とハンドルのクローズ
	m_Memory->FreeMemoryAndCloseHandle(pInterfaceList, hClient, m_networkInfos, displayedSSIDs);
	m_preWifilevels.clear();
}

