/**
 * @file   Wifi.h
 *
 * @brief  Wi-Fi�̓d�g�̋����E�F�؂ƈÍ��̃Z�L�����e�B���x���𓾂�\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "pch.h"
#include <memory>
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Wifi.h"
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
	, m_updateInfo{ nullptr }
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
	m_updateInfo = std::make_unique<UpdateInfo>();
	m_output = std::make_unique<Output>();
	m_memory = std::make_unique<ReleaseMemory>();
}
// �X�V����
void Wifi::Update(float elapsedTime)
{

	// �S�f�[�^���X�V
	m_updateInfo->Set(m_dwResult, m_dwMaxClient, m_dwCurVersion, m_hClient, m_pInterfaceList, m_pNetworkList, m_networkInfos, m_network, m_ssid, m_displayedSSIDs, m_converter, m_count);
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// �\�[�g��̏���\��
	m_output->DisplayInformation(m_networkInfos, m_count);
	m_output->SetInformation(m_networkInfos);
	// ���Ԃ��v��
	m_time += elapsedTime;
	// Wi-Fi���擾�ł��Ȃ���Ԃ̎�
	if (m_dwResult != ERROR_SUCCESS)
	{
		if (m_preWifilevels.size() < 30)
		{
			for (int index = 0; index < 30; index++)
			{
				// �ܕb�o������X�V�I��
				if (m_time >= 5.0f)
				{

					break;
				}
				m_preWifilevels.push_back(100);// �d�g�̋�����100�ɐݒ�
				m_preSSIDLengths.push_back(10);// ssid�̕�������10�ɐݒ�
				m_preSSIDValues.push_back(1000);// ssid�̕�����ASCII�R�[�h�̍��v��1000�ɐݒ�

			}
		}
		m_wifilevels = m_preWifilevels;
		m_ssidLengths = m_preSSIDLengths;
		m_ssidValues = m_preSSIDValues;
	}
	else// Wi-Fi���擾�ł����Ԃ̎�
	{
		// �擾��������0�̎�
		if (m_networkInfos.size() == 0)
		{
			if (m_preWifilevels.size() < 30)
			{
				for (int index = 0; index < 30; index++)
				{
					// �ܕb�o������X�V�I��
					if (m_time >= 5.0f)
					{
						break;
					}
					m_preWifilevels.push_back(100);// �d�g�̋�����100�ɐݒ�
					m_preSSIDLengths.push_back(10);// ssid�̕�������10�ɐݒ�
					m_preSSIDValues.push_back(1000);// ssid�̕�����ASCII�R�[�h�̍��v��1000�ɐݒ�

				}
			}
			m_wifilevels = m_preWifilevels;
			m_ssidLengths = m_preSSIDLengths;
			m_ssidValues = m_preSSIDValues;
		}
		// �擾��������1�ȏ�̎�
		//���l�����o��
		for (const auto& networkInfo : m_networkInfos)
		{
			// �ܕb�o������X�V�I��
			if (m_time >= 5.0f)
			{
				m_time = 5.0f;
				break;
			}
			//�d�g�̋������ϒ��z��ɓo�^
			m_preWifilevels.push_back(networkInfo.signalQuality);
			//ssid�̕��������ϒ��z��ɓo�^
			m_preSSIDLengths.push_back((int)(networkInfo.ssid.length()));
			//ssid�̕�����ASCII�R�[�h�̍��v���ϒ��z��ɓo�^
			m_preSSIDValues.push_back(m_output->ConvertSsidToInt(networkInfo.ssid));

			m_wifilevels = m_preWifilevels;
			m_ssidLengths = m_preSSIDLengths;
			m_ssidValues = m_preSSIDValues;
		}
	}

}

void Wifi::Clear()
{
	if (m_dwResult != ERROR_SUCCESS)
	{
		return;
	}
	// �������̉���ƃn���h���̃N���[�Y
	m_memory->FreeMemoryAndCloseHandle(m_pInterfaceList, m_hClient, m_networkInfos, m_displayedSSIDs);
	m_preWifilevels.clear();
}

