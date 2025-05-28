/*
*	@file	Wifi.cpp
*	@brief	Wi-Fi�̏����擾����N���X
*/
#include <pch.h>
#include "Wifi.h"
/*
*	@brief	�R���X�g���N�^
*	@details Wi-Fi�N���X�̃R���X�g���N�^
*	@param	�Ȃ�
*	@return �Ȃ�
*/
Wifi::Wifi()
	: m_dwCurVersion{}// �o�[�W����
	, m_hClient{}// �N���C�A���g�n���h��
	, m_dwMaxClient{}// �ő�N���C�A���g��
	, m_dwResult{}// ����
	, m_pInterfaceList{}// �C���^�[�t�F�[�X�̃��X�g
	, m_pNetworkList{}// �X�L�������ʂ̃��X�g
	, m_exportSSIDs{}// ���łɓn����SSID
	, m_count{}// �\��������
	, m_networkInfos{}// �l�b�g���[�N���
	, m_currentWifiInfo{}// ���ݐڑ����Ă���Wi-Fi���
	, m_network{}// �X�L��������
	, m_ssid{}// SSID
	, m_converter{}// wstring��string�ɕϊ����邽�߂̕ϐ�
	, m_isGotCurrentWifiInfo(false)// ���ݐڑ����Ă���Wi-Fi�����擾�ς݂�
	, m_time(0.0f)// Wi-Fi�擾����
{
}
/*
*	@brief �f�X�g���N�^
*	@details Wi-Fi�N���X�̃f�X�g���N�^(�������Ȃ��j
*	@param �Ȃ�
*	@return �Ȃ�
*/
Wifi::~Wifi() {/*do nothing*/ }

/*
*	@brief �X�V����
*	@details Wi-Fi�N���X�̍X�V����
*	@param elapsedTime �X�V�ɂ�����������
*	@return �Ȃ�
*/
void Wifi::Update(float elapsedTime)
{
	if (m_time >= WiFiParameters::MAX_TIME)return;// �ܕb�o������X�V�I��
	InitHandle();// Wi-Fi�n���h���̏�����
	Checkversion();// �o�[�W�����̊m�F
	GetInterfacesList();// �C���^�[�t�F�[�X�̃��X�g�擾
	StartScan();// �X�L�����̊J�n
	GetScanResults();// �X�L�������ʂ̎擾
	GetCurrentWifiInfo();// ���ݐڑ����Ă���Wi-Fi�����擾
	ClearExportInfo();// �\������
	ProcessingScanResults();// �X�L�������ʂ̏���
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());// �d�g�̋����Ń\�[�g
	m_time += elapsedTime;// ���Ԃ����Z
	if (m_dwResult != ERROR_SUCCESS)// Wi-Fi���擾�ł��Ȃ���Ԃ̎�
	{
		if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)// �ݒ肳�ꂽ�G�̍ő�l�����̏ꍇ
		{
			for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)//�ő�l�ɂȂ�܂ŌJ��Ԃ�
			{
				if (m_time >= WiFiParameters::MAX_TIME)break;// �ܕb�o������X�V�I��
				m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// �d�g�̋�����100�ɐݒ�
				m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// �G�̎�ނ�0(�ʏ�)�ɐݒ�
			}
		}
		m_wifilevels = m_preWifilevels;// �d�g�̋������ϒ��z��ɓo�^
		m_enemyTypes = m_preEnemyTypes;// �G�̎�ނ��ϒ��z��ɓo�^
	}
	else// Wi-Fi���擾�ł����Ԃ̎�
	{
		if (m_networkInfos.size() == 0)// �擾��������0�̎�
		{
			if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)// �ݒ肳�ꂽ�G�̍ő�l�����̏ꍇ
			{
				for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)//�ő�l�ɂȂ�܂ŌJ��Ԃ�
				{
					if (m_time >= WiFiParameters::MAX_TIME)break;// �ܕb�o������X�V�I��
					m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// �d�g�̋�����100�ɐݒ�
					m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// �G�̎�ނ�0(�ʏ�)�ɐݒ�
				}
			}
			m_wifilevels = m_preWifilevels;// �d�g�̋������ϒ��z��ɓo�^
			m_enemyTypes = m_preEnemyTypes;// �G�̎�ނ��ϒ��z��ɓo�^
		}
		// �ȉ��A�����Wi-Fi���擾�ł����ꍇ
		for (const auto& networkInfo : m_networkInfos)// �擾�����������J��Ԃ�
		{
			if (m_time >= WiFiParameters::MAX_TIME)// �ܕb�o������X�V�I��
			{
				m_time = WiFiParameters::MAX_TIME;// ���Ԃ��ő�l�ɌŒ�
				break;// ���[�v�𔲂���
			}
			int ssidLength = (int)(networkInfo.ssid.length());// ssid�̕��������ϒ��z��ɓo�^
			int ssidValue = ConvertSsidToInt(networkInfo.ssid);// ssid�̕�����ASCII�R�[�h�̍��v���ϒ��z��ɓo�^
			int enemyType = (ssidValue * ssidLength) % ENEMY_TYPE_MAX;// (�������~������ASCII�R�[�h�̍��v)/ENEMY_TYPE_MAX�̗]���G�̎�ނɕϊ�
			m_preWifilevels.push_back(networkInfo.signalQuality);// �d�g�̋������ϒ��z��ɓo�^
			m_preEnemyTypes.push_back(enemyType);// �G�̎�ނ�ݒ�	
			m_wifilevels = m_preWifilevels;// �d�g�̋������ϒ��z��ɓo�^
			m_enemyTypes = m_preEnemyTypes;// �G�̎�ނ��ϒ��z��ɓo�^
		}
	}
}
/*
*	@brief	�N���A
*	@details Wi-Fi�N���X�̃N���A����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::Clear()
{
	if (m_dwResult != ERROR_SUCCESS || m_time >= WiFiParameters::MAX_TIME)return;// �ܕb�o������X�V�I��
	WlanFreeMemory(m_pInterfaceList);// �C���^�[�t�F�[�X���X�g�̃����������
	WlanCloseHandle(m_hClient, NULL);// �n���h�����N���[�Y
	m_networkInfos.clear();// Wi-Fi�����i�[����ϒ��z����N���A
	m_exportSSIDs.clear();// �\���ς�SSID�̃Z�b�g���N���A
	m_preWifilevels.clear();// �d�g�̋������N���A
	m_preEnemyTypes.clear();// �G�̎�ނ��N���A
}
/*
*	@brief	�n���h���̏�����
*	@details Wi-Fi�N���X�̃n���h���̏�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::InitHandle()
{
	m_dwMaxClient = 2; // �ő�̃N���C�A���g��
	m_hClient = NULL;// �N���C�A���g�n���h��
	m_dwCurVersion = 0;// WLAN�̃o�[�W����
	m_dwResult = 0;// ����
}
/*
*	@brief	�o�[�W�����̊m�F
*	@details WlanAPI�̃N���C�A���g�o�[�W�������m�F
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::Checkversion()
{
	m_dwResult = WlanOpenHandle(// WlanAPI�̃N���C�A���g�o�[�W�������m�F
		m_dwMaxClient,     // �v������N���C�A���g�̃o�[�W�����i���́j
		NULL,              // �\��iNULL��OK�j
		&m_dwCurVersion,   // ���ۂɎg����o�[�W�����������ɕԂ�i�o�́j
		&m_hClient);       // �擾���ꂽ�N���C�A���g�n���h���i�o�́j
	if (m_dwResult != ERROR_SUCCESS) return;// �G���[����
}
/*
*	@brief �C���^�[�t�F�[�X�̃��X�g�擾
*	@details �C���^�[�t�F�[�X�̃��X�g���擾
*/
void Wifi::GetInterfacesList()
{
	m_pInterfaceList = NULL;// ���X�g�|�C���^�[��������
	m_dwResult = WlanEnumInterfaces(m_hClient, NULL, &m_pInterfaceList);// �C���^�[�t�F�[�X�̃��X�g�擾
	if (m_dwResult != ERROR_SUCCESS)// �G���[����
	{
		WlanCloseHandle(m_hClient, NULL);// �n���h���̃N���[�Y
		return;// �����̒��~
	}
}

