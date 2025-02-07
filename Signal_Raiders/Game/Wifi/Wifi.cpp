/**
 * @file   Wifi.h
 *
 * @brief  Wi-Fi�̓d�g�̋����𓾂�\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "pch.h"
#include <memory>
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Wifi/Parameters/WiFiParameters.h"
#include "Game/CommonResources.h"

// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
Wifi::Wifi()
	: m_dwCurVersion{}
	, m_hClient{}
	, m_dwMaxClient{}
	, m_dwResult{}
	, m_pInterfaceList{}
	, m_pNetworkList{}
	, m_displayedSSIDs{}
	, m_count{}
	, m_networkInfos{}
	, m_network{}
	, m_ssid{}
	, m_converter{}
	, m_output{ nullptr }
	, m_memory{ nullptr }
	, m_time(0.0f)
{
}
/**
 * @brief �f�X�g���N�^
 */
Wifi::~Wifi()
{
}
// ����������
void Wifi::Initialize()
{

	m_output = std::make_unique<Output>();
	m_memory = std::make_unique<ReleaseMemory>();
}
// �X�V����
void Wifi::Update(float elapsedTime)
{
	// �ܕb�o������X�V�I��
	if (m_time >= WiFiParameters::MAX_TIME)
	{
		return;
	}
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
	// �\������
	SetUp();
	// �X�L�������ʂ̏���
	ProcessingScanResults();
	// �d�g�̋����Ń\�[�g
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// �\�[�g��̏���\��
	m_output->DisplayInformation(m_networkInfos, m_count);
	m_output->SetInformation(m_networkInfos);
	// ���Ԃ��v��
	m_time += elapsedTime;
	// Wi-Fi���擾�ł��Ȃ���Ԃ̎�
	if (m_dwResult != ERROR_SUCCESS)
	{
		if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
		{
			for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
			{
				// �ܕb�o������X�V�I��
				if (m_time >= WiFiParameters::MAX_TIME)
				{

					break;
				}
				m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// �d�g�̋�����100�ɐݒ�

				m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// �G�̎�ނ�0(�ʏ�)�ɐݒ�
			}
		}
		m_wifilevels = m_preWifilevels;		// �d�g�̋������ϒ��z��ɓo�^
		m_enemyTypes = m_preEnemyTypes;		// �G�̎�ނ��ϒ��z��ɓo�^
	}
	else// Wi-Fi���擾�ł����Ԃ̎�
	{
		// �擾��������0�̎�
		if (m_networkInfos.size() == 0)
		{
			if (m_preWifilevels.size() < WiFiParameters::MAX_ENEMY)
			{
				for (int index = 0; index < WiFiParameters::MAX_ENEMY; index++)
				{
					// �ܕb�o������X�V�I��
					if (m_time >= WiFiParameters::MAX_TIME)
					{
						break;
					}
					m_preWifilevels.push_back(WiFiParameters::DEFAULT_ENEMY_HP);// �d�g�̋�����100�ɐݒ�
					m_preEnemyTypes.push_back(WiFiParameters::DEFAULT_ENEMY_TYPE);// �G�̎�ނ�0(�ʏ�)�ɐݒ�
				}
			}
			m_wifilevels = m_preWifilevels;		// �d�g�̋������ϒ��z��ɓo�^
			m_enemyTypes = m_preEnemyTypes;		// �G�̎�ނ��ϒ��z��ɓo�^
		}
		// �擾��������1�ȏ�̎�
		// ���l�����o��
		for (const auto& networkInfo : m_networkInfos)
		{
			// �ܕb�o������X�V�I��
			if (m_time >= WiFiParameters::MAX_TIME)
			{
				m_time = WiFiParameters::MAX_TIME;
				break;
			}
			//ssid�̕��������ϒ��z��ɓo�^
			int ssidLength = (int)(networkInfo.ssid.length());
			//ssid�̕�����ASCII�R�[�h�̍��v���ϒ��z��ɓo�^
			int ssidValue = m_output->ConvertSsidToInt(networkInfo.ssid);
			int enemyType = (ssidValue % ssidLength) % ENEMY_TYPE_MAX;
			//�d�g�̋������ϒ��z��ɓo�^
			m_preWifilevels.push_back(networkInfo.signalQuality);

			m_preEnemyTypes.push_back(enemyType);// �G�̎�ނ�ݒ�	
			m_wifilevels = m_preWifilevels;		// �d�g�̋������ϒ��z��ɓo�^
			m_enemyTypes = m_preEnemyTypes;		// �G�̎�ނ��ϒ��z��ɓo�^

		}
	}

}

