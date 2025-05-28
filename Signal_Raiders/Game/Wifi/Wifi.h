/*
*	@file	Wifi.h
*	@brief	Wi-Fi�̏����擾����N���X
*/
#pragma once
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
class Wifi
{
public:// �\����
	struct CurrentWifiInfo// ���ݐڑ����Ă���Wi-Fi���̍\����
	{
		int ssidLenght;// SSID�̒���
		int signalQuality;// �d�g�̋���
	};
public:// �A�N�Z�T
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }//�d�g�̋�����n��
	std::vector<int> GetEnemyTypes()const { return m_enemyTypes; }//�G�̎�ނ�n��
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }// �d�g�̋�����n��
	std::vector<int> GetPreEnemyTypes()const { return m_preEnemyTypes; }// �G�̎�ނ�n��
	int GetCurrentWifiSSIDLength()const { return m_currentWifiInfo.ssidLenght; }// �ڑ����Ă���Wi-Fi��SSID�̒�����n��
	int GetCurrentWifiSignalQuality()const { return m_currentWifiInfo.signalQuality; }// �ڑ����Ă���Wi-Fi�̓d�g�̋�����n��
	DWORD GetResult()const { return m_dwResult; }// ���ʂ�n��
	DWORD* GetResult() { return &m_dwResult; }// ���ʂ�n��(�|�C���^�[)
	void SetResult(DWORD result) { m_dwResult = result; }// ���ʂ��Z�b�g����
	DWORD GetMaxClient()const { return m_dwMaxClient; }// �ő�N���C�A���g����n��
	void SetMaxClient(DWORD maxClient) { m_dwMaxClient = maxClient; }// �ő�N���C�A���g�����Z�b�g����
	DWORD* GetCurVersion() { return &m_dwCurVersion; }// �o�[�W������n��(�|�C���^�[)
	void SetCurVersion(DWORD curVersion) { m_dwCurVersion = curVersion; }// �o�[�W�������Z�b�g����
	HANDLE* GetClient() { return &m_hClient; }// �N���C�A���g�n���h����n��(�|�C���^�[)
	void SetClient(HANDLE client) { m_hClient = client; }// �N���C�A���g�n���h�����Z�b�g����
	const PWLAN_INTERFACE_INFO_LIST& GetInterfaceList() { return m_pInterfaceList; }// �C���^�[�t�F�[�X�̃��X�g��n��
	PWLAN_INTERFACE_INFO_LIST* GetPInterfaceList() { return &m_pInterfaceList; }// �C���^�[�t�F�[�X�̃��X�g��n��(�|�C���^�[)
	void SetInterfaceList(PWLAN_INTERFACE_INFO_LIST interfaceList) { m_pInterfaceList = interfaceList; }// �C���^�[�t�F�[�X�̃��X�g���Z�b�g����
	PWLAN_AVAILABLE_NETWORK_LIST GetNetworkList()const { return m_pNetworkList; }// �X�L�������ʂ̃��X�g��n��
	void SetNetworkList(PWLAN_AVAILABLE_NETWORK_LIST networkList) { m_pNetworkList = networkList; }// �X�L�������ʂ̃��X�g���Z�b�g����
	PWLAN_AVAILABLE_NETWORK_LIST* GetPNetworkList() { return &m_pNetworkList; }// �X�L�������ʂ̃��X�g��n��(�|�C���^�[)
	std::vector<NetworkInfo> GetNetworkInfos()const { return m_networkInfos; }// �l�b�g���[�N����n��
	void SetNetworkInfos(std::vector<NetworkInfo> networkInfos) { m_networkInfos = networkInfos; }// �l�b�g���[�N�����Z�b�g����
	WLAN_AVAILABLE_NETWORK GetNetwork()const { return m_network; }// �X�L�������ʂ�n��
	void SetNetwork(WLAN_AVAILABLE_NETWORK network) { m_network = network; }// �X�L�������ʂ��Z�b�g����
	std::string GetSSID()const { return m_ssid; }// SSID��n��
	void SetSSID(std::string ssid) { m_ssid = ssid; }// SSID���Z�b�g����
	std::set<std::string> GetDisplayedSSIDs()const { return m_exportSSIDs; }// ���łɕ\������SSID��n��
	void SetDisplayedSSIDs(std::set<std::string> displayedSSIDs) { m_exportSSIDs = displayedSSIDs; }// ���łɕ\������SSID���Z�b�g����
	int GetCount()const { return m_count; }// �\�������񐔂�n��
	void SetCount(int count) { m_count = count; }// �\�������񐔂��Z�b�g����
public:// public�֐�
	Wifi();// �R���X�g���N�^
	~Wifi();// �f�X�g���N�^
	void Update(float elapsedTime);// �X�V����
	void Clear();// �N���A
private:// private�֐�
	void InitHandle();// �n���h���̏�����
	void Checkversion();// �o�[�W�����̊m�F
	void GetInterfacesList();// �C���^�[�t�F�[�X�̃��X�g�擾
	void StartScan();// �X�L�����̊J�n
	void GetScanResults();// �X�L�������ʂ̎擾
	void ClearExportInfo();// �\������
	void GetCurrentWifiInfo();// ���ݐڑ����Ă���Wi-Fi�����擾
	void ProcessingScanResults();// �X�L�������ʂ̏���
	int ConvertSsidToInt(const std::string& ssid);// SSID�𖳗���萔�l�ɕϊ�����֐��i�e������ASCII�R�[�h�̍��v�j
private:// private�ϐ�

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
	// �ŏ��̌ܕb�͂��̔z���Wi-Fi�̋������i�[����
	std::vector<int> m_preWifilevels;// Wi-Fi�̋���
	std::vector<int> m_preEnemyTypes;// �G�̎��(ssid�̕�����ASCII�R�[�h�̍��v��ssid�̕������Ŋ��������̂̂��܂�)
	// �ܕb��Ɉȉ��̔z���Wi-Fi�̏����i�[����i��x���ꂽ����͍X�V���Ȃ��j
	std::vector<int> m_wifilevels;// Wi-Fi�̋���
	std::vector<int> m_enemyTypes;// �G�̎��(ssid�̕�����ASCII�R�[�h�̍��v��ssid�̕������Ŋ��������̂̂��܂�)
	// ���ڑ����Ă���Wi-Fi���
	CurrentWifiInfo m_currentWifiInfo;
	// ���ڑ����Ă���Wi-Fi�����擾�ς݂�
	bool m_isGotCurrentWifiInfo;
	// Wi-Fi�擾���ԁi�T�b�߂�����T�b�ŌŒ�j
	float m_time;
	// �U�R�G�̎�ނ̍ő�l
	const int ENEMY_TYPE_MAX = 2;
};
