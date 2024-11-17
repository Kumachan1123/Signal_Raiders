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
		CCMP = 4,
		WEP104 = 5,
		Unknown = 0xFF
	};
}

struct Datas
{
	int SSIDLength;		//SSID�̕�����
	int SSIDValue;		//SSID�̊e������ASCII�R�[�h�̍��v
	int WVlevel;	//�d�g�̋���

};