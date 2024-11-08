#pragma once
#include <windows.h>
#include <wlanapi.h>
#include <locale>
#include <codecvt>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#pragma comment(lib, "wlanapi.lib")
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
	bool operator()(const NetworkInfo& a, const NetworkInfo& b) const
	{
		return a.signalQuality > b.signalQuality;
	}
};
// セキュリティの評価
enum SecurityLevel
{
	VerySecure,
	Secure,
	Moderate,
	Unsafe,
	VeryUnsafe
};
// Cipher Algorithmの列挙型
namespace CipherAlgorithms
{
	enum CipherAlgorithm
	{
		WEP40 = 1,
		TKIP = 2,
		CCMP = 4,
		WEP104 = 5,
		Unknown = 0xFF
	};
}

struct Datas
{
	int SSIDLength;		//SSIDの文字数
	int SSIDValue;		//SSIDの各文字のASCIIコードの合計
	int WVlevel;	//電波の強さ

};