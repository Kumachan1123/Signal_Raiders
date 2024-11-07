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
	: dwCurVersion{}
	, hClient{}
	, dwMaxClient{}
	, dwResult{}
	, pInterfaceList{}
	, pNetworkList{}
	, displayedSSIDs{}
	, count{}
	, m_networkInfos{}
	, network{}
	, ssid{}
	, cipherSecurityLevel{}
	, authSecurityLevel{}
	, converter{}
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
	m_updateInfo->Set(dwResult, dwMaxClient, dwCurVersion, hClient, pInterfaceList, pNetworkList, m_networkInfos, network, ssid, displayedSSIDs, converter, count);
	std::sort(m_networkInfos.begin(), m_networkInfos.end(), CompareBySignalQuality());
	// �\�[�g��̏���\��
	m_output->DisplayInformation(m_networkInfos, count, cipherSecurityLevel, authSecurityLevel);
	m_output->SetInformation(m_networkInfos);
	// ���Ԃ��v��
	m_time += elapsedTime;
	// Wi-Fi���擾�ł��Ȃ���Ԃ̎�
	if (dwResult != ERROR_SUCCESS)
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
				m_preWifilevels.push_back(100);

			}
		}

		m_wifilevels = m_preWifilevels;
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
					m_preWifilevels.push_back(100);
				}
			}
			m_wifilevels = m_preWifilevels;
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
			m_wifilevels = m_preWifilevels;
		}
	}

}

void Wifi::Clear()
{
	if (dwResult != ERROR_SUCCESS)
	{
		return;
	}
	// �������̉���ƃn���h���̃N���[�Y
	m_memory->FreeMemoryAndCloseHandle(pInterfaceList, hClient, m_networkInfos, displayedSSIDs);
	m_preWifilevels.clear();
}

