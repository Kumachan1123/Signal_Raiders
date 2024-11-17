/**
 * @file   Preparation.h
 *
 * @brief  �\�������Ɋւ���\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "Game/Wifi/Preparation/Preparation.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
Preparation::Preparation()
{
}
/**
 * @brief �f�X�g���N�^
 */
Preparation::~Preparation()
{
}
// �\������
void Preparation::Set(std::set<std::string>& displayedSSIDs, int& count, std::vector<NetworkInfo>& networkInfos)
{
	// ���łɕ\������SSID���Ǘ����邽�߂�set
	displayedSSIDs.clear();
	// �\��������
	count = 0;
	// �l�b�g���[�N�����i�[����vector
	networkInfos.clear();
}
