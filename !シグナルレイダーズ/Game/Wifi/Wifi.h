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
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
#include "Libraries/MyLib/DebugString.h"



// クラスの定義 ===============================================================
/**
  * @brief ゲーム
  */
class CommonResources;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
	class DebugString;
}

class Wifi
{

	// データメンバの宣言
private:

	// wi-fi関連の変数
		// クラスいろいろ
	std::unique_ptr<UpdateInfo>				 m_updateInfo;
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
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_converter;
	std::string m_ssid;



	// 最初の五秒はこの配列にWi-Fiの強さを格納する
	std::vector<int> m_preWifilevels;// Wi-Fiの強さ
	std::vector<int> m_preSSIDLengths;// ssidの文字数
	std::vector<int> m_preSSIDValues;// ssidの文字のASCIIコードの合計

	// 五秒後に以下の配列にWi-Fiの情報を格納する（一度入れたら情報は更新しない）
	std::vector<int> m_wifilevels;// Wi-Fiの強さ
	std::vector<int> m_ssidLengths;// ssidの文字の合計値
	std::vector<int> m_ssidValues;// ssidの文字のASCIIコードの合計

	float m_time;				  // Wi-Fi取得時間（５秒過ぎたら５秒で固定）

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
public:
	// 取得した情報を送る
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }//電波の強さを渡す
	std::vector<int> GetSSIDLengths()const { return m_ssidLengths; }//SSIDの文字数を渡す
	std::vector<int> GetSSIDValues()const { return m_ssidValues; }//SSIDの文字の合計値を渡す
	// 更新が終わったかどうかを調べるためのゲッター
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }
	std::vector<int> GetPreSSIDLengths()const { return m_preSSIDLengths; }
	std::vector<int> GetPreSSIDValues()const { return m_preSSIDValues; }
};
