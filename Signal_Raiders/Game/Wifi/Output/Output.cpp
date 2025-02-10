/**
 * @file   Output.h
 *
 * @brief  ���\���Ɋւ���\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include "pch.h"
#include <sstream>
#include "Game/Wifi/Output/Output.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
Output::Output()
	: m_networkInfos{}
	, m_count{}
	, m_data{}
	, m_datas{}
{
}
/**
 * @brief �f�X�g���N�^
 */
Output::~Output()
{
}
void Output::DisplayInformation(std::vector<NetworkInfo>& networkInfos)
{

	UNREFERENCED_PARAMETER(networkInfos);
	//���l�����o��
	for (const auto& networkInfo : m_networkInfos)
	{
		int idLength = (int)(networkInfo.ssid.length());//SSID�̒���(������)
		int waveLevel = networkInfo.signalQuality;//�d�g�̋���

		Datas datas{};
		datas.SSIDLength = idLength;
		datas.SSIDValue = ConvertSsidToInt(networkInfo.ssid);
		datas.WVlevel = waveLevel;

		m_datas.push_back(datas);


	}

	m_datas.clear();
}




int Output::ConvertSsidToInt(const std::string& ssid)
{
	int sum = 0;
	for (char c : ssid)
	{
		sum += static_cast<int>(c); // �������Ƃ�ASCII�R�[�h�����v
	}
	return abs(sum);
}




