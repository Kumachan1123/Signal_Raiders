/*
*	@file	SettingData.cpp
*	@brief	�ݒ�f�[�^�̓ǂݏ������Ǘ�����N���X
*/
#include <pch.h>
#include "SettingData.h"
/*
*	@brief �R���X�g���N�^
*	@details �e�ݒ荀�ڂ̒l������������
*	@param �Ȃ�
*	@return �Ȃ�
*/
SettingData::SettingData()
	: m_bgmVolume{}			// BGM���ʏ�����
	, m_seVolume{}			// SE���ʏ�����
	, m_mouseSensitivity{}	// �}�E�X���x������
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���ɂ�邱�Ƃ͂Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
SettingData::~SettingData() {/*do nothing*/ }
/*
*	@brief �ݒ�t�@�C����ǂݍ���
*	@details JSON�t�@�C������BGM�ESE���ʁA�}�E�X���x��ǂݎ���ĕۑ�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingData::Load()
{
	using json = nlohmann::json;
	using namespace std;
	string filename = "Resources/Jsons/Setting.json";//�ǂݍ��ރt�@�C���̖��O���쐬
	ifstream ifs(filename.c_str());//�t�@�C�����J��
	if (ifs.good())// �t�@�C��������ɊJ������
	{
		json j;							//json�I�u�W�F�N�g
		ifs >> j;						//�t�@�C������ǂݍ���
		m_bgmVolume = j["BGM"];			// BGM�̉���
		m_seVolume = j["SE"];			// SE�̉���
		m_mouseSensitivity = j["Mouse"];// �}�E�X���x
		ifs.close();					//�t�@�C�������
	}
}
/*
*	@brief �ݒ�t�@�C���ɏ�������
*	@details BGM�ESE���ʁA�}�E�X���x��JSON�t�@�C���ɏ�������
*	@param BGM BGM����
*	@param SE SE����
*	@param Mouse �}�E�X���x
*	@return �Ȃ�
*/
void SettingData::Save(int BGM, int SE, int Mouse)
{
	using json = nlohmann::json;
	using namespace std;
	string filename = "Resources/Setting/Setting.json";//�������ރt�@�C���̖��O���쐬
	ofstream ofs(filename.c_str());//�t�@�C�����J��
	if (ofs.good())// �t�@�C��������ɊJ������
	{
		json j;					//json�I�u�W�F�N�g
		j["BGM"] = BGM;			// BGM�̉���
		j["SE"] = SE;			// SE�̉���
		j["Mouse"] = Mouse;		// �}�E�X���x
		ofs << j;				//�t�@�C���ɏ�������
		ofs.close();			//�t�@�C�������
	}
}