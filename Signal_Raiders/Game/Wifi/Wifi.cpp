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
	// �ܕb�o������X�V�I��
	if (m_time >= WiFiParameters::MAX_TIME)return;
	// Wi-Fi�n���h���̏�����
	InitHandle();
	// �o�[�W�����̊m�F
	Checkversion();
	// �C���^�[�t�F�[�X�̃��X�g�擾
	GetInterfacesList();
	// �X�L�����̊J�n
	StartScan();
	// �X�L�������ʂ̎擾
	GetScanResults();
	// ���ݐڑ����Ă���Wi-Fi�����擾
	GetCurrentWifiInfo();
	// �o�͏���
	ClearExportInfo();
	// �X�L�������ʂ̏���
	ProcessingScanResults();
	// �d�g�̋����Ń\�[�g
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// ���Ԃ����Z
	m_time += elapsedTime;
	// Wi-Fi���擾�ł������ǂ����ɉ����ĕ���
	if (m_dwResult != ERROR_SUCCESS)// Wi-Fi���擾�ł��Ȃ���Ԃ̎�
	{
		// �ݒ肳�ꂽ�G�̍ő�l�����̏ꍇ
		if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
		{
			//�ő�l�ɂȂ�܂ŌJ��Ԃ�
			for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
			{
				// �ܕb�o������X�V�I��
				if (m_time >= WiFiParameters::MAX_TIME)break;
				// �d�g�̋�����100�ɐݒ�
				m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);
				// �G�̎�ނ�0(�ʏ�)�ɐݒ�
				m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);
			}
		}
		// �d�g�̋������ϒ��z��ɓo�^
		m_wifilevels = m_preWifilevels;
		// �G�̎�ނ��ϒ��z��ɓo�^
		m_enemyTypes = m_preEnemyTypes;
	}
	else// Wi-Fi���擾�ł����Ԃ̎�
	{
		// �擾��������0�̎�
		if (m_networkInfos.size() == 0)
		{
			// �ݒ肳�ꂽ�G�̍ő�l�����̏ꍇ
			if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
			{
				//�ő�l�ɂȂ�܂ŌJ��Ԃ�
				for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
				{
					// �ܕb�o������X�V�I��
					if (m_time >= WiFiParameters::MAX_TIME)break;
					// �d�g�̋�����100�ɐݒ�
					m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);
					// �G�̎�ނ�0(�ʏ�)�ɐݒ�
					m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);
				}
			}
			// �d�g�̋������ϒ��z��ɓo�^
			m_wifilevels = m_preWifilevels;
			// �G�̎�ނ��ϒ��z��ɓo�^
			m_enemyTypes = m_preEnemyTypes;
		}
		// �ȉ��A�����Wi-Fi���擾�ł����ꍇ
		// �擾�����������J��Ԃ�
		for (const auto& networkInfo : m_networkInfos)
		{
			// �����Ԍo������X�V�I��
			if (m_time >= WiFiParameters::MAX_TIME)
			{
				// ���Ԃ��ő�l�ɌŒ�
				m_time = WiFiParameters::MAX_TIME;
				// ���[�v�𔲂���
				break;
			}
			// ssid�̕��������ϒ��z��ɓo�^
			int ssidLength = (int)(networkInfo.ssid.length());
			// ssid�̕�����ASCII�R�[�h�̍��v���ϒ��z��ɓo�^
			int ssidValue = ConvertSsidToInt(networkInfo.ssid);
			// (�������~������ASCII�R�[�h�̍��v)/ENEMY_TYPE_MAX�̗]���G�̎�ނɕϊ�
			int enemyType = (ssidValue * ssidLength) % ENEMY_TYPE_MAX;
			// �d�g�̋������ϒ��z��ɓo�^
			m_preWifilevels.push_back(networkInfo.signalQuality);
			// �G�̎�ނ�ݒ�	
			m_preEnemyTypes.push_back(enemyType);
			// �d�g�̋������ϒ��z��ɓo�^
			m_wifilevels = m_preWifilevels;
			// �G�̎�ނ��ϒ��z��ɓo�^
			m_enemyTypes = m_preEnemyTypes;
		}
	}
	// �e��|�C���^�[���N���A
	Clear();
}
/*
*	@brief	�N���A
*	@details Wi-Fi�N���X�̃N���A����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::Clear()
{
	// �ܕb�o������X�V�I��
	if (m_dwResult != ERROR_SUCCESS || m_time >= WiFiParameters::MAX_TIME)return;
	// �C���^�[�t�F�[�X���X�g�̃����������
	WlanFreeMemory(m_pInterfaceList);
	// �n���h�����N���[�Y
	WlanCloseHandle(m_hClient, NULL);
	// Wi-Fi�����i�[����ϒ��z����N���A
	m_networkInfos.clear();
	// �\���ς�SSID�̃Z�b�g���N���A
	m_exportSSIDs.clear();
	// �d�g�̋������N���A
	m_preWifilevels.clear();
	// �G�̎�ނ��N���A
	m_preEnemyTypes.clear();
}
/*
*	@brief	�n���h���̏�����
*	@details Wi-Fi�N���X�̃n���h���̏�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::InitHandle()
{
	// �ő�̃N���C�A���g��
	m_dwMaxClient = 2;
	// �N���C�A���g�n���h��
	m_hClient = NULL;
	// WLAN�̃o�[�W����
	m_dwCurVersion = 0;
	// ����
	m_dwResult = 0;
}
/*
*	@brief	�o�[�W�����̊m�F
*	@details WlanAPI�̃N���C�A���g�o�[�W�������m�F
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::Checkversion()
{
	// WlanAPI�̃N���C�A���g�o�[�W�������m�F
	m_dwResult = WlanOpenHandle(
		m_dwMaxClient,     // �v������N���C�A���g�̃o�[�W�����i���́j
		NULL,              // �\��iNULL��OK�j
		&m_dwCurVersion,   // ���ۂɎg����o�[�W�����������ɕԂ�i�o�́j
		&m_hClient);       // �擾���ꂽ�N���C�A���g�n���h���i�o�́j
	// �o�[�W�����̊m�F���ʂ������łȂ��ꍇ�͏����𒆎~
	if (m_dwResult != ERROR_SUCCESS) return;
}
/*
*	@brief �C���^�[�t�F�[�X�̃��X�g�擾
*	@details �C���^�[�t�F�[�X�̃��X�g���擾
*/
void Wifi::GetInterfacesList()
{
	// ���X�g�|�C���^�[��������
	m_pInterfaceList = NULL;
	// �C���^�[�t�F�[�X�̃��X�g�擾
	m_dwResult = WlanEnumInterfaces(m_hClient, NULL, &m_pInterfaceList);
	// �C���^�[�t�F�[�X�̃��X�g�擾�Ɏ��s�����ꍇ�͏����𒆎~
	if (m_dwResult != ERROR_SUCCESS)
	{
		// �n���h���̃N���[�Y
		WlanCloseHandle(m_hClient, NULL);
		// �����̒��~
		return;
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
	// �w�肵���C���^�[�t�F�[�X��Wi-Fi�X�L�������J�n
	m_dwResult = WlanScan(
		m_hClient, // Wi-Fi�N���C�A���g�̃n���h��
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid, // �g�p����A�_�v�^��GUID
		NULL,// �X�L�����̃I�v�V�����i���Ɏw��Ȃ��j
		NULL, // �\������i�s�g�p�j
		NULL);// �\������i�s�g�p�j
	// �X�L�����̊J�n���ʂ������łȂ��ꍇ�͏����𒆎~
	if (m_dwResult != ERROR_SUCCESS)
	{
		// �������̉��
		WlanFreeMemory(m_pInterfaceList);
		// �n���h���̃N���[�Y
		WlanCloseHandle(m_hClient, NULL);
		// �����̒��~
		return;
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
	// �l�b�g���[�N���X�g�̃|�C���^�[��������
	m_pNetworkList = NULL;
	// �g�p�\�ȃl�b�g���[�N�̈ꗗ���擾
	m_dwResult = WlanGetAvailableNetworkList(
		m_hClient,// Wi-Fi�N���C�A���g�̃n���h��
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// �g���C���^�[�t�F�[�X��GUID
		0,// �I�v�V�����̃t���O�i���Ɏw��Ȃ��j
		NULL,// �g�p���錾��i��{��NULL�j
		&m_pNetworkList);// �l�b�g���[�N���X�g
	// �X�L�������ʂ̎擾�Ɏ��s�����ꍇ�͏����𒆎~
	if (m_dwResult != ERROR_SUCCESS)return;
}

/*
*	@brief	�O���ɓn��Wi-Fi����������
*	@details �Q�[���̃��W�b�N�⑼�̃V�X�e���ɓn�����߂�SSID��d�g�����N���A����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::ClearExportInfo()
{
	// �\������SSID���N���A
	m_exportSSIDs.clear();
	// �l�b�g���[�N�����N���A
	m_networkInfos.clear();
}
/*
*	@brief	���ݐڑ����Ă���Wi-Fi�����擾
*	@details ���ݐڑ����Ă���Wi-Fi��SSID��d�g�̋������擾����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::GetCurrentWifiInfo()
{
	// ���łɎ擾�ς݂Ȃ�I��
	if (m_isGotCurrentWifiInfo)return;
	// �ڑ����̃|�C���^�[
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	// �ڑ����̃T�C�Y
	DWORD connectInfoSize = 0;
	// �f�[�^�̎��
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;
	// ���ݐڑ����Ă���Wi-Fi�����擾
	m_dwResult = WlanQueryInterface(
		m_hClient,// Wi-Fi�N���C�A���g�̃n���h��
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,// �g�p����C���^�[�t�F�[�X��GUID
		wlan_intf_opcode_current_connection,// ���݂̐ڑ������擾
		NULL,// �\������i�s�g�p�j
		&connectInfoSize,// �ڑ����̃T�C�Y�i�o�́j
		(PVOID*)&pConnectInfo,// �ڑ����̃|�C���^�[�i�o�́j
		&opCode); // �f�[�^�̎�ށi�s�g�p�j
	// �ڑ���Ԃ�n���h���̏�Ԃɉ����ĕ���
	if (m_dwResult == ERROR_SUCCESS && pConnectInfo != NULL)// �ڑ����̎擾���ʂ����������ꍇ
	{
		// ssid��std::wstring�ɕϊ�
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		// SSID��std::wstring�ɕϊ�
		std::wstring wssid = std::wstring(
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID,		// SSID�̃|�C���^�[
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID +		// SSID�̃|�C���^�[�̒���
			pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength); // SSID�̒���
		// ssid��std::string�ɕϊ�
		std::string ssid = m_converter.to_bytes(wssid);
		// SSID�̒������擾���Đ��l��
		m_currentWifiInfo.ssidLenght = (int)(ssid.length());
		// �d�g�̋������擾
		m_currentWifiInfo.signalQuality = pConnectInfo->wlanAssociationAttributes.wlanSignalQuality;
	}
	else// ���̎擾�Ɏ��s�����ꍇ
	{
		// SSID�̒�����0�ɐݒ�
		m_currentWifiInfo.ssidLenght = 0;
		// �d�g�̋�����0�ɐݒ�
		m_currentWifiInfo.signalQuality = 0;
		// �����̒��~
		return;
	}
	// �擾�ς݃t���O��true�ɐݒ�
	m_isGotCurrentWifiInfo = true;
	// �������̉��
	WlanFreeMemory(pConnectInfo);
}
/*
*	@brief	�X�L�������ʂ̏���
*	@details �X�L�������ʂ��������āASSID��d�g�̋������擾����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wifi::ProcessingScanResults()
{
	// �p�����[�^���s���Ȏ���n���h�����s���Ȏ��͏����𒆎~
	if (m_dwResult == ERROR_INVALID_PARAMETER || m_dwResult == ERROR_INVALID_HANDLE)return;
	// �擾�������ʂ̐�����
	for (DWORD i = 0; i < m_pNetworkList->dwNumberOfItems; i++)
	{
		// �X�L�������ʂ��擾
		m_network = m_pNetworkList->Network[i];
		// SSID�̒�����0�̎�
		m_ssid = (m_network.dot11Ssid.uSSIDLength == 0)
			? "����J�̃l�b�g���[�N"// SSID�����J�̃l�b�g���[�N�ɐݒ�
			: m_converter.to_bytes(// SSID�𕶎���ɕϊ�
				std::wstring(// SSID��std::wstring�ɕϊ�
					m_network.dot11Ssid.ucSSID, // SSID�̐擪�|�C���^�[
					m_network.dot11Ssid.ucSSID + m_network.dot11Ssid.uSSIDLength));// �I�_���v�Z���Ĕ͈͎w��
		// ���łɕ\������SSID�̎��̓X�L�b�v
		if (m_exportSSIDs.find(m_ssid) != m_exportSSIDs.end())continue;
		// �\������SSID���Z�b�g
		m_exportSSIDs.insert(m_ssid);
		// �l�b�g���[�N���̍\���̂�������
		NetworkInfo networkInfo;
		// SSID���Z�b�g
		networkInfo.ssid = m_ssid;
		// �d�g�̋������Z�b�g
		networkInfo.signalQuality = m_network.wlanSignalQuality;
		// �l�b�g���[�N�����x�N�^�[�ɒǉ�
		m_networkInfos.push_back(networkInfo);
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
	// ���v�l
	int sum = 0;
	// �������Ƃ�ASCII�R�[�h�����v
	for (char c : ssid)sum += static_cast<int>(c);
	// ���v�l���Βl�ɕϊ����ĕԂ�
	return abs(sum);
}