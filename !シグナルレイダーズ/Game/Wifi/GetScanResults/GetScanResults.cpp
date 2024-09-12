/**
 * @file   GetScanResults.h
 *
 * @brief  �X�L�������ʂ̎擾����\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */


 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/GetScanResults/GetScanResults.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
GetScanResults::GetScanResults()
{
}
/**
 * @brief �f�X�g���N�^
 */
GetScanResults::~GetScanResults()
{
}
// �X�L�������ʂ̎擾
void GetScanResults::Set(PWLAN_AVAILABLE_NETWORK_LIST& pNetworkList, DWORD& dwResult, HANDLE& hClient, PWLAN_INTERFACE_INFO_LIST& pInterfaceList)
{
	pNetworkList = NULL;
	dwResult =
		WlanGetAvailableNetworkList(hClient,
									&pInterfaceList->InterfaceInfo[0].InterfaceGuid,
									0,
									NULL,
									&pNetworkList);
	if (dwResult != ERROR_SUCCESS)
	{
		return;
	}
}




