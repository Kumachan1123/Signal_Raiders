/**
 * @file   ReleaseMemory.h
 *
 * @brief  ���\���Ɋւ���w�b�_�t�@�C��
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
class ReleaseMemory
{
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	ReleaseMemory();
	// �f�X�g���N�^
	~ReleaseMemory();
	// �������̉���ƃn���h���̃N���[�Y
	void FreeMemoryAndCloseHandle(PWLAN_INTERFACE_INFO_LIST& pInterfaceList,HANDLE& hClient, std::vector<NetworkInfo>& networkInfos, std::set<std::string>& displayedSSIDs);
};
