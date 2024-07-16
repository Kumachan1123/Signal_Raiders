/**
 * @file   UpdateInfo.h
 *
 * @brief  Wi-Fi�̏����X�V����\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // ���d�C���N���[�h�̖h�~ =====================================================
#pragma once
#include "Game/Wifi/CheckVersion/CheckVersion.h"
#include "Game/Wifi/GetListOfInterfaces/GetListOfInterfaces.h"
#include "Game/Wifi/GetScanResults/GetScanResults.h"
#include "Game/Wifi/StartScan/StartScan.h"
#include "Game/Wifi/InitHandle/InitHandle.h"
#include "Game/Wifi/Preparation/Preparation.h"
#include "Game/Wifi/ProcessingScanResults/ProcessingScanResults.h"
#include "Game/Wifi/Interface/IWifiParts.h"
// �N���X�̒�` ===============================================================
/**
  * @brief �Q�[��
  */
class UpdateInfo
{
	// �f�[�^�����o�̐錾
private:
	// �N���X���낢��
	std::unique_ptr<GetListOfInterfaces>	 m_InterfacesList;
	std::unique_ptr<CheckVersion>			 m_Checkversion;
	std::unique_ptr<StartScan>				 m_StartScan;
	std::unique_ptr<GetScanResults>			 m_GetResults;
	std::unique_ptr <InitHandle>			 m_handle;
	std::unique_ptr<Preparation>			 m_Preparation;
	std::unique_ptr< ProcessingScanResults>	 m_ProcessingScanResults;
	// �����o�֐��̐錾

public:
	// �R���X�g���N�^
	UpdateInfo();
	// �f�X�g���N�^
	~UpdateInfo();
	void Initialize();
	void Set(DWORD& dwResult,// Wi-Fi�֘A�̐錾
			 DWORD& dwMaxClient,// Wi-Fi�֘A�̐錾
			 DWORD& dwCurVersion,// Wi-Fi�֘A�̐錾
			 HANDLE& hClient,// Wi-Fi�֘A�̐錾
			 PWLAN_INTERFACE_INFO_LIST& pInterfaceList,// �C���^�[�t�F�[�X�̃��X�g�擾
			 PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,// �X�L�������ʂ̎擾
			 std::vector<NetworkInfo>& networkInfos,// �l�b�g���[�N�����i�[����vector
			 WLAN_AVAILABLE_NETWORK& network,// �X�L�������ʂ̏����Ɏg���ϐ�
			 std::string& ssid,// �X�L�������ʂ̏����Ɏg���ϐ�
			 std::set<std::string>& displayedSSIDs,// ���łɕ\������SSID���Ǘ����邽�߂�set
			 std::wstring_convert<std::codecvt_utf8<wchar_t>>& converter,// �X�L�������ʂ̏����Ɏg���ϐ�
			 int& count,// �\��������
			 std::vector<int>& preWifilevels
	);
};
