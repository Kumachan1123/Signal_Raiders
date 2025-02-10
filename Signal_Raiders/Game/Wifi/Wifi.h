/**
 * @file   Wifi.h
 *
 * @brief  Wi-Fi�̓d�g�̋����E�F�؂ƈÍ��̃Z�L�����e�B���x���𓾂�w�b�_�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */

 // ���d�C���N���[�h�̖h�~ =====================================================
#pragma once
#include "Game/CommonResources.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"

#include "Game/Wifi/Wifi.h"
#include "Libraries/MyLib/DebugString.h"



// �N���X�̒�` ===============================================================
/**
  * @brief �Q�[��
  */
class CommonResources;
class Output;
class ReleaseMemory;

class Wifi
{
public:
	struct CurrentWifiInfo// ���ݐڑ����Ă���Wi-Fi���̍\����
	{
		int ssidValue;// ���l������SSID
		int signalQuality;// �d�g�̋���
	};
	// �f�[�^�����o�̐錾
private:

	// wi-fi�֘A�̕ϐ�
	std::unique_ptr<Output>					 m_output;
	std::unique_ptr<ReleaseMemory>			 m_memory;
	// Wi-Fi�֘A�̐錾
	DWORD  m_dwMaxClient; // �ő�̃N���C�A���g��
	HANDLE m_hClient;
	DWORD  m_dwCurVersion;
	DWORD  m_dwResult;
	// �C���^�[�t�F�[�X�̃��X�g�擾
	PWLAN_INTERFACE_INFO_LIST m_pInterfaceList;
	// �X�L�������ʂ̎擾
	PWLAN_AVAILABLE_NETWORK_LIST m_pNetworkList;
	// ���łɕ\������SSID���Ǘ����邽�߂�set
	std::set<std::string> m_displayedSSIDs;
	// �\��������
	int m_count;
	// �l�b�g���[�N�����i�[����vector
	std::vector<NetworkInfo> m_networkInfos;
	// �X�L�������ʂ̏����Ɏg���ϐ�
	WLAN_AVAILABLE_NETWORK m_network;
	std::string m_ssid;
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
	float m_time;				  // Wi-Fi�擾���ԁi�T�b�߂�����T�b�ŌŒ�j
	const int ENEMY_TYPE_MAX = 2;// �U�R�G�̎�ނ̍ő�l
public:
	// �R���X�g���N�^
	Wifi();
	// �f�X�g���N�^
	~Wifi();
	// ����
public:
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �N���A
	void Clear();
private:
	void InitHandle();// �n���h���̏�����
	void Checkversion();// �o�[�W�����̊m�F
	void GetInterfacesList();// �C���^�[�t�F�[�X�̃��X�g�擾
	void StartScan();// �X�L�����̊J�n
	void GetScanResults();// �X�L�������ʂ̎擾
	void SetUp();// �\������
	void GetCurrentWifiInfo();// ���ݐڑ����Ă���Wi-Fi�����擾
	void ProcessingScanResults();// �X�L�������ʂ̏���
public:
	// �擾�������𑗂�
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }//�d�g�̋�����n��
	std::vector<int> GetEnemyTypes()const { return m_enemyTypes; }//�G�̎�ނ�n��
	// �X�V���I��������ǂ����𒲂ׂ邽�߂̃Q�b�^�[
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }
	std::vector<int> GetPreEnemyTypes()const { return m_preEnemyTypes; }
	// ���ڑ����Ă���Wi-Fi�����擾����
	int GetCurrentWifiSSIDValue()const { return m_currentWifiInfo.ssidValue; }
	int GetCurrentWifiSignalQuality()const { return m_currentWifiInfo.signalQuality; }
	DWORD GetResult()const { return m_dwResult; }
	DWORD* GetResult() { return &m_dwResult; }
	void SetResult(DWORD result) { m_dwResult = result; }
	DWORD GetMaxClient()const { return m_dwMaxClient; }
	void SetMaxClient(DWORD maxClient) { m_dwMaxClient = maxClient; }
	DWORD* GetCurVersion() { return &m_dwCurVersion; }
	void SetCurVersion(DWORD curVersion) { m_dwCurVersion = curVersion; }
	HANDLE* GetClient() { return &m_hClient; }
	void SetClient(HANDLE client) { m_hClient = client; }
	const PWLAN_INTERFACE_INFO_LIST& GetInterfaceList() { return m_pInterfaceList; }
	PWLAN_INTERFACE_INFO_LIST* GetPInterfaceList() { return &m_pInterfaceList; }
	void SetInterfaceList(PWLAN_INTERFACE_INFO_LIST interfaceList) { m_pInterfaceList = interfaceList; }
	PWLAN_AVAILABLE_NETWORK_LIST GetNetworkList()const { return m_pNetworkList; }
	void SetNetworkList(PWLAN_AVAILABLE_NETWORK_LIST networkList) { m_pNetworkList = networkList; }
	PWLAN_AVAILABLE_NETWORK_LIST* GetPNetworkList() { return &m_pNetworkList; }
	std::vector<NetworkInfo> GetNetworkInfos()const { return m_networkInfos; }
	void SetNetworkInfos(std::vector<NetworkInfo> networkInfos) { m_networkInfos = networkInfos; }
	WLAN_AVAILABLE_NETWORK GetNetwork()const { return m_network; }
	void SetNetwork(WLAN_AVAILABLE_NETWORK network) { m_network = network; }
	std::string GetSSID()const { return m_ssid; }
	void SetSSID(std::string ssid) { m_ssid = ssid; }
	std::set<std::string> GetDisplayedSSIDs()const { return m_displayedSSIDs; }
	void SetDisplayedSSIDs(std::set<std::string> displayedSSIDs) { m_displayedSSIDs = displayedSSIDs; }
	int GetCount()const { return m_count; }
	void SetCount(int count) { m_count = count; }
};
