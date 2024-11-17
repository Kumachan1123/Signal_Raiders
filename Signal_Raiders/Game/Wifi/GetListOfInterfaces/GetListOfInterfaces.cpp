/**
 * @file   GetListOfInterfaces.h
 *
 * @brief �C���^�[�t�F�[�X�̃��X�g�擾����\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "pch.h"
#include "Game/Screen.h"
#include "Game/Wifi/GetListOfInterfaces/GetListOfInterfaces.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
GetListOfInterfaces::GetListOfInterfaces()
{
}
/**
 * @brief �f�X�g���N�^
 */
GetListOfInterfaces::~GetListOfInterfaces()
{
}
// �C���^�[�t�F�[�X�̃��X�g�擾
void GetListOfInterfaces::Set(PWLAN_INTERFACE_INFO_LIST& pInterfaceList, DWORD& dwResult, HANDLE& hClient)
{
	pInterfaceList = NULL;
	dwResult = WlanEnumInterfaces(hClient, NULL, &pInterfaceList);
	
	if (dwResult != ERROR_SUCCESS)
	{
		WlanCloseHandle(hClient, NULL);
		return;
	}
}



