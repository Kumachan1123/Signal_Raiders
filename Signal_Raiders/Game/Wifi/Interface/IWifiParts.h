/*
*	@file IWifiParts.h
*	@brief Wi-Fiの情報を取得するためのインターフェース
*/
#pragma once
// Windows APIを使うためのヘッダー
#include <windows.h>
#include <wlanapi.h>
// 文字コード変換用（wstring to string）
#include <locale>
#include <codecvt>
// STL（標準テンプレートライブラリ）
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
// WlanAPIライブラリをリンク
#pragma comment(lib, "wlanapi.lib")
// 構造体
// Wi-Fiの情報の構造体
struct NetworkInfo
{
	std::string ssid;// SSID（名前）
	DWORD signalQuality;// 電波の強さ
	DOT11_CIPHER_ALGORITHM defaultCipherAlgorithm;// 暗号アルゴリズム
	DOT11_AUTH_ALGORITHM defaultAuthAlgorithm;// 認証アルゴリズム
};
// 電波の強さで比較するための関数オブジェクト
struct CompareBySignalQuality
{
	// 比較演算子をオーバーロード
	bool operator()(const NetworkInfo& a, const NetworkInfo& b) const
	{
		// 強い順にソート
		return a.signalQuality > b.signalQuality;
	}
};

// SSIDや電波強度をもとにしたデータ構造体
struct Datas
{
	int SSIDLength;		//SSIDの文字数
	int SSIDValue;		//SSIDの各文字のASCIIコードの合計
	int WVlevel;	//電波の強さ
};

// 列挙型
// セキュリティの評価
enum SecurityLevel
{
	VerySecure,// 非常に安全
	Secure,// 安全
	Moderate,// 普通
	Unsafe,// 危険
	VeryUnsafe,// 非常に危険
};
// Cipher Algorithmの列挙型
namespace CipherAlgorithms
{
	enum CipherAlgorithm
	{
		WEP40 = 1,// WEP 40ビット
		TKIP = 2,// 一時的なキー交換方式
		CCMP = 4,// AESベースの暗号方式
		WEP104 = 5,// WEP 104bit（WEPの上位版）
		Unknown = 0xFF// 不明
	};
}