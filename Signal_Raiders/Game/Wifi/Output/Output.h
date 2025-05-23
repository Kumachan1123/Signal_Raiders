/*
*	@file		Output.h
*	@brief		���\���Ɋւ���\�[�X�t�@�C��
*/
#pragma once
// �W�����C�u����
#include <sstream>
// ����w�b�_�[�t�@�C��
#include "Game/Screen.h"
#include "Game/Wifi/Interface/IWifiParts.h"
class Output
{
public:// public�֐�
	Output();// �R���X�g���N�^
	~Output();// �f�X�g���N�^
	void DisplayInformation(std::vector<NetworkInfo>& networkInfos);// ����\��
	std::vector<NetworkInfo> GetInformation() { return m_networkInfos; };// �����o��
	void SetInformation(std::vector<NetworkInfo>& networkInfos) { m_networkInfos = networkInfos; }// �����Z�b�g
	std::vector<Datas> GetDatas()const { return m_datas; }// �f�[�^��n��
	int ConvertSsidToInt(const std::string& ssid);// SSID�𖳗���萔�l�ɕϊ�����֐��i�e������ASCII�R�[�h�̍��v�j
private:// private�ϐ�
	std::vector<NetworkInfo> m_networkInfos;// Wi-Fi�����i�[����ϒ��z��
	int m_count;// �\����
	Datas m_data;// ���l�������f�[�^���i�[����ϐ�
	std::vector<Datas> m_datas;// ���l�������f�[�^���i�[����ϒ��z��
};
