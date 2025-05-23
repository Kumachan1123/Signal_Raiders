/*
*	@file		Output.h
*	@brief		情報表示に関するソースファイル
*/
#pragma once
// 標準ライブラリ
#include <sstream>
// 自作ヘッダーファイル
#include "Game/Screen.h"
#include "Game/Wifi/Interface/IWifiParts.h"
class Output
{
public:// public関数
	Output();// コンストラクタ
	~Output();// デストラクタ
	void DisplayInformation(std::vector<NetworkInfo>& networkInfos);// 情報を表示
	std::vector<NetworkInfo> GetInformation() { return m_networkInfos; };// 情報を出力
	void SetInformation(std::vector<NetworkInfo>& networkInfos) { m_networkInfos = networkInfos; }// 情報をセット
	std::vector<Datas> GetDatas()const { return m_datas; }// データを渡す
	int ConvertSsidToInt(const std::string& ssid);// SSIDを無理やり数値に変換する関数（各文字のASCIIコードの合計）
private:// private変数
	std::vector<NetworkInfo> m_networkInfos;// Wi-Fi情報を格納する可変長配列
	int m_count;// 表示回数
	Datas m_data;// 数値化したデータを格納する変数
	std::vector<Datas> m_datas;// 数値化したデータを格納する可変長配列
};
