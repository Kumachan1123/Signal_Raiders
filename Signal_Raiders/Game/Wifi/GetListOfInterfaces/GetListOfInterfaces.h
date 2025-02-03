/**
 * @file   GetListOfInterfaces.h
 *
 * @brief  �C���^�[�t�F�[�X�̃��X�g�擾����w�b�_�t�@�C��
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
class Wifi;
class GetListOfInterfaces
{

	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	GetListOfInterfaces();

	// �f�X�g���N�^
	~GetListOfInterfaces();
	void Set(PWLAN_INTERFACE_INFO_LIST& pInterfaceList, DWORD& dwResult, HANDLE& hClient);
	// �C���^�[�t�F�[�X�̃��X�g�擾
	void GetList(Wifi* pWifi);
};
