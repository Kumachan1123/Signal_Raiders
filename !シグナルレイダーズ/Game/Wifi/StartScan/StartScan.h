/**
 * @file   StartScan.h
 *
 * @brief  スキャンの開始に関するヘッダファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // 多重インクルードの防止 =====================================================
#pragma once
#include <windows.h>
#include <wlanapi.h>
#include <locale>
#include <codecvt>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#pragma comment(lib, "wlanapi.lib")
// クラスの定義 ===============================================================
/**
  * @brief ゲーム
  */
class StartScan
{
	// メンバ関数の宣言
public:
	// コンストラクタ
	StartScan();
	// デストラクタ
	~StartScan();
	// スキャンの開始
	void Set(DWORD&  dwResult,HANDLE& hClient, PWLAN_INTERFACE_INFO_LIST& pInterfaceList);
};
