/**
 * @file   GetScanResults.h
 *
 * @brief  �X�L�������ʂ̎擾����w�b�_�t�@�C��
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
class GetScanResults
{
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	GetScanResults();
	// �f�X�g���N�^
	~GetScanResults();
	void Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList,
			 DWORD& dwResult,
			 HANDLE& hClient,
			 PWLAN_INTERFACE_INFO_LIST& pInterfaceList);
};
