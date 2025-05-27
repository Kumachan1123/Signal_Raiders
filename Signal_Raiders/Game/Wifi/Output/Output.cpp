/*
*	@file Output.cpp
*	@brief ���\���Ɋւ���\�[�X�t�@�C��
*/
#include <pch.h>
#include "Output.h"
/*
*	@brief �R���X�g���N�^
*	@details Wi-Fi����\������N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Output::Output()
	: m_networkInfos{}// Wi-Fi�����i�[����ϒ��z��
	, m_count{}// �\����
	, m_data{}// ���l�������f�[�^���i�[����ϐ�
	, m_datas{}// ���l�������f�[�^���i�[����ϒ��z��
{
}
/*
 *	@brief �f�X�g���N�^
 *	@details Wi-Fi����\������N���X�̃f�X�g���N�^
 *	@param �Ȃ�
 *	@return �Ȃ�
 */
Output::~Output() {/*do nothing*/ }
/*
*	@brief ����\������֐�
*	@details Wi-Fi����\������֐�
*	@param networkInfos Wi-Fi�����i�[����ϒ��z��
*	@return �Ȃ�
*/
void Output::DisplayInformation(std::vector<NetworkInfo>& networkInfos)
{
	UNREFERENCED_PARAMETER(networkInfos);// ���g�p�ϐ��̌x���𖳎�����}�N��
	for (const auto& networkInfo : m_networkInfos)// ���l�����o��
	{
		int idLength = (int)(networkInfo.ssid.length());//SSID�̒���(������)
		int waveLevel = networkInfo.signalQuality;//�d�g�̋���
		Datas datas{};// ���l�������f�[�^���i�[����ϐ�
		datas.SSIDLength = idLength;// SSID�̒���(������)
		datas.SSIDValue = ConvertSsidToInt(networkInfo.ssid);// SSID�𐔒l��
		datas.WVlevel = waveLevel;// �d�g�̋���
		m_datas.push_back(datas);// ���l�������f�[�^���i�[����ϒ��z��ɒǉ�
	}
	m_datas.clear();// ���l�������f�[�^���i�[����ϒ��z����N���A
}
/*
*	@brief SSID�𖳗���萔�l�ɕϊ�����֐�
*	@details SSID�𖳗���萔�l�ɕϊ�����֐��i�e������ASCII�R�[�h�̍��v�j
*	@param ssid SSID
*	@return �ϊ��������l
*/
int Output::ConvertSsidToInt(const std::string& ssid)
{
	int sum = 0;// ���v�l
	for (char c : ssid)sum += static_cast<int>(c); // �������Ƃ�ASCII�R�[�h�����v
	return abs(sum);// ���v�l���Βl�ɕϊ����ĕԂ�
}
