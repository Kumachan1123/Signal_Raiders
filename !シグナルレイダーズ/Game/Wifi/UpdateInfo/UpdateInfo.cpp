/**
 * @file   UpdateInfo.h
 *
 * @brief  Wi-Fi�̏����X�V����\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
UpdateInfo::UpdateInfo()
	: m_InterfacesList{ nullptr }
	, m_Checkversion{ nullptr }
	, m_StartScan{ nullptr }
	, m_GetResults{ nullptr }
	, m_handle{ nullptr }
	, m_Preparation{ nullptr }
	, m_ProcessingScanResults{ nullptr }
{
}
/**
 * @brief �f�X�g���N�^
 */
UpdateInfo::~UpdateInfo()
{
}
void UpdateInfo::Initialize()
{
	m_InterfacesList = std::make_unique<GetListOfInterfaces>();
	m_Checkversion = std::make_unique<CheckVersion>();
	m_StartScan = std::make_unique<StartScan>();
	m_GetResults = std::make_unique<GetScanResults>();
	m_Preparation = std::make_unique<Preparation>();
	m_ProcessingScanResults = std::make_unique< ProcessingScanResults>();
}
void UpdateInfo::Set(DWORD& dwResult,
					 DWORD& dwMaxClient,
					 DWORD& dwCurVersion,
					 HANDLE& hClient,
					 PWLAN_INTERFACE_INFO_LIST& pInterfaceList,
					 PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,
					 std::vector<NetworkInfo>& networkInfos,
					 WLAN_AVAILABLE_NETWORK& network,
					 std::string& ssid,
					 std::set<std::string>& displayedSSIDs,
					 std::wstring_convert<std::codecvt_utf8<wchar_t>>& converter,
					 int& count,
					 std::vector<int>& preWifilevels)
{
	// Wi-Fi�n���h���̏�����
	m_handle->Set(dwResult, dwMaxClient, dwCurVersion, hClient);
	// �o�[�W�����̊m�F
	m_Checkversion->Set(dwResult, dwMaxClient, dwCurVersion, hClient);
	// �C���^�[�t�F�[�X�̃��X�g�擾
	m_InterfacesList->Set(pInterfaceList, dwResult, hClient);
	// �X�L�����̊J�n
	m_StartScan->Set(dwResult, hClient, pInterfaceList);
	// �X�L�������ʂ̎擾
	m_GetResults->Set(pNetworkList, dwResult, hClient, pInterfaceList, networkInfos, preWifilevels);
	// �\������
	m_Preparation->Set(displayedSSIDs, count, networkInfos);
	// �X�L�������ʂ̏���
	m_ProcessingScanResults->Set(pNetworkList, network, ssid, displayedSSIDs, converter, networkInfos);
}
