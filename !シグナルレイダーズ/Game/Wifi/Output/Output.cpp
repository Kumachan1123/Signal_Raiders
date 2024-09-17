/**
 * @file   Output.h
 *
 * @brief  ���\���Ɋւ���\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/Output/Output.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
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
 * @brief �f�X�g���N�^
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
	//���l�����o��
	for (const auto& networkInfo : m_networkInfos)
	{
		int WaveLevel = networkInfo.signalQuality;//�d�g�̋���
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

// �Í��A���S���Y����]�����ăZ�L�����e�B���x����Ԃ��֐�
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
		case CipherAlgorithms::WPA_PSK:   // �ǉ�: WPA-PSK
		case CipherAlgorithms::WPA2_PSK:  // �ǉ�: WPA2-PSK
		case CipherAlgorithms::WPA3_PSK:  // �ǉ�: WPA3-PSK
		case CipherAlgorithms::WPA3_SAE:  // �ǉ�: WPA3-SAE
			return VerySecure; // ���̕]���ł�
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
// �Z�L�����e�B���x���𕶎���ɕϊ�����֐�
std::string Output::ConvertSecurityLevelToJapanese(SecurityLevel securityLevel)
{
	switch (securityLevel)
	{
		case VerySecure:
			return "���Ɉ��S";
		case Secure:
			return "���S";
		case Moderate:
			return "����Ȃ�";
		case Unsafe:
			return "�댯";
		case VeryUnsafe:
			return "���Ɋ댯";
		default:
			return "�s��";
	}
}
// �Z�L�����e�B���x���𕶎���ɕϊ�����֐�
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

// �F�؃A���S���Y����]�����ăZ�L�����e�B���x����Ԃ��֐�
SecurityLevel Output::EvaluateAuthAlgorithmSecurity(DOT11_AUTH_ALGORITHM authAlgorithm)
{
	switch (authAlgorithm)
	{
		case DOT11_AUTH_ALGO_80211_OPEN: // �I�[�v���V�X�e���F��
			return VeryUnsafe;
		case DOT11_AUTH_ALGO_80211_SHARED_KEY: // ���L�L�[�F��
			return Moderate; // ���̕]���ł�
		case DOT11_AUTH_ALGO_WPA: // WPA�F��
			return Moderate; // ���̕]���ł�
		case DOT11_AUTH_ALGO_WPA_PSK: // WPA-PSK�F��
			return Moderate; // ���̕]���ł�
		case DOT11_AUTH_ALGO_WPA_NONE: // WPA�ł̔F�؂Ȃ�
			return VeryUnsafe; // ���̕]���ł�
		case DOT11_AUTH_ALGO_RSNA: // RSNA�F��
			return Secure; // ���̕]���ł�
		case DOT11_AUTH_ALGO_RSNA_PSK: // RSNA-PSK�F��
			return Secure; // ���̕]���ł�
		default:
			return VeryUnsafe;
	}
}
// �Z�L�����e�B���x���𕶎���ɕϊ�����֐�
std::string Output::ConvertAuthSecurityLevelToJapanese(SecurityLevel securityLevel)
{
	switch (securityLevel)
	{
		case VerySecure:
			return "���Ɉ��S";
		case Secure:
			return "���S";
		case Moderate:
			return "����Ȃ�";
		case Unsafe:
			return "�댯";
		case VeryUnsafe:
			return "���Ɋ댯";
		default:
			return "�s��";
	}
}

// �Z�L�����e�B���x���𕶎���ɕϊ�����֐�
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

//���l��
void Output::SetValue()
{
	for (const auto& networkInfo : m_networkInfos)
	{
		int WaveLevel = networkInfo.signalQuality;//�d�g�̋���
		int ASLevel = ConvertAuthSecurityLevelToValue(EvaluateSecurityLevel(networkInfo.defaultCipherAlgorithm));
		int CILevel = ConvertSecurityLevelToValue(EvaluateSecurityLevel(networkInfo.defaultCipherAlgorithm));
		Datas datas{};
		datas.WVlevel = WaveLevel;
		datas.ASlevel = ASLevel;
		datas.CIlevel = CILevel;
		m_datas.push_back(datas);
	}
}