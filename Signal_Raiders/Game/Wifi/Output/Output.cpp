/*
*	@file Output.cpp
*	@brief 情報表示に関するソースファイル
*/
#include <pch.h>
#include "Output.h"
/*
*	@brief コンストラクタ
*	@details Wi-Fi情報を表示するクラスのコンストラクタ
*	@param なし
*	@return なし
*/
Output::Output()
	: m_networkInfos{}// Wi-Fi情報を格納する可変長配列
	, m_count{}// 表示回数
	, m_data{}// 数値化したデータを格納する変数
	, m_datas{}// 数値化したデータを格納する可変長配列
{
}
/*
 *	@brief デストラクタ
 *	@details Wi-Fi情報を表示するクラスのデストラクタ
 *	@param なし
 *	@return なし
 */
Output::~Output() {/*do nothing*/ }
/*
*	@brief 情報を表示する関数
*	@details Wi-Fi情報を表示する関数
*	@param networkInfos Wi-Fi情報を格納する可変長配列
*	@return なし
*/
void Output::DisplayInformation(std::vector<NetworkInfo>& networkInfos)
{
	UNREFERENCED_PARAMETER(networkInfos);// 未使用変数の警告を無視するマクロ
	for (const auto& networkInfo : m_networkInfos)// 数値だけ出す
	{
		int idLength = (int)(networkInfo.ssid.length());//SSIDの長さ(文字数)
		int waveLevel = networkInfo.signalQuality;//電波の強さ
		Datas datas{};// 数値化したデータを格納する変数
		datas.SSIDLength = idLength;// SSIDの長さ(文字数)
		datas.SSIDValue = ConvertSsidToInt(networkInfo.ssid);// SSIDを数値化
		datas.WVlevel = waveLevel;// 電波の強さ
		m_datas.push_back(datas);// 数値化したデータを格納する可変長配列に追加
	}
	m_datas.clear();// 数値化したデータを格納する可変長配列をクリア
}
/*
*	@brief SSIDを無理やり数値に変換する関数
*	@details SSIDを無理やり数値に変換する関数（各文字のASCIIコードの合計）
*	@param ssid SSID
*	@return 変換した数値
*/
int Output::ConvertSsidToInt(const std::string& ssid)
{
	int sum = 0;// 合計値
	for (char c : ssid)sum += static_cast<int>(c); // 文字ごとのASCIIコードを合計
	return abs(sum);// 合計値を絶対値に変換して返す
}
