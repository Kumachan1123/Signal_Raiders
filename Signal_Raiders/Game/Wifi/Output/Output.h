/**
 * @file   Output.h
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

class Output
{
private:

	// Wi-Fi�����i�[����ϒ��z��
	std::vector<NetworkInfo> m_networkInfos;
	// �\����
	int m_count;

	// ���l�������f�[�^���i�[����ϐ�
	Datas m_data;
	std::vector<Datas> m_datas;
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	Output();
	// �f�X�g���N�^
	~Output();
	// ����\��
	void DisplayInformation(std::vector<NetworkInfo>& networkInfos);
	// �����o��
	std::vector<NetworkInfo> GetInformation() { return m_networkInfos; };
	// �������
	void SetInformation(std::vector<NetworkInfo>& networkInfos) { m_networkInfos = networkInfos; };



	// �f�[�^��n��
	std::vector<Datas> GetDatas()const { return m_datas; }

	// SSID�𖳗���萔�l�ɕϊ�����֐��i�e������ASCII�R�[�h�̍��v�j
	int ConvertSsidToInt(const std::string& ssid);

};
