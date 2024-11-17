/**
 * @file   Preparation.h
 *
 * @brief  表示準備に関するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/Preparation/Preparation.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
Preparation::Preparation()
{
}
/**
 * @brief デストラクタ
 */
Preparation::~Preparation()
{
}
// 表示準備
void Preparation::Set(std::set<std::string>& displayedSSIDs, int& count, std::vector<NetworkInfo>& networkInfos)
{
	// すでに表示したSSIDを管理するためのset
	displayedSSIDs.clear();
	// 表示した回数
	count = 0;
	// ネットワーク情報を格納するvector
	networkInfos.clear();
}
