/**
 * @file   GetListOfInterfaces.h
 *
 * @brief  インターフェースのリスト取得するヘッダファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */

 // 多重インクルードの防止 =====================================================
#pragma once
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/Wifi.h"
// クラスの定義 ===============================================================
class Wifi;
class GetListOfInterfaces
{

	// メンバ関数の宣言
public:
	// コンストラクタ
	GetListOfInterfaces();

	// デストラクタ
	~GetListOfInterfaces();
	void Set(PWLAN_INTERFACE_INFO_LIST& pInterfaceList, DWORD& dwResult, HANDLE& hClient);
	// インターフェースのリスト取得
	void GetList(Wifi* pWifi);
};
