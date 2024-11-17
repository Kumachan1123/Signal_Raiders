/**
 * @file   ReleaseMemory.h
 *
 * @brief  ���\���Ɋւ���\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
ReleaseMemory::ReleaseMemory()
{
}
/**
 * @brief �f�X�g���N�^
 */
ReleaseMemory::~ReleaseMemory()
{
}
// �������̉���ƃn���h���̃N���[�Y
void ReleaseMemory::FreeMemoryAndCloseHandle(PWLAN_INTERFACE_INFO_LIST& pInterfaceList, HANDLE& hClient, std::vector<NetworkInfo>& networkInfos, std::set<std::string>& displayedSSIDs)
{
	WlanFreeMemory(pInterfaceList);
	WlanCloseHandle(hClient, NULL);
	networkInfos.clear();
	displayedSSIDs.clear();
}