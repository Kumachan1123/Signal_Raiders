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
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Wifi.h"
#include "Game/CommonResources.h"



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
	// クラス定数の宣言
public:


	// データメンバの宣言
private:

	// wi-fi関連の変数
		// クラスいろいろ
	std::unique_ptr<UpdateInfo>				 m_updateInfo;
	std::unique_ptr<Output>					 m_output;
	std::unique_ptr<ReleaseMemory>			 m_memory;
	// Wi-Fi関連の宣言
	DWORD  dwMaxClient; // 最大のクライアント数
	HANDLE hClient;
	DWORD  dwCurVersion;
	DWORD  dwResult;
	// インターフェースのリスト取得
	PWLAN_INTERFACE_INFO_LIST pInterfaceList;
	// スキャン結果の取得
	PWLAN_AVAILABLE_NETWORK_LIST pNetworkList;
	// すでに表示したSSIDを管理するためのset
	std::set<std::string> displayedSSIDs;
	// 表示した回数
	int count;
	// ネットワーク情報を格納するvector
	std::vector<NetworkInfo> m_networkInfos;
	// スキャン結果の処理に使う変数
	WLAN_AVAILABLE_NETWORK network;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string ssid;
	// 暗号アルゴリズムのセキュリティレベルを表示
	SecurityLevel cipherSecurityLevel;
	// 認証アルゴリズムのセキュリティレベルを表示
	SecurityLevel authSecurityLevel;
	// メンバ関数の宣言

	// 最初の五秒はこの配列にWi-Fiの強さを格納する
	std::vector<int> m_preWifilevels;
	// 五秒後にこの配列にWi-Fiの強さを格納する（一度入れたら情報は更新しない）
	std::vector<int> m_wifilevels;
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
	// 描画処理
	void Render(mylib::DebugString* debugString);
	// クリア
	void Clear();
public:
	// 取得したWi-Fiを受け取る
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }
	// 更新が終わったかどうかを調べるためのゲッター
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }
};