// �N���A����
void Wifi::Clear()
{
	if (m_dwResult != ERROR_SUCCESS || m_time >= WiFiParameters::MAX_TIME)
	{

		return;
	}
	// �������̉���ƃn���h���̃N���[�Y
	m_memory->FreeMemoryAndCloseHandle(m_pInterfaceList, m_hClient, m_networkInfos, m_displayedSSIDs);
	m_preWifilevels.clear();
	m_preEnemyTypes.clear();
}

// �n���h���̏�����
void Wifi::InitHandle()
{
	m_dwMaxClient = 2; // �ő�̃N���C�A���g��
	m_hClient = NULL;
	m_dwCurVersion = 0;
	m_dwResult = 0;
}

// �o�[�W�����̊m�F
void Wifi::Checkversion()
{
	m_dwResult = WlanOpenHandle(m_dwMaxClient, NULL, &m_dwCurVersion, &m_hClient);
	if (m_dwResult != ERROR_SUCCESS) return;
}

// �C���^�[�t�F�[�X�̃��X�g�擾
void Wifi::GetInterfacesList()
{
	m_pInterfaceList = NULL;
	m_dwResult = WlanEnumInterfaces(m_hClient, NULL, &m_pInterfaceList);

	if (m_dwResult != ERROR_SUCCESS)
	{
		WlanCloseHandle(m_hClient, NULL);
		return;
	}
}

// �X�L�����̊J�n
void Wifi::StartScan()
{
	m_dwResult = WlanScan(m_hClient, &m_pInterfaceList->InterfaceInfo[0].InterfaceGuid, NULL, NULL, NULL);
	if (m_dwResult != ERROR_SUCCESS)
	{
		WlanFreeMemory(m_pInterfaceList);
		WlanCloseHandle(m_hClient, NULL);
		return;
	}
}

// �X�L�������ʂ̎擾
void Wifi::GetScanResults()
{
	m_pNetworkList = NULL;
	m_dwResult = WlanGetAvailableNetworkList(
		m_hClient,
		&m_pInterfaceList->InterfaceInfo[0].InterfaceGuid,
		0,
		NULL,
		&m_pNetworkList
	);
	if (m_dwResult != ERROR_SUCCESS)
	{

		return;
	}
}

// �\������
void Wifi::SetUp()
{
	m_displayedSSIDs.clear();
	m_networkInfos.clear();
}

// �X�L�������ʂ̏���
void Wifi::ProcessingScanResults()
{
	for (DWORD i = 0; i < m_pNetworkList->dwNumberOfItems; i++)
	{
		m_network = m_pNetworkList->Network[i];
		m_ssid = (m_network.dot11Ssid.uSSIDLength == 0) ? "����J�̃l�b�g���[�N" : m_converter.to_bytes(std::wstring(m_network.dot11Ssid.ucSSID, m_network.dot11Ssid.ucSSID + m_network.dot11Ssid.uSSIDLength));
		if (m_displayedSSIDs.find(m_ssid) != m_displayedSSIDs.end())
		{
			continue;
		}
		m_displayedSSIDs.insert(m_ssid);
		NetworkInfo networkInfo;
		networkInfo.ssid = m_ssid;
		networkInfo.signalQuality = m_network.wlanSignalQuality;
		m_networkInfos.push_back(networkInfo);
	}
}

