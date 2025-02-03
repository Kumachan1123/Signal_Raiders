/**
 * @file   ProcessingScanResults.h
 *
 * @brief  �X�L�������ʂ̏����Ɋւ���\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/ProcessingScanResults/ProcessingScanResults.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
ProcessingScanResults::ProcessingScanResults()
	: m_converter{}
{
}
/**
 * @brief �f�X�g���N�^
 */
ProcessingScanResults::~ProcessingScanResults()
{
}
// �X�L�������ʂ̏���
void ProcessingScanResults::Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,
	WLAN_AVAILABLE_NETWORK& network,
	std::string& ssid,
	std::set<std::string>& displayedSSIDs,
	std::vector<NetworkInfo>& networkInfos)
{
	// �X�L�������ʂ̏���
	for (DWORD i = 0; i < pNetworkList->dwNumberOfItems; i++)
	{

		network = pNetworkList->Network[i];


		ssid = (network.dot11Ssid.uSSIDLength == 0) ? "����J�̃l�b�g���[�N" : m_converter.to_bytes(std::wstring(network.dot11Ssid.ucSSID, network.dot11Ssid.ucSSID + network.dot11Ssid.uSSIDLength));

		// ���łɕ\������SSID�̏ꍇ�̓X�L�b�v
		if (displayedSSIDs.find(ssid) == displayedSSIDs.end())
		{
			// �l�b�g���[�N����vector�ɒǉ�
			networkInfos.push_back({ ssid, network.wlanSignalQuality, network.dot11DefaultCipherAlgorithm, network.dot11DefaultAuthAlgorithm });

			// ���łɕ\������SSID��set�ɒǉ�
			displayedSSIDs.insert(ssid);

		}
	}
}

void ProcessingScanResults::GetResults(Wifi* pWifi)
{
	// �X�L�������ʂ̏���
	PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = pWifi->GetNetworkList();
	WLAN_AVAILABLE_NETWORK network = pWifi->GetNetwork();
	for (DWORD i = 0; i < pNetworkList->dwNumberOfItems; i++)
	{

		network = pNetworkList->Network[i];


		pWifi->SetSSID((network.dot11Ssid.uSSIDLength == 0) ? "����J�̃l�b�g���[�N" : m_converter.to_bytes(std::wstring(network.dot11Ssid.ucSSID, network.dot11Ssid.ucSSID + network.dot11Ssid.uSSIDLength)));

		// ���łɕ\������SSID�̏ꍇ�̓X�L�b�v
		if (pWifi->GetDisplayedSSIDs().find(pWifi->GetSSID()) == pWifi->GetDisplayedSSIDs().end())
		{
			// �l�b�g���[�N����vector�ɒǉ�
			pWifi->GetNetworkInfos().push_back({ pWifi->GetSSID(), network.wlanSignalQuality, network.dot11DefaultCipherAlgorithm, network.dot11DefaultAuthAlgorithm });

			// ���łɕ\������SSID��set�ɒǉ�
			pWifi->GetDisplayedSSIDs().insert(pWifi->GetSSID());

		}
	}
}