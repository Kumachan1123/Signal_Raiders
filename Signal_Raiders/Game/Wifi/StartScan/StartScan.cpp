/**
 * @file   StartScan.h
 *
 * @brief  �X�L�����̊J�n�Ɋւ���\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/StartScan/StartScan.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
StartScan::StartScan()
{
}
/**
 * @brief �f�X�g���N�^
 */
StartScan::~StartScan()
{
}
// �X�L�����̊J�n
void StartScan::Set(DWORD& dwResult, HANDLE& hClient, PWLAN_INTERFACE_INFO_LIST& pInterfaceList)
{
	dwResult = WlanScan(hClient, &pInterfaceList->InterfaceInfo[0].InterfaceGuid, NULL, NULL, NULL);
	if (dwResult != ERROR_SUCCESS)
	{
		WlanFreeMemory(pInterfaceList);
		WlanCloseHandle(hClient, NULL);
		return;
	}
}

void StartScan::Scan(Wifi* pWifi)
{
	PWLAN_INTERFACE_INFO_LIST pInterfaceList = pWifi->GetInterfaceList();
	const GUID* pInterfaceGuid = &pInterfaceList->InterfaceInfo[0].InterfaceGuid;
	// WlanScan �Ăяo��
	pWifi->SetResult(WlanScan(
		pWifi->GetClient(),
		pInterfaceGuid,
		NULL, NULL, NULL));	if (pWifi->GetResult() != ERROR_SUCCESS)
	{
		WlanFreeMemory(pWifi->GetInterfaceList());
		WlanCloseHandle(pWifi->GetClient(), NULL);
		return;
	}
}
