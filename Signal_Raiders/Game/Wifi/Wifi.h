/*
*	@file	Wifi.h
*	@brief	Wi-Fi�̏����擾����N���X
*/
#pragma once
#ifndef WIFI_DEFINED
#define WIFI_DEFINED
// �W�����C�u����
#include <memory>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Wifi/Parameters/WiFiParameters.h"

// �O���錾
class CommonResources;
class Output;
class ReleaseMemory;

// Wi-Fi�̏����擾����N���X
class Wifi
{
public:
	// �\����
	// ���ݐڑ����Ă���Wi-Fi���̍\����
	struct CurrentWifiInfo
	{
		int ssidLenght;// SSID�̒���
		int signalQuality;// �d�g�̋���
	};
public:
	// �A�N�Z�T
	//�d�g�̋�����n��
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }
	//�G�̎�ނ�n��
	std::vector<int> GetEnemyTypes()const { return m_enemyTypes; }
	// �ڑ����Ă���Wi-Fi��SSID�̒�����n��
	int GetCurrentWifiSSIDLength()const { return m_currentWifiInfo.ssidLenght; }
	// �ڑ����Ă���Wi-Fi�̓d�g�̋�����n��
	int GetCurrentWifiSignalQuality()const { return m_currentWifiInfo.signalQuality; }
public:
	// public�֐�
	// �R���X�g���N�^
	Wifi();
	// �f�X�g���N�^
	~Wifi();
	// �X�V����
	void Update(float elapsedTime);
	// �N���A
	void Clear();
private:
	// private�֐�
	// �n���h���̏�����
	void InitHandle();
	// �o�[�W�����̊m�F
	void Checkversion();
	// �C���^�[�t�F�[�X�̃��X�g�擾
	void GetInterfacesList();
	// �X�L�����̊J�n
	void StartScan();
	// �X�L�������ʂ̎擾
	void GetScanResults();
	// �\������
	void ClearExportInfo();
	// ���ݐڑ����Ă���Wi-Fi�����擾
	void GetCurrentWifiInfo();
	// �X�L�������ʂ̏���
	void ProcessingScanResults();
	// SSID�𖳗���萔�l�ɕϊ�����֐��i�e������ASCII�R�[�h�̍��v�j
	int ConvertSsidToInt(const std::string& ssid);
private:
	// private�ϐ�
	// �ő�̃N���C�A���g��
	DWORD  m_dwMaxClient;
	// �N���C�A���g�n���h��
	HANDLE m_hClient;
	// WLAN�̃o�[�W����
	DWORD  m_dwCurVersion;
	// WLAN�̌���
	DWORD  m_dwResult;
	// �C���^�[�t�F�[�X�̃��X�g�擾
	PWLAN_INTERFACE_INFO_LIST m_pInterfaceList;
	// �X�L�������ʂ̎擾
	PWLAN_AVAILABLE_NETWORK_LIST m_pNetworkList;
	// ���łɕ\������SSID���Ǘ����邽�߂�set
	std::set<std::string> m_exportSSIDs;
	// �\��������
	int m_count;
	// �l�b�g���[�N�����i�[����vector
	std::vector<NetworkInfo> m_networkInfos;
	// �X�L�������ʂ̏����Ɏg���ϐ�
	WLAN_AVAILABLE_NETWORK m_network;
	// SSID���i�[����ϐ�
	std::string m_ssid;
	// wstring��string�ɕϊ����邽�߂̕ϐ�
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_converter;
	// �v���C�J�n5�b�ԂɎg���z��
	// Wi-Fi�̋������i�[����z��
	std::vector<int> m_preWifilevels;
	// �G�̎��(ssid�̕�����ASCII�R�[�h�̍��v��ssid�̕������Ŋ��������̂̂��܂�)���i�[����z��
	std::vector<int> m_preEnemyTypes;
	// �ܕb��Ɉȉ��̔z���Wi-Fi�̏����i�[����i��x���ꂽ����͍X�V���Ȃ��j
	// Wi-Fi�̋������i�[����z��
	std::vector<int> m_wifilevels;
	// �G�̎��(ssid�̕�����ASCII�R�[�h�̍��v��ssid�̕������Ŋ��������̂̂��܂�)���i�[����z��
	std::vector<int> m_enemyTypes;
	// ���ڑ����Ă���Wi-Fi���
	CurrentWifiInfo m_currentWifiInfo;
	// ���ڑ����Ă���Wi-Fi�����擾�ς݂�
	bool m_isGotCurrentWifiInfo;
	// Wi-Fi�擾���ԁi�T�b�߂�����T�b�ŌŒ�j
	float m_time;
	// �U�R�G�̎�ނ̍ő�l
	const int ENEMY_TYPE_MAX = 3;
};
#endif // WIFI_DEFINED
