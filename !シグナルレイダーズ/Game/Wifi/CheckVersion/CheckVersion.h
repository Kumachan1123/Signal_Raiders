/**
 * @file   CheckVersion.h
 *
 * @brief  Wi-Fi�̃o�[�W�������m�F����w�b�_�t�@�C��
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
class CheckVersion 
{
// �N���X�萔�̐錾
public:
	

// �f�[�^�����o�̐錾
private:
	
	
// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	CheckVersion();

	// �f�X�g���N�^
	~CheckVersion();


	
	void Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient);
	
};
