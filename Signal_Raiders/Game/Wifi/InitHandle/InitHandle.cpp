/**
 * @file   InitHandle.h
 *
 * @brief  �n���h���̏������Ɋւ���\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/InitHandle/InitHandle.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
InitHandle::InitHandle()
{
}
/**
 * @brief �f�X�g���N�^
 */
InitHandle::~InitHandle()
{
}
// Wi-Fi�n���h���̏�����
void InitHandle::Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient)
{
	dwMaxClient = 2; // �ő�̃N���C�A���g��
	hClient = NULL;
	dwCurVersion = 0;
	dwResult = 0;
}

void InitHandle::Initialize(Wifi* pWifi)
{
	pWifi->SetMaxClient(2);
	pWifi->SetClient(NULL);
	pWifi->SetCurVersion(0);
	pWifi->SetResult(0);
}
