/**
 * @file   CheckVersion.h
 *
 * @brief  Wi-Fiのバージョンを確認するヘッダファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */

// 多重インクルードの防止 =====================================================
#pragma once

#include "Game/Wifi/Interface/IWifiParts.h"

// クラスの定義 ===============================================================
/**
  * @brief ゲーム
  */
class CheckVersion 
{
// クラス定数の宣言
public:
	

// データメンバの宣言
private:
	
	
// メンバ関数の宣言
public:
	// コンストラクタ
	CheckVersion();

	// デストラクタ
	~CheckVersion();


	
	void Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient);
	
};
