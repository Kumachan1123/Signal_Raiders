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
	// 暗号アルゴリズム評価
	SecurityLevel m_cipherSecurityLevel;
	// 認証アルゴリズム評価
	SecurityLevel m_authSecurityLevel;
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
	void DisplayInformation(std::vector<NetworkInfo>& networkInfos,
							int& count,
							SecurityLevel& cipherSecurityLevel,
							SecurityLevel& authSecurityLevel  );
	// 情報を出力
	std::vector<NetworkInfo> GetInformation() { return m_networkInfos; };
	// 情報を入力
	void SetInformation(std::vector<NetworkInfo>& networkInfos) { m_networkInfos = networkInfos; };
	// 表示回数を出力
	int GetCount() { return m_count; };
	// 表示回数を入力
	void SetCount(int count) { m_count = count; };
	// 暗号アルゴリズム評価出力
	SecurityLevel GetCipherSecurityLevel() { return m_cipherSecurityLevel; };
	// 暗号アルゴリズム評価入力
	void SetCipherSecurityLevel(SecurityLevel& cipherSecurityLevel) { m_cipherSecurityLevel = cipherSecurityLevel; };
	// 認証アルゴリズム評価出力
	SecurityLevel GetAuthSecurityLevel() {return m_authSecurityLevel;};
	// 暗号アルゴリズム評価入力
	void SetAuthSecurityLevel(SecurityLevel& authSecurityLevel) { m_authSecurityLevel = authSecurityLevel; };
	// 数値化
	void SetValue();
	std::vector<Datas> GetDatas()const { return m_datas; }
	// Cipher Algorithmを評価してセキュリティレベルを返す関数
	SecurityLevel EvaluateSecurityLevel(DWORD cipherAlgorithm);
	// セキュリティレベルを文字列に変換する関数
	std::string ConvertSecurityLevelToJapanese(SecurityLevel securityLevel);
	std::string ConvertSecurityLevelToJapanese(DWORD cipherAlgorithm);
	int ConvertSecurityLevelToValue(SecurityLevel securityLevel);
	// 認証アルゴリズムを評価してセキュリティレベルを返す関数
	SecurityLevel EvaluateAuthAlgorithmSecurity(DOT11_AUTH_ALGORITHM authAlgorithm);
	// セキュリティレベルを文字列に変換する関数
	std::string ConvertAuthSecurityLevelToJapanese(SecurityLevel securityLevel);
	int ConvertAuthSecurityLevelToValue(SecurityLevel securityLevel);
};
