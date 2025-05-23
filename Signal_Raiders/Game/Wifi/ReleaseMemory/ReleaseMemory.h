/*
*	@file	ReleaseMemory.h
*	@brief	�������̉���ƃn���h���̃N���[�Y�Ɋւ���w�b�_�[�t�@�C��
 */
#pragma once
 // �W�����C�u����
#include <sstream>
// ����w�b�_�[�t�@�C��
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Screen.h"
class ReleaseMemory
{
public:// public�֐�
	ReleaseMemory();// �R���X�g���N�^
	~ReleaseMemory();// �f�X�g���N�^
	void FreeMemoryAndCloseHandle(// �������̉���ƃn���h���̃N���[�Y
		PWLAN_INTERFACE_INFO_LIST& pInterfaceList, HANDLE& hClient,
		std::vector<NetworkInfo>& networkInfos, std::set<std::string>& displayedSSIDs);
};
