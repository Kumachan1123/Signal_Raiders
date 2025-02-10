/**
 * @file   Output.h
 *
 * @brief  情報表示に関するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/Output/Output.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
Output::Output()
	: m_networkInfos{}
	, m_count{}
	, m_data{}
	, m_datas{}
{
}
/**
 * @brief デストラクタ
 */
Output::~Output()
{
}
void Output::DisplayInformation(std::vector<NetworkInfo>& networkInfos)
{

	UNREFERENCED_PARAMETER(networkInfos);
	//数値だけ出す
	for (const auto& networkInfo : m_networkInfos)
	{
		int idLength = (int)(networkInfo.ssid.length());//SSIDの長さ(文字数)
		int waveLevel = networkInfo.signalQuality;//電波の強さ

		Datas datas{};
		datas.SSIDLength = idLength;
		datas.SSIDValue = ConvertSsidToInt(networkInfo.ssid);
		datas.WVlevel = waveLevel;

		m_datas.push_back(datas);


	}

	m_datas.clear();
}




int Output::ConvertSsidToInt(const std::string& ssid)
{
	int sum = 0;
	for (char c : ssid)
	{
		sum += static_cast<int>(c); // 文字ごとのASCIIコードを合計
	}
	return abs(sum);
}




