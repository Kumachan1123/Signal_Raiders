/**
 * @file   Preparation.h
 *
 * @brief  表示準備に関するヘッダファイル
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
  // ネットワーク情報を格納する構造体
class Preparation
{
	// メンバ関数の宣言
public:
	// コンストラクタ
	Preparation();
	// デストラクタ
	~Preparation();
	// 表示準備
	void Set(std::set<std::string>& displayedSSIDs,int& count, std::vector<NetworkInfo>& networkInfos);
};
