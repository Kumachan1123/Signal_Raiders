/*
*	@file IWifiParts.h
*	@brief Wi-Fi�̏����擾���邽�߂̃C���^�[�t�F�[�X
*/
#pragma once
// Windows API���g�����߂̃w�b�_�[
#include <windows.h>
#include <wlanapi.h>
// �����R�[�h�ϊ��p�iwstring to string�j
#include <locale>
#include <codecvt>
// STL�i�W���e���v���[�g���C�u�����j
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
// WlanAPI���C�u�����������N
#pragma comment(lib, "wlanapi.lib")
// �\����
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
	// ��r���Z�q���I�[�o�[���[�h
	bool operator()(const NetworkInfo& a, const NetworkInfo& b) const
	{
		// �������Ƀ\�[�g
		return a.signalQuality > b.signalQuality;
	}
};

// SSID��d�g���x�����Ƃɂ����f�[�^�\����
struct Datas
{
	int SSIDLength;		//SSID�̕�����
	int SSIDValue;		//SSID�̊e������ASCII�R�[�h�̍��v
	int WVlevel;	//�d�g�̋���
};

// �񋓌^
// �Z�L�����e�B�̕]��
enum SecurityLevel
{
	VerySecure,// ���Ɉ��S
	Secure,// ���S
	Moderate,// ����
	Unsafe,// �댯
	VeryUnsafe,// ���Ɋ댯
};
// Cipher Algorithm�̗񋓌^
namespace CipherAlgorithms
{
	enum CipherAlgorithm
	{
		WEP40 = 1,// WEP 40�r�b�g
		TKIP = 2,// �ꎞ�I�ȃL�[��������
		CCMP = 4,// AES�x�[�X�̈Í�����
		WEP104 = 5,// WEP 104bit�iWEP�̏�ʔŁj
		Unknown = 0xFF// �s��
	};
}