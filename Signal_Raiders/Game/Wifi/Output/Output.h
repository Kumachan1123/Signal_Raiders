/**
 * @file   Output.h
 *
 * @brief  情報表示に関するヘッダファイル
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

class Output
{
private:

	// Wi-Fi情報を格納する可変長配列
	std::vector<NetworkInfo> m_networkInfos;
	// 表示回数
	int m_count;

	// 数値化したデータを格納する変数
	Datas m_data;
	std::vector<Datas> m_datas;
	// メンバ関数の宣言
public:
	// コンストラクタ
	Output();
	// デストラクタ
	~Output();
	// 情報を表示
	void DisplayInformation(std::vector<NetworkInfo>& networkInfos);
	// 情報を出力
	std::vector<NetworkInfo> GetInformation() { return m_networkInfos; };
	// 情報を入力
	void SetInformation(std::vector<NetworkInfo>& networkInfos) { m_networkInfos = networkInfos; };



	// データを渡す
	std::vector<Datas> GetDatas()const { return m_datas; }

	// SSIDを無理やり数値に変換する関数（各文字のASCIIコードの合計）
	int ConvertSsidToInt(const std::string& ssid);

};
