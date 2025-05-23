/*
*	@file	ReleaseMemory.cpp
*	@brief	メモリの解放とハンドルのクローズに関するソースファイル
*/
// ヘッダファイルの読み込み ===================================================
#include "pch.h"
#include "ReleaseMemory.h"
/*
*	@brief コンストラクタ
*	@details メモリの解放とハンドルのクローズに関するクラスのコンストラクタ(何もしない）
*	@param なし
*	@return なし
*/
ReleaseMemory::ReleaseMemory() {/*do nothing*/ }
/*
 *	@brief デストラクタ
 *	@details メモリの解放とハンドルのクローズに関するクラスのデストラクタ(何もしない）
 *	@param なし
 *	@return なし
 */
ReleaseMemory::~ReleaseMemory() {/*do nothing*/ }
/*
*	@brief メモリの解放とハンドルのクローズ
*	@details メモリの解放とハンドルのクローズを行う関数
*	@param pInterfaceList インターフェースリスト
*	@param hClient ハンドル
*	@param networkInfos Wi-Fi情報を格納する可変長配列
*	@param displayedSSIDs 表示済みSSIDのセット
*	@return なし
*/
void ReleaseMemory::FreeMemoryAndCloseHandle(
	PWLAN_INTERFACE_INFO_LIST& pInterfaceList,
	HANDLE& hClient,
	std::vector<NetworkInfo>& networkInfos,
	std::set<std::string>& displayedSSIDs)
{
	WlanFreeMemory(pInterfaceList);// インターフェースリストのメモリを解放
	WlanCloseHandle(hClient, NULL);// ハンドルをクローズ
	networkInfos.clear();// Wi-Fi情報を格納する可変長配列をクリア
	displayedSSIDs.clear();// 表示済みSSIDのセットをクリア
}