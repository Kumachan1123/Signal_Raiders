/**
 * @file   InitHandle.h
 *
 * @brief  ハンドルの初期化に関するヘッダファイル
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
class InitHandle
{
	// メンバ関数の宣言
public:
	// コンストラクタ
	InitHandle();
	// デストラクタ
	~InitHandle();
	void Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient);
	void Initialize(Wifi* pWifi);
};
