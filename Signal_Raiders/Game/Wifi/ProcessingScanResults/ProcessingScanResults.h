/**
 * @file   ProcessingScanResults.h
 *
 * @brief  �X�L�������ʂ̏����Ɋւ���w�b�_�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // ���d�C���N���[�h�̖h�~ =====================================================
#pragma once
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/Wifi.h"
// �N���X�̒�` ===============================================================

// �l�b�g���[�N�����i�[����\����
class Wifi;
class ProcessingScanResults
{
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	ProcessingScanResults();
	// �f�X�g���N�^
	~ProcessingScanResults();
	// �X�L�������ʂ̏���
	void Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,
		WLAN_AVAILABLE_NETWORK& network,
		std::string& ssid,
		std::set<std::string>& displayedSSIDs,
		std::vector<NetworkInfo>& networkInfos);
	void GetResults(Wifi* pWifi);
private:
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_converter;


};