/*
*	@brief �X�L�����̊J�n
*	@details ���͂�Wi-Fi�A�N�Z�X�|�C���g���X�L��������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::StartScan()
{
	m_dwResult = WlanScan(// �w�肵���C���^�[�t�F�[�X��Wi-Fi�X�L�������J�n
		m_hClient, // Wi-Fi�N���C�A���g�̃n���h��
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid, // �g�p����A�_�v�^��GUID
		NULL,// �X�L�����̃I�v�V�����i���Ɏw��Ȃ��j
		NULL, // �\������i�s�g�p�j
		NULL);// �\������i�s�g�p�j
	if (m_dwResult != ERROR_SUCCESS)// �G���[����
	{
		WlanFreeMemory(m_pInterfaceList);// �������̉��
		WlanCloseHandle(m_hClient, NULL);// �n���h���̃N���[�Y
		return;// �����̒��~
	}
}
/*
*	@brief �X�L�������ʂ̎擾
*	@details �X�L�����������ʂ���A���̗͂��p�\��Wi-Fi�l�b�g���[�N�����擾����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::GetScanResults()
{
	m_pNetworkList = NULL; // �l�b�g���[�N���X�g�̃|�C���^�[��������
	m_dwResult = WlanGetAvailableNetworkList(// �g�p�\�ȃl�b�g���[�N�̈ꗗ���擾
		m_hClient,// Wi-Fi�N���C�A���g�̃n���h��
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// �g���C���^�[�t�F�[�X��GUID
		0,// �I�v�V�����̃t���O�i���Ɏw��Ȃ��j
		NULL,// �g�p���錾��i��{��NULL�j
		&m_pNetworkList);// �l�b�g���[�N���X�g
	if (m_dwResult != ERROR_SUCCESS)return;// �G���[����
}

/*
*	@brief	�O���ɓn��Wi-Fi����������
*	@details �Q�[���̃��W�b�N�⑼�̃V�X�e���ɓn�����߂�SSID��d�g�����N���A����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::ClearExportInfo()
{
	m_exportSSIDs.clear();// �\������SSID���N���A
	m_networkInfos.clear();// �l�b�g���[�N�����N���A
}
/*
*	@brief	���ݐڑ����Ă���Wi-Fi�����擾
*	@details ���ݐڑ����Ă���Wi-Fi��SSID��d�g�̋������擾����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::GetCurrentWifiInfo()
{
	if (m_isGotCurrentWifiInfo)return;// ���łɎ擾�ς݂Ȃ�I��
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;// �ڑ����̃|�C���^�[
	DWORD connectInfoSize = 0;// �ڑ����̃T�C�Y
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;// �f�[�^�̎��
	m_dwResult = WlanQueryInterface(// ���ݐڑ����Ă���Wi-Fi�����擾
		m_hClient,// Wi-Fi�N���C�A���g�̃n���h��
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// �g�p����C���^�[�t�F�[�X��GUID
		wlan_intf_opcode_current_connection,// ���݂̐ڑ������擾
		NULL,// �\������i�s�g�p�j
		&connectInfoSize,// �ڑ����̃T�C�Y�i�o�́j
		(PVOID*)&pConnectInfo,// �ڑ����̃|�C���^�[�i�o�́j
		&opCode); // �f�[�^�̎�ށi�s�g�p�j
	if (m_dwResult == ERROR_SUCCESS && pConnectInfo != NULL)// ���̎擾�ɐ��������ꍇ
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;// ssid��std::wstring�ɕϊ�
		std::wstring wssid = std::wstring(// SSID��std::wstring�ɕϊ�
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID,		// SSID�̃|�C���^�[
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID +		// SSID�̃|�C���^�[�̒���
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength); // SSID�̒���
		std::string ssid = m_converter.to_bytes(wssid);// ssid��std::string�ɕϊ�
		m_currentWifiInfo.ssidLenght = (int)(ssid.length());// SSID�̒������擾���Đ��l��
		m_currentWifiInfo.signalQuality = pConnectInfo->wlanAssociationAttributes.wlanSignalQuality;// �d�g�̋������擾
	}
	else// ���̎擾�Ɏ��s�����ꍇ
	{
		m_currentWifiInfo.ssidLenght = 0;// SSID�̒�����0�ɐݒ�
		m_currentWifiInfo.signalQuality = 0; // �d�g�̋�����0�ɐݒ�
		return;// �����̒��~
	}
	m_isGotCurrentWifiInfo = true;// �擾�ς݃t���O��true�ɐݒ�
	WlanFreeMemory(pConnectInfo); // �������̉��
}
/*
*	@brief	�X�L�������ʂ̏���
*	@details �X�L�������ʂ��������āASSID��d�g�̋������擾����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::ProcessingScanResults()
{
	if (m_dwResult == ERROR_INVALID_PARAMETER ||	// �p�����[�^���s���Ȏ���
		m_dwResult == ERROR_INVALID_HANDLE)return;	// �n���h�����s���Ȏ��͏����𒆎~
	for (DWORD i = 0; i < m_pNetworkList->dwNumberOfItems; i++)// �擾�������ʂ̐�����
	{
		m_network = m_pNetworkList->Network[i];// �X�L�������ʂ��擾
		m_ssid = (m_network.dot11Ssid.uSSIDLength == 0) // SSID�̒�����0�̎�
			? "����J�̃l�b�g���[�N"// SSID�����J�̃l�b�g���[�N�ɐݒ�
			: m_converter.to_bytes(// SSID�𕶎���ɕϊ�
				std::wstring(// SSID��std::wstring�ɕϊ�
					m_network.dot11Ssid.ucSSID, // SSID�̐擪�|�C���^�[
					m_network.dot11Ssid.ucSSID + m_network.dot11Ssid.uSSIDLength));// �I�_���v�Z���Ĕ͈͎w��
		if (m_exportSSIDs.find(m_ssid) != m_exportSSIDs.end())continue;// ���łɕ\������SSID�̎��̓X�L�b�v
		m_exportSSIDs.insert(m_ssid); // �\������SSID���Z�b�g
		NetworkInfo networkInfo;// �l�b�g���[�N���̍\���̂�������
		networkInfo.ssid = m_ssid; // SSID���Z�b�g
		networkInfo.signalQuality = m_network.wlanSignalQuality; // �d�g�̋������Z�b�g
		m_networkInfos.push_back(networkInfo); // �l�b�g���[�N�����x�N�^�[�ɒǉ�
	}
}
/*
*	@brief SSID�𖳗���萔�l�ɕϊ�����֐�
*	@details SSID�𖳗���萔�l�ɕϊ�����֐��i�e������ASCII�R�[�h�̍��v�j
*	@param ssid SSID
*	@return �ϊ��������l
*/
int Wifi::ConvertSsidToInt(const std::string& ssid)
{
	int sum = 0;// ���v�l
	for (char c : ssid)sum += static_cast<int>(c); // �������Ƃ�ASCII�R�[�h�����v
	return abs(sum);// ���v�l���Βl�ɕϊ����ĕԂ�
}