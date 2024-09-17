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
	, m_cipherSecurityLevel{}
	, m_authSecurityLevel{}

{
}
/**
 * @brief デストラクタ
 */
Output::~Output()
{
}
void Output::DisplayInformation(std::vector<NetworkInfo>& networkInfos,
								int& count,
								SecurityLevel& cipherSecurityLevel,
								SecurityLevel& authSecurityLevel)
{
	UNREFERENCED_PARAMETER(authSecurityLevel);
	UNREFERENCED_PARAMETER(cipherSecurityLevel);
	SetInformation(networkInfos);
	SetCount(count);
	//数値だけ出す
	for (const auto& networkInfo : m_networkInfos)
	{
		int WaveLevel = networkInfo.signalQuality;//電波の強さ
		int ASLevel = ConvertAuthSecurityLevelToValue(EvaluateSecurityLevel(networkInfo.defaultCipherAlgorithm));
		int CILevel = ConvertSecurityLevelToValue(EvaluateSecurityLevel(networkInfo.defaultCipherAlgorithm));
		Datas datas{};
		datas.WVlevel = WaveLevel;
		datas.ASlevel = ASLevel;
		datas.CIlevel = CILevel;
		m_datas.push_back(datas);


	}

	m_datas.clear();
}

// 暗号アルゴリズムを評価してセキュリティレベルを返す関数
SecurityLevel Output::EvaluateSecurityLevel(DWORD cipherAlgorithm)
{
	switch (cipherAlgorithm)
	{
		case CipherAlgorithms::WEP40:
			return Unsafe;
		case CipherAlgorithms::TKIP:
			return Moderate;
		case CipherAlgorithms::CCMP_AES128:
		case CipherAlgorithms::CCMP_AES256:
		case CipherAlgorithms::GCMP_AES128:
		case CipherAlgorithms::GCMP_AES256:
		case CipherAlgorithms::BIP_CMAC_256:
		case CipherAlgorithms::BIP_GMAC_128:
		case CipherAlgorithms::BIP_GMAC_256:
			return Secure;
		case CipherAlgorithms::WEP104:
			return Unsafe;
		case CipherAlgorithms::WPA_PSK:   // 追加: WPA-PSK
		case CipherAlgorithms::WPA2_PSK:  // 追加: WPA2-PSK
		case CipherAlgorithms::WPA3_PSK:  // 追加: WPA3-PSK
		case CipherAlgorithms::WPA3_SAE:  // 追加: WPA3-SAE
			return VerySecure; // 仮の評価です
		default:
			return VeryUnsafe;
	}



}
std::string Output::ConvertSecurityLevelToJapanese(DWORD cipherAlgorithm)
{
	switch (cipherAlgorithm)
	{
		case CipherAlgorithms::WEP40:
			return "WEP40";
		case CipherAlgorithms::TKIP:
			return "TKIP";
		case CipherAlgorithms::CCMP_AES128:
			return "CCMP_AES128";
		case CipherAlgorithms::CCMP_AES256:
			return "CCMP_AES256";
		case CipherAlgorithms::WEP104:
			return "WEP104";
		case CipherAlgorithms::WPA_PSK:
			return "WPA_PSK";
		case CipherAlgorithms::WPA2_PSK:
			return "WPA2_PSK";
		case CipherAlgorithms::WPA3_PSK:
			return "WPA3_PSK";
		case CipherAlgorithms::WPA3_SAE:
			return "WPA3_SAE";
		case CipherAlgorithms::GCMP_AES128:
			return "GCMP_AES128";
		case CipherAlgorithms::GCMP_AES256:
			return "GCMP_AES256";
		case CipherAlgorithms::BIP_CMAC_256:
			return "BIP_CMAC_256";
		case CipherAlgorithms::BIP_GMAC_128:
			return "GCMP_AES128";
		case CipherAlgorithms::BIP_GMAC_256:
			return "BIP_GMAC_256";
		default:
			return "Unknown";
	}
}
// セキュリティレベルを文字列に変換する関数
std::string Output::ConvertSecurityLevelToJapanese(SecurityLevel securityLevel)
{
	switch (securityLevel)
	{
		case VerySecure:
			return "非常に安全";
		case Secure:
			return "安全";
		case Moderate:
			return "それなり";
		case Unsafe:
			return "危険";
		case VeryUnsafe:
			return "非常に危険";
		default:
			return "不明";
	}
}
// セキュリティレベルを文字列に変換する関数
int Output::ConvertSecurityLevelToValue(SecurityLevel securityLevel)
{
	switch (securityLevel)
	{
		case VerySecure:
			return 1;
		case Secure:
			return 2;
		case Moderate:
			return 3;
		case Unsafe:
			return  4;
		case VeryUnsafe:
			return 5;
		default:
			return 6;
	}
}

// 認証アルゴリズムを評価してセキュリティレベルを返す関数
SecurityLevel Output::EvaluateAuthAlgorithmSecurity(DOT11_AUTH_ALGORITHM authAlgorithm)
{
	switch (authAlgorithm)
	{
		case DOT11_AUTH_ALGO_80211_OPEN: // オープンシステム認証
			return VeryUnsafe;
		case DOT11_AUTH_ALGO_80211_SHARED_KEY: // 共有キー認証
			return Moderate; // 仮の評価です
		case DOT11_AUTH_ALGO_WPA: // WPA認証
			return Moderate; // 仮の評価です
		case DOT11_AUTH_ALGO_WPA_PSK: // WPA-PSK認証
			return Moderate; // 仮の評価です
		case DOT11_AUTH_ALGO_WPA_NONE: // WPAでの認証なし
			return VeryUnsafe; // 仮の評価です
		case DOT11_AUTH_ALGO_RSNA: // RSNA認証
			return Secure; // 仮の評価です
		case DOT11_AUTH_ALGO_RSNA_PSK: // RSNA-PSK認証
			return Secure; // 仮の評価です
		default:
			return VeryUnsafe;
	}
}
// セキュリティレベルを文字列に変換する関数
std::string Output::ConvertAuthSecurityLevelToJapanese(SecurityLevel securityLevel)
{
	switch (securityLevel)
	{
		case VerySecure:
			return "非常に安全";
		case Secure:
			return "安全";
		case Moderate:
			return "それなり";
		case Unsafe:
			return "危険";
		case VeryUnsafe:
			return "非常に危険";
		default:
			return "不明";
	}
}

// セキュリティレベルを文字列に変換する関数
int Output::ConvertAuthSecurityLevelToValue(SecurityLevel securityLevel)
{
	switch (securityLevel)
	{
		case VerySecure:
			return 1;
		case Secure:
			return 2;
		case Moderate:
			return 3;
		case Unsafe:
			return 4;
		case VeryUnsafe:
			return 5;
		default:
			return 6;
	}
}

//数値化
void Output::SetValue()
{
	for (const auto& networkInfo : m_networkInfos)
	{
		int WaveLevel = networkInfo.signalQuality;//電波の強さ
		int ASLevel = ConvertAuthSecurityLevelToValue(EvaluateSecurityLevel(networkInfo.defaultCipherAlgorithm));
		int CILevel = ConvertSecurityLevelToValue(EvaluateSecurityLevel(networkInfo.defaultCipherAlgorithm));
		Datas datas{};
		datas.WVlevel = WaveLevel;
		datas.ASlevel = ASLevel;
		datas.CIlevel = CILevel;
		m_datas.push_back(datas);
	}
}