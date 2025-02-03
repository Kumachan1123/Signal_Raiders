/**
 * @file   StartScan.h
 *
 * @brief  �X�L�����̊J�n�Ɋւ���w�b�_�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // ���d�C���N���[�h�̖h�~ =====================================================
#pragma once
#include <windows.h>
#include <wlanapi.h>
#include <locale>
#include <codecvt>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include "Game/Wifi/Wifi.h"
#pragma comment(lib, "wlanapi.lib")
// �N���X�̒�` ===============================================================
class Wifi;
class StartScan
{
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	StartScan();
	// �f�X�g���N�^
	~StartScan();
	// �X�L�����̊J�n
	void Set(DWORD& dwResult, HANDLE& hClient, PWLAN_INTERFACE_INFO_LIST& pInterfaceList);
	void Scan(Wifi* pWifi);
};
