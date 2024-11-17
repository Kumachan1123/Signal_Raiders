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
// �N���X�̒�` ===============================================================
/**
  * @brief �Q�[��
  */
  // �l�b�g���[�N�����i�[����\����
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
			 std::wstring_convert<std::codecvt_utf8<wchar_t>>& converter,
			 std::vector<NetworkInfo>& networkInfos);


};
