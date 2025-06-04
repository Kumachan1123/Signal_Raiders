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
	// nlohmann::json���C�u�������g�p����JSON�t�@�C����ǂݍ���
	using json = nlohmann::json;
	// �W�����C�u�������g�p
	using namespace std;
	//�ǂݍ��ރt�@�C���̖��O���쐬
	string filename = "Resources/Setting/Setting.json";
	//�t�@�C�����J��
	ifstream ifs(filename.c_str());
	// �t�@�C��������ɊJ������
	if (ifs.good())
	{
		//json�I�u�W�F�N�g
		json j;
		//�t�@�C������ǂݍ���
		ifs >> j;
		// BGM�̉���
		m_bgmVolume = j["BGM"];
		// SE�̉���
		m_seVolume = j["SE"];
		// �}�E�X���x
		m_mouseSensitivity = j["Mouse"];
		//�t�@�C�������
		ifs.close();
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
	// nlohmann::json���C�u�������g�p����JSON�t�@�C���ɏ�������
	using json = nlohmann::json;
	// �W�����C�u�������g�p
	using namespace std;
	//�������ރt�@�C���̖��O���쐬
	string filename = "Resources/Setting/Setting.json";
	//�t�@�C�����J��
	ofstream ofs(filename.c_str());
	// �t�@�C��������ɊJ������
	if (ofs.good())
	{
		//json�I�u�W�F�N�g
		json j;
		// BGM�̉���
		j["BGM"] = BGM;
		// SE�̉���
		j["SE"] = SE;
		// �}�E�X���x
		j["Mouse"] = Mouse;
		//�t�@�C���ɏ�������
		ofs << j;
		//�t�@�C�������
		ofs.close();
	}
}