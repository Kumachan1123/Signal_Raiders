/**
 * @file   InitHandle.h
 *
 * @brief  �n���h���̏������Ɋւ���w�b�_�t�@�C��
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
class InitHandle
{
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	InitHandle();
	// �f�X�g���N�^
	~InitHandle();
	void Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient);
	void Initialize(Wifi* pWifi);
};
