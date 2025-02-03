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
#include "Game/Wifi/Wifi.h"
class Wifi;
class InitHandle;
class CheckVersion;
class GetListOfInterfaces;
class StartScan;
class GetScanResults;
class Preparation;
class ProcessingScanResults;
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
	Wifi* m_pWifi;
public:
	// �R���X�g���N�^
	UpdateInfo(Wifi* pWifi);
	// �f�X�g���N�^
	~UpdateInfo();
	void Initialize();

	Wifi* GetWifi() { return m_pWifi; }
	void Set();
};
