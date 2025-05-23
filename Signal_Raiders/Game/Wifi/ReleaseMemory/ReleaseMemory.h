/*
*	@file	ReleaseMemory.h
*	@brief	メモリの解放とハンドルのクローズに関するヘッダーファイル
 */
#pragma once
 // 標準ライブラリ
#include <sstream>
// 自作ヘッダーファイル
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Screen.h"
class ReleaseMemory
{
public:// public関数
	ReleaseMemory();// コンストラクタ
	~ReleaseMemory();// デストラクタ
	void FreeMemoryAndCloseHandle(// メモリの解放とハンドルのクローズ
		PWLAN_INTERFACE_INFO_LIST& pInterfaceList, HANDLE& hClient,
		std::vector<NetworkInfo>& networkInfos, std::set<std::string>& displayedSSIDs);
};
