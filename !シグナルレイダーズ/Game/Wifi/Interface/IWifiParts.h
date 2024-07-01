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
// Wi-Fi�̏��̍\����
struct NetworkInfo
{
	std::string ssid;// SSID�i���O�j
	DWORD signalQuality;// �d�g�̋���
	DOT11_CIPHER_ALGORITHM defaultCipherAlgorithm;// �Í��A���S���Y��
	DOT11_AUTH_ALGORITHM defaultAuthAlgorithm;// �F�؃A���S���Y��
};
// �d�g�̋����Ŕ�r���邽�߂̊֐��I�u�W�F�N�g
struct CompareBySignalQuality
{
	bool operator()(const NetworkInfo& a, const NetworkInfo& b) const
	{
		return a.signalQuality > b.signalQuality;
	}
};
// �Z�L�����e�B�̕]��
enum SecurityLevel
{
	VerySecure,
	Secure,
	Moderate,
	Unsafe,
	VeryUnsafe
};
// Cipher Algorithm�̗񋓌^
namespace CipherAlgorithms
{
    enum CipherAlgorithm
    {
        WEP40 = 1,
        TKIP = 2,
        CCMP_AES128 = 4,  // CCMP with AES-128
        CCMP_AES256 = 8,  // CCMP with AES-256
        WEP104 = 16,
        WPA_PSK = 32,     // WPA-PSK
        WPA2_PSK = 64,    // WPA2-PSK
        WPA3_PSK = 128,   // WPA3-PSK
        WPA3_SAE = 256,   // WPA3-SAE
        GCMP_AES128 = 512,  // GCMP with AES-128
        GCMP_AES256 = 1024, // GCMP with AES-256
        BIP_CMAC_256 = 2048,  // BIP-CMAC-256
        BIP_GMAC_128 = 4096,  // BIP-GMAC-128
        BIP_GMAC_256 = 8192,  // BIP-GMAC-256
        Unknown = 0xFF
    };

}

struct Datas
{
	int WVlevel;//�d�g�̋���
	int ASlevel;//�F�؃A���S���Y���̃Z�L�����e�B���x��
	int CIlevel;//�Í��A���S���Y���̃Z�L�����e�B���x��
};