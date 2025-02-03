/**
 * @file   CheckVersion.h
 *
 * @brief  Wi-Fi�̓d�g�̋����E�F�؂ƈÍ��̃Z�L�����e�B���x���𓾂�\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "pch.h"
#include "Game/Screen.h"
#include <sstream>
#include "Game/Wifi/CheckVersion/CheckVersion.h"

// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
CheckVersion::CheckVersion()
{
}
/**
 * @brief �f�X�g���N�^
 */
CheckVersion::~CheckVersion()
{
}
// �o�[�W�����̊m�F
void CheckVersion::Set(DWORD& dwResult, DWORD& dwMaxClient, DWORD& dwCurVersion, HANDLE& hClient)
{
	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) return;
}

void CheckVersion::Check(Wifi* pWifi)
{
	auto result = WlanOpenHandle(pWifi->GetMaxClient(), NULL, pWifi->GetCurVersion(), pWifi->GetClient());
	pWifi->SetResult(result);
}
