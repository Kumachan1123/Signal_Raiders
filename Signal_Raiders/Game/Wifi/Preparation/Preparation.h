/**
 * @file   Preparation.h
 *
 * @brief  �\�������Ɋւ���w�b�_�t�@�C��
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
class Preparation
{
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	Preparation();
	// �f�X�g���N�^
	~Preparation();
	// �\������
	void Set(std::set<std::string>& displayedSSIDs,int& count, std::vector<NetworkInfo>& networkInfos);
};
