#include "pch.h"
#include "SettingData.h"

SettingData::SettingData()
	: m_bgmVolume{}
	, m_seVolume{}
	, m_mouseSensitivity{}
{
}

SettingData::~SettingData()
{
}

void SettingData::Load()
{
	//�ǂݍ��ރt�@�C���̖��O���쐬
	string filename = "Resources/Setting/Setting.json";

	//�t�@�C�����J��
	ifstream ifs(filename.c_str());
	if (ifs.good())
	{
		json j;//json�I�u�W�F�N�g
		ifs >> j;//�t�@�C������ǂݍ���
		m_bgmVolume = j["BGM"];// BGM�̉���
		m_seVolume = j["SE"];// SE�̉���
		m_mouseSensitivity = j["Mouse"];// �}�E�X���x
		ifs.close();//�t�@�C�������
	}
}

void SettingData::Save(int BGM, int SE, int Mouse)
{
	//�������ރt�@�C���̖��O���쐬
	string filename = "Resources/Setting/Setting.json";

	//�t�@�C�����J��
	ofstream ofs(filename.c_str());
	if (ofs.good())
	{
		json j;//json�I�u�W�F�N�g
		j["BGM"] = BGM;// BGM�̉���
		j["SE"] = SE;// SE�̉���
		j["Mouse"] = Mouse;// �}�E�X���x
		ofs << j;//�t�@�C���ɏ�������
		ofs.close();//�t�@�C�������
	}
}